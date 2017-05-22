/*
 * This file is part of gnome-c-utils.
 *
 * Copyright © 2016, 2017 Sébastien Wilmet <swilmet@gnome.org>
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
 * Does a multi-line substitution (or, multi-line search and replace).
 *
 * Usage:
 * $ gcu-multi-line-substitution <search-text-file> <replacement-file> <file1> [file2] ...
 * WARNING: the script directly modifies <file1>, [file2], ... without doing
 * backups first!
 *
 * Example:
 * $ gcu-multi-line-substitution license-header-old license-header-new *.[ch]
 */

/* Note: yes, this script uses GTK+ and GtkSourceView, because
 * GtkSourceBuffer/GtkTextBuffer are good at navigating through text and editing
 * it. Another reason is because I'm familiar with those APIs, so it was easier
 * for me to write the code. And in the future, this code might be useful for a
 * graphical text editor based on GtkSourceView.
 */
#include <gtef/gtef.h>
#include <stdlib.h>
#include <locale.h>

typedef struct _Sub Sub;
struct _Sub
{
  gchar *search_text;
  gchar *replacement;
  GtefBuffer *buffer;
};

static Sub *
sub_new (const gchar *search_text,
         const gchar *replacement,
         const gchar *filename)
{
  Sub *sub = g_new0 (Sub, 1);
  GFile *location;
  GtefFile *file;

  g_assert (search_text != NULL);
  g_assert (search_text[0] != '\0');
  g_assert (replacement != NULL);
  g_assert (filename != NULL);
  g_assert (filename[0] != '\0');

  sub->search_text = g_strdup (search_text);
  sub->replacement = g_strdup (replacement);

  sub->buffer = gtef_buffer_new ();
  gtk_source_buffer_set_implicit_trailing_newline (GTK_SOURCE_BUFFER (sub->buffer), FALSE);

  location = g_file_new_for_commandline_arg (filename);
  file = gtef_buffer_get_file (sub->buffer);
  gtef_file_set_location (file, location);
  g_object_unref (location);

  return sub;
}

static void
sub_free (Sub *sub)
{
  if (sub != NULL)
    {
      g_free (sub->search_text);
      g_free (sub->replacement);
      g_clear_object (&sub->buffer);

      g_free (sub);
    }
}

static void
save_cb (GObject      *source_object,
         GAsyncResult *result,
         gpointer      user_data)
{
  GtefFileSaver *saver = GTEF_FILE_SAVER (source_object);
  GError *error = NULL;

  gtef_file_saver_save_finish (saver, result, &error);
  g_object_unref (saver);

  if (error != NULL)
    g_error ("Error when saving file: %s", error->message);

  gtk_main_quit ();
}

static void
save_file (Sub *sub)
{
  GtefFile *file;
  GtefFileSaver *saver;

  file = gtef_buffer_get_file (sub->buffer);
  saver = gtef_file_saver_new (sub->buffer, file);

  gtef_file_saver_save_async (saver,
                              G_PRIORITY_HIGH,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              save_cb,
                              NULL);
}

static void
do_substitution (Sub *sub)
{
  GtkSourceSearchSettings *search_settings;
  GtkSourceSearchContext *search_context;
  GtkTextIter iter;
  GtkTextIter match_start;
  GtkTextIter match_end;

  search_settings = gtk_source_search_settings_new ();
  gtk_source_search_settings_set_search_text (search_settings, sub->search_text);
  gtk_source_search_settings_set_case_sensitive (search_settings, TRUE);

  search_context = gtk_source_search_context_new (GTK_SOURCE_BUFFER (sub->buffer),
                                                  search_settings);

  gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (sub->buffer), &iter);

  while (gtk_source_search_context_forward2 (search_context,
                                             &iter,
                                             &match_start,
                                             &match_end,
                                             NULL))
    {
      GError *error = NULL;

      gtk_source_search_context_replace2 (search_context,
                                          &match_start,
                                          &match_end,
                                          sub->replacement, -1,
                                          &error);

      if (error != NULL)
        g_error ("Error when doing the substitution: %s", error->message);

      iter = match_end;
    }

  g_object_unref (search_settings);
  g_object_unref (search_context);
}

static void
load_cb (GObject      *source_object,
         GAsyncResult *result,
         gpointer      user_data)
{
  GtefFileLoader *loader = GTEF_FILE_LOADER (source_object);
  Sub *sub = user_data;
  GError *error = NULL;

  gtef_file_loader_load_finish (loader, result, &error);
  g_object_unref (loader);

  if (error != NULL)
    g_error ("Error when loading file: %s", error->message);

  do_substitution (sub);
  save_file (sub);
}

static void
sub_launch (Sub *sub)
{
  GtefFile *file;
  GtefFileLoader *loader;

  file = gtef_buffer_get_file (sub->buffer);
  loader = gtef_file_loader_new (sub->buffer, file);

  gtef_file_loader_load_async (loader,
                               G_PRIORITY_HIGH,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               load_cb,
                               sub);
}

static gchar *
get_file_contents (const gchar *filename)
{
  gchar *contents;
  GError *error = NULL;

  g_file_get_contents (filename, &contents, NULL, &error);
  g_assert_no_error (error);

  return contents;
}

gint
main (gint   argc,
      gchar *argv[])
{
  const gchar *search_text_path;
  const gchar *replacement_path;
  gchar *search_text;
  gchar *replacement;
  gint arg_num;

  setlocale (LC_ALL, "");

  gtk_init (NULL, NULL);

  if (argc < 4)
    {
      g_printerr ("Usage: %s <search-text-file> <replacement-file> <file1> [file2] ...\n", argv[0]);
      g_printerr ("WARNING: the script directly modifies <file1>, [file2], ... without doing backups first!\n");
      return EXIT_FAILURE;
    }

  search_text_path = argv[1];
  replacement_path = argv[2];

  search_text = get_file_contents (search_text_path);
  replacement = get_file_contents (replacement_path);

  /* Launching a GTask for each file would be so boring, let's have some fun
   * with the GTK main loop!
   */
  for (arg_num = 3; arg_num < argc; arg_num++)
    {
      const gchar *filename = argv[arg_num];
      Sub *sub;

      sub = sub_new (search_text, replacement, filename);
      sub_launch (sub);
      gtk_main ();
      sub_free (sub);
    }

  g_free (search_text);
  g_free (replacement);

  return EXIT_SUCCESS;
}
