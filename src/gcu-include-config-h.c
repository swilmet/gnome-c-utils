/*
 * This file is part of gnome-c-utils.
 *
 * Copyright © 2017 Sébastien Wilmet <swilmet@gnome.org>
 *
 * gnome-c-utils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gnome-c-utils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gnome-c-utils.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Usage:
 * $ gcu-include-config-h <file.c>
 * WARNING: the script directly modifies the file without doing a backup first!
 *
 * Ensures that the file includes config.h as follows:
 * #if HAVE_CONFIG_H
 * #include <config.h>
 * #endif
 *
 * If config.h is already included differently, it is replaced by the above
 * snippet. The snippet is always inserted as the first #include.
 */

#include <tepl/tepl.h>
#include <stdlib.h>
#include <locale.h>

static void
save_file_cb (GObject      *source_object,
              GAsyncResult *result,
              gpointer      user_data)
{
  TeplFileSaver *saver = TEPL_FILE_SAVER (source_object);
  GError *error = NULL;

  tepl_file_saver_save_finish (saver, result, &error);
  g_object_unref (saver);
  g_assert_no_error (error);

  gtk_main_quit ();
}

static void
save_file (TeplBuffer *buffer)
{
  TeplFile *file;
  TeplFileSaver *saver;

  file = tepl_buffer_get_file (buffer);
  saver = tepl_file_saver_new (buffer, file);

  tepl_file_saver_save_async (saver,
                              G_PRIORITY_DEFAULT,
                              NULL,
                              save_file_cb,
                              NULL);
}

static gboolean
find_include_config (GtkSourceBuffer *buffer,
                     GtkTextIter     *match_start,
                     GtkTextIter     *match_end)
{
  GtkSourceSearchSettings *search_settings;
  GtkSourceSearchContext *search_context;
  GtkTextIter start;
  gboolean found;

  search_settings = gtk_source_search_settings_new ();
  gtk_source_search_settings_set_regex_enabled (search_settings, TRUE);
  gtk_source_search_settings_set_case_sensitive (search_settings, TRUE);

  /* The regex is not perfect but it's good enough for my needs. */
  gtk_source_search_settings_set_search_text (search_settings,
                                              "(^#if(def)?\\s+HAVE_CONFIG_H\\s*$\\n)?"
                                              "^#\\s*include\\s+(\"config\\.h\"|<config\\.h>)\\s*$\\n"
                                              "(^#endif\\s*$\\n)?"
                                              "(\\n\\s)*");

  search_context = gtk_source_search_context_new (buffer, search_settings);

  gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (buffer), &start);

  found = gtk_source_search_context_forward (search_context,
                                             &start,
                                             match_start,
                                             match_end,
                                             NULL);

  g_object_unref (search_settings);
  g_object_unref (search_context);

  return found;
}

static void
remove_existing_include_config (TeplBuffer *buffer)
{
  GtkTextIter match_start;
  GtkTextIter match_end;

  if (find_include_config (GTK_SOURCE_BUFFER (buffer), &match_start, &match_end))
    gtk_text_buffer_delete (GTK_TEXT_BUFFER (buffer), &match_start, &match_end);
}

/* FIXME: sometimes the first #include is inside an #if, #ifdef, etc.
 * Look at previous lines and skip them if they start with "#if".
 */
static gboolean
find_first_include (GtkSourceBuffer *buffer,
                    GtkTextIter     *iter)
{
  GtkSourceSearchSettings *search_settings;
  GtkSourceSearchContext *search_context;
  GtkTextIter start;
  gboolean found;

  search_settings = gtk_source_search_settings_new ();
  gtk_source_search_settings_set_regex_enabled (search_settings, TRUE);
  gtk_source_search_settings_set_case_sensitive (search_settings, TRUE);
  gtk_source_search_settings_set_search_text (search_settings, "^#include");

  search_context = gtk_source_search_context_new (buffer, search_settings);

  gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (buffer), &start);

  found = gtk_source_search_context_forward (search_context,
                                             &start,
                                             iter,
                                             NULL,
                                             NULL);

  g_object_unref (search_settings);
  g_object_unref (search_context);

  return found;
}

static void
insert_include_config (TeplBuffer *buffer)
{
  GtkTextIter pos;

  if (!find_first_include (GTK_SOURCE_BUFFER (buffer), &pos))
    {
      TeplFile *file;
      gchar *filename;

      /* I don't know where to insert the #include. */
      file = tepl_buffer_get_file (buffer);
      filename = g_file_get_parse_name (tepl_file_get_location (file));
      g_warning ("%s: first #include not found.", filename);
      g_free (filename);
      return;
    }

  gtk_text_buffer_insert (GTK_TEXT_BUFFER (buffer),
                          &pos,
                          "#ifdef HAVE_CONFIG_H\n"
                          "#include <config.h>\n"
                          "#endif\n\n",
                          -1);
}

static void
load_file_cb (GObject      *source_object,
              GAsyncResult *result,
              gpointer      user_data)
{
  TeplFileLoader *loader = TEPL_FILE_LOADER (source_object);
  TeplBuffer *buffer = TEPL_BUFFER (user_data);
  GError *error = NULL;

  tepl_file_loader_load_finish (loader, result, &error);
  g_object_unref (loader);
  g_assert_no_error (error);

  remove_existing_include_config (buffer);
  insert_include_config (buffer);
  save_file (buffer);
}

static void
load_file (TeplBuffer *buffer)
{
  TeplFile *file;
  TeplFileLoader *loader;

  file = tepl_buffer_get_file (buffer);
  loader = tepl_file_loader_new (buffer, file);

  tepl_file_loader_load_async (loader,
                               G_PRIORITY_DEFAULT,
                               NULL,
                               load_file_cb,
                               buffer);
}

int
main (int    argc,
      char **argv)
{
  GFile *location;
  TeplBuffer *buffer;
  TeplFile *file;

  setlocale (LC_ALL, "");
  gtk_init (NULL, NULL);

  if (argc != 2)
    {
      g_printerr ("Usage: %s <file.c>\n", argv[0]);
      g_printerr ("WARNING: the script directly modifies the file without doing a backup first!\n");
      return EXIT_FAILURE;
    }

  location = g_file_new_for_commandline_arg (argv[1]);

  buffer = tepl_buffer_new ();
  file = tepl_buffer_get_file (buffer);
  tepl_file_set_location (file, location);

  load_file (buffer);

  gtk_main ();

  g_object_unref (location);
  g_object_unref (buffer);

  return EXIT_SUCCESS;
}
