/*
 * This file is part of gnome-c-utils.
 *
 * Copyright © 2016 Sébastien Wilmet <swilmet@gnome.org>
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

/* Note: yes, this script uses GTK+ and GtkSourceView, because
 * GtkSourceBuffer/GtkTextBuffer are good at navigating through text and editing
 * it. Another reason is because I'm familiar with those APIs, so it was easier
 * for me to write the code. And in the future, this code might be useful for a
 * graphical text editor based on GtkSourceView.
 */
#include <gtksourceview/gtksource.h>
#include <stdlib.h>

static GtkSourceBuffer *
open_file (GFile *file)
{
  gchar *content;
  GtkSourceBuffer *buffer;
  GError *error = NULL;

  g_file_load_contents (file, NULL, &content, NULL, NULL, &error);
  g_assert_no_error (error);

  buffer = gtk_source_buffer_new (NULL);
  gtk_text_buffer_set_text (GTK_TEXT_BUFFER (buffer), content, -1);

  g_free (content);
  return buffer;
}

static gchar *
get_function_name (const GtkTextIter *_iter)
{
  GtkTextIter iter = *_iter;

  while (gtk_text_iter_backward_line (&iter))
    {
      gunichar c;

      c = gtk_text_iter_get_char (&iter);
      if (g_unichar_isalpha (c) || c == '_')
        {
          GtkTextIter function_name_start;
          GtkTextIter function_name_end;

          function_name_start = iter;
          function_name_end = iter;

          while (gtk_text_iter_forward_char (&function_name_end))
            {
              gunichar end_c;

              end_c = gtk_text_iter_get_char (&function_name_end);
              if (!g_unichar_isalpha (end_c) && end_c != '_')
                break;
            }

          /* A goto label, not a function name. */
          if (gtk_text_iter_get_char (&function_name_end) == ':')
            continue;

          return gtk_text_iter_get_text (&function_name_start, &function_name_end);
        }
    }

  return NULL;
}

static void
check_chain_up (GtkSourceBuffer   *buffer,
		const GtkTextIter *vfunc_start)
{
  gchar *function_name;
  GtkTextIter vfunc_end;
  gchar *vfunc;

  function_name = get_function_name (vfunc_start);
  if (function_name == NULL)
    return;

  if (!gtk_text_iter_starts_word (vfunc_start))
    return;

  vfunc_end = *vfunc_start;
  while (TRUE)
    {
      if (gtk_text_iter_get_char (&vfunc_end) == '_')
        gtk_text_iter_forward_char (&vfunc_end);
      else if (gtk_text_iter_starts_word (&vfunc_end))
        gtk_text_iter_forward_word_end (&vfunc_end);
      else
        break;
    }

  vfunc = gtk_text_buffer_get_text (GTK_TEXT_BUFFER (buffer),
                                    vfunc_start,
                                    &vfunc_end,
                                    FALSE);

  if (g_str_has_suffix (function_name, vfunc))
    {
      g_print ("%s(): OK\n", function_name);
    }
  else
    {
      g_printerr ("%s() chains up '%s'. Is that correct?\n", function_name, vfunc);
    }

  g_free (function_name);
  g_free (vfunc);
}

static void
check_buffer (GtkSourceBuffer *buffer)
{
  GtkSourceSearchSettings *search_settings;
  GtkSourceSearchContext *search_context;
  GtkTextIter iter;
  GtkTextIter match_end;

  search_settings = gtk_source_search_settings_new ();
  gtk_source_search_settings_set_regex_enabled (search_settings, TRUE);

  /* Search "_parent_class)->", with possible spaces. */
  gtk_source_search_settings_set_search_text (search_settings, "_parent_class\\s*\\)\\s*->\\s*");

  search_context = gtk_source_search_context_new (buffer, search_settings);

  gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (buffer), &iter);

  while (gtk_source_search_context_forward2 (search_context,
                                             &iter,
                                             NULL,
                                             &match_end,
                                             NULL))
    {
      check_chain_up (buffer, &match_end);
      iter = match_end;
    }

  g_object_unref (search_settings);
  g_object_unref (search_context);
}

gint
main (gint   argc,
      gchar *argv[])
{
  gint arg_num;

  gtk_init (NULL, NULL);

  if (argc < 2)
    {
      g_printerr ("Usage: %s <file1> [file2] ...\n", argv[0]);
      return EXIT_FAILURE;
    }

  for (arg_num = 1; arg_num < argc; arg_num++)
    {
      const gchar *path;
      GFile *file;
      GtkSourceBuffer *buffer;

      path = argv[arg_num];
      g_print ("File: %s\n", path);

      file = g_file_new_for_path (path);
      buffer = open_file (file);
      check_buffer (buffer);

      g_print ("\n");

      g_object_unref (file);
      g_object_unref (buffer);
    }

  return EXIT_SUCCESS;
}
