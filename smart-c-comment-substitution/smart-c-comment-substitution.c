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

/*
 * Smart substitution (or, search and replace) in C comments. Can be useful to
 * change license headers.
 *
 * Comments with the // style are not supported, only C89-compliant comments
 * are supported, like the comments present in this file.
 *
 * Usage:
 * $ ./smart-c-comment-substitution <search-text-file> <replacement-file> <file1> [file2] ...
 * <file1>, [file2], etc must be *.c or *.h files.
 * Warning: the script modifies directly <file1>, [file2], etc.
 *
 * <search-text-file> should contain a fragment of a C comment. The script
 * canonicalizes its content, to have a list of words to search. When doing the
 * search, the script tries to match the list of words in C comments, by
 * ignoring spacing differences and ignoring the positions of newlines (where a
 * sentence is split).
 *
 * By default the search is case insensitive, but it can be changed with the
 * #define CASE_SENSITIVE below.
 *
 * When a match is found, it is replaced by the content of <replacement-file>.
 */

#include <gtksourceview/gtksource.h>
#include <stdlib.h>
#include <locale.h>

#define CASE_SENSITIVE FALSE

typedef struct _Sub Sub;
struct _Sub
{
  /* List of words (gchar *).
   * Unowned.
   */
  GQueue *canonicalized_search_text;

  gchar *replacement;
  GtkSourceFile *file;
  GtkSourceBuffer *buffer;
};

static Sub *
sub_new (GQueue      *canonicalized_search_text,
         const gchar *replacement,
         const gchar *filename)
{
  Sub *sub = g_new0 (Sub, 1);
  GFile *location;

  g_assert (replacement != NULL);
  g_assert (filename != NULL);
  g_assert (filename[0] != '\0');

  sub->canonicalized_search_text = canonicalized_search_text;

  sub->replacement = g_strdup (replacement);

  sub->file = gtk_source_file_new ();

  location = g_file_new_for_commandline_arg (filename);
  gtk_source_file_set_location (sub->file, location);
  g_object_unref (location);

  sub->buffer = gtk_source_buffer_new (NULL);
  gtk_source_buffer_set_implicit_trailing_newline (sub->buffer, FALSE);

  return sub;
}

static void
sub_free (Sub *sub)
{
  if (sub != NULL)
    {
      g_free (sub->replacement);
      g_clear_object (&sub->file);
      g_clear_object (&sub->buffer);

      g_free (sub);
    }
}

#if 0
static void
print_canonicalized_search_text (GQueue *words)
{
  GList *l;

  g_print ("Canonicalized search text:");

  for (l = words->head; l != NULL; l = l->next)
    {
      const gchar *word = l->data;
      g_print (" %s", word);
    }

  g_print ("\n");
}
#endif

static void
skip_spaces_forward (GtkTextIter *iter)
{
  while (!gtk_text_iter_is_end (iter))
    {
      gunichar ch;

      ch = gtk_text_iter_get_char (iter);
      if (!g_unichar_isspace (ch))
        break;

      gtk_text_iter_forward_char (iter);
    }
}

static void
skip_nonspaces_forward (GtkTextIter *iter)
{
  while (!gtk_text_iter_is_end (iter))
    {
      gunichar ch;

      ch = gtk_text_iter_get_char (iter);
      if (g_unichar_isspace (ch))
        break;

      gtk_text_iter_forward_char (iter);
    }
}

static void
skip_char_forward (GtkTextIter *iter,
                   gunichar     char_to_skip)
{
  while (!gtk_text_iter_is_end (iter))
    {
      gunichar ch;

      ch = gtk_text_iter_get_char (iter);
      if (ch != char_to_skip)
        break;

      gtk_text_iter_forward_char (iter);
    }
}

static void
skip_spaces_backward (GtkTextIter *iter)
{
  while (!gtk_text_iter_is_start (iter))
    {
      GtkTextIter prev;
      gunichar ch;

      prev = *iter;
      gtk_text_iter_backward_char (&prev);

      ch = gtk_text_iter_get_char (&prev);
      if (!g_unichar_isspace (ch))
        break;

      *iter = prev;
    }
}

static void
remove_opening_c_comment (GtkTextBuffer *comment)
{
  GtkTextIter iter;
  GtkTextIter start;
  GtkTextIter end;
  gchar *text;

  gtk_text_buffer_get_start_iter (comment, &iter);
  skip_spaces_forward (&iter);

  start = iter;
  end = start;
  gtk_text_iter_forward_chars (&end, 2);

  text = gtk_text_iter_get_text (&start, &end);
  if (g_strcmp0 (text, "/*") == 0)
    gtk_text_buffer_delete (comment, &start, &end);

  g_free (text);
}

static void
remove_closing_c_comment (GtkTextBuffer *comment)
{
  GtkTextIter iter;
  GtkTextIter start;
  GtkTextIter end;
  gchar *text;

  gtk_text_buffer_get_end_iter (comment, &iter);
  skip_spaces_backward (&iter);

  end = iter;
  start = end;
  gtk_text_iter_backward_chars (&start, 2);

  text = gtk_text_iter_get_text (&start, &end);
  if (g_strcmp0 (text, "*/") == 0)
    gtk_text_buffer_delete (comment, &start, &end);

  g_free (text);
}

static void
remove_leading_stars_at_line (GtkTextBuffer *comment,
                              gint           line_num)
{
  GtkTextIter line_start;
  GtkTextIter end;

  gtk_text_buffer_get_iter_at_line (comment, &line_start, line_num);

  end = line_start;
  skip_spaces_forward (&end);
  skip_char_forward (&end, '*');

  gtk_text_buffer_delete (comment, &line_start, &end);
}

static void
remove_leading_stars (GtkTextBuffer *comment)
{
  gint n_lines;
  gint line_num;

  n_lines = gtk_text_buffer_get_line_count (comment);

  for (line_num = 0; line_num < n_lines; line_num++)
    remove_leading_stars_at_line (comment, line_num);
}

static GQueue *
extract_words (GtkTextBuffer *comment)
{
  GQueue *words;
  GtkTextIter iter;

  words = g_queue_new ();

  gtk_text_buffer_get_start_iter (comment, &iter);

  while (!gtk_text_iter_is_end (&iter))
    {
      GtkTextIter word_start;
      GtkTextIter word_end;

      word_start = iter;
      skip_spaces_forward (&word_start);

      word_end = word_start;
      skip_nonspaces_forward (&word_end);

      if (!gtk_text_iter_equal (&word_start, &word_end))
        {
          gchar *text;

          text = gtk_text_iter_get_text (&word_start, &word_end);
          g_queue_push_tail (words, text);
        }

      iter = word_end;
    }

  return words;
}

static GQueue *
canonicalize_c_comment (const gchar *comment_str)
{
  GtkTextBuffer *comment;
  GQueue *words;

  comment = gtk_text_buffer_new (NULL);
  gtk_text_buffer_set_text (comment, comment_str, -1);

  remove_opening_c_comment (comment);
  remove_closing_c_comment (comment);
  remove_leading_stars (comment);

  words = extract_words (comment);

  g_object_unref (comment);
  return words;
}

static void
get_end_of_leading_stars_at_line (const GtkTextIter *iter,
                                  GtkTextIter       *end_of_leading_stars)
{
  *end_of_leading_stars = *iter;
  gtk_text_iter_set_line_offset (end_of_leading_stars, 0);
  skip_spaces_forward (end_of_leading_stars);
  skip_char_forward (end_of_leading_stars, '*');
}

static void
skip_leading_stars (GtkTextIter *iter)
{
  GtkTextIter end_of_leading_stars;

  get_end_of_leading_stars_at_line (iter, &end_of_leading_stars);

  if (gtk_text_iter_compare (iter, &end_of_leading_stars) < 0)
    *iter = end_of_leading_stars;
}

/* Returns the next word, and moves @iter at the end of the returned word. */
static gchar *
next_word (Sub         *sub,
           GtkTextIter *iter)
{
  GtkTextIter word_start;

  while (TRUE)
    {
      gint line_before;
      gint line_after;

      line_before = gtk_text_iter_get_line (iter);
      skip_leading_stars (iter);
      skip_spaces_forward (iter); /* can go to next line */
      line_after = gtk_text_iter_get_line (iter);

      if (line_before == line_after)
        break;
    }

  word_start = *iter;
  skip_nonspaces_forward (iter);

  if (gtk_text_iter_equal (&word_start, iter))
    return NULL;

  return gtk_text_iter_get_text (&word_start, iter);
}

static gboolean
is_in_c_comment (Sub               *sub,
                 const GtkTextIter *iter)
{
  return gtk_source_buffer_iter_has_context_class (sub->buffer, iter, "comment");
}

static gboolean
is_in_same_c_comment (Sub               *sub,
                      const GtkTextIter *start,
                      const GtkTextIter *end)
{
  GtkTextIter comment_end;

  if (!is_in_c_comment (sub, start))
    return FALSE;

  comment_end = *start;
  gtk_source_buffer_iter_forward_to_context_class_toggle (sub->buffer,
                                                          &comment_end,
                                                          "comment");

  return gtk_text_iter_compare (end, &comment_end) <= 0;
}

static gint
my_strcmp0 (const gchar *str1,
            const gchar *str2)
{
  gchar *str1_casefolded;
  gchar *str2_casefolded;
  gint result;

  if (str1 == NULL || str2 == NULL || CASE_SENSITIVE)
    return g_strcmp0 (str1, str2);

  str1_casefolded = g_utf8_casefold (str1, -1);
  str2_casefolded = g_utf8_casefold (str2, -1);

  result = g_strcmp0 (str1_casefolded, str2_casefolded);

  g_free (str1_casefolded);
  g_free (str2_casefolded);

  return result;
}

static gboolean
match_search_text (Sub               *sub,
                   const GtkTextIter *match_start,
                   GtkTextIter       *match_end)
{
  GtkTextIter iter;
  GList *l;

  if (!is_in_c_comment (sub, match_start))
    return FALSE;

  iter = *match_start;
  for (l = sub->canonicalized_search_text->head; l != NULL; l = l->next)
    {
      const gchar *word_to_match = l->data;
      gchar *word;

      word = next_word (sub, &iter);
      if (my_strcmp0 (word, word_to_match) != 0)
        {
          g_free (word);
          return FALSE;
        }

      g_free (word);
    }

  if (!is_in_same_c_comment (sub, match_start, &iter))
    return FALSE;

  *match_end = iter;
  return TRUE;
}

static void
save_cb (GtkSourceFileSaver *saver,
         GAsyncResult       *result,
         Sub                *sub)
{
  GError *error = NULL;

  gtk_source_file_saver_save_finish (saver, result, &error);
  g_object_unref (saver);

  if (error != NULL)
    g_error ("Error when saving file: %s", error->message);

  gtk_main_quit ();
}

static void
save_file (Sub *sub)
{
  GtkSourceFileSaver *saver;

  saver = gtk_source_file_saver_new (sub->buffer, sub->file);

  gtk_source_file_saver_save_async (saver,
                                    G_PRIORITY_HIGH,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    (GAsyncReadyCallback) save_cb,
                                    sub);
}

static void
do_substitution (Sub *sub)
{
  const gchar *first_word;
  GtkSourceSearchSettings *search_settings;
  GtkSourceSearchContext *search_context;
  GtkTextIter iter;
  GtkTextIter match_start;
  GtkTextIter match_end;

  if (sub->canonicalized_search_text == NULL ||
      g_queue_is_empty (sub->canonicalized_search_text))
    return;

  first_word = g_queue_peek_head (sub->canonicalized_search_text);

  search_settings = gtk_source_search_settings_new ();
  gtk_source_search_settings_set_search_text (search_settings, first_word);
  gtk_source_search_settings_set_case_sensitive (search_settings, CASE_SENSITIVE);

  search_context = gtk_source_search_context_new (sub->buffer, search_settings);

  gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (sub->buffer), &iter);

  while (gtk_source_search_context_forward2 (search_context,
                                             &iter,
                                             &match_start,
                                             &match_end,
                                             NULL))
    {
      if (match_search_text (sub, &match_start, &match_end))
        {
          gtk_text_buffer_begin_user_action (GTK_TEXT_BUFFER (sub->buffer));
          gtk_text_buffer_delete (GTK_TEXT_BUFFER (sub->buffer), &match_start, &match_end);
          gtk_text_buffer_insert (GTK_TEXT_BUFFER (sub->buffer), &match_end, sub->replacement, -1);
          gtk_text_buffer_end_user_action (GTK_TEXT_BUFFER (sub->buffer));
        }

      iter = match_end;
    }

  g_object_unref (search_settings);
  g_object_unref (search_context);
}

static void
set_c_language (Sub *sub)
{
  GtkSourceLanguageManager *manager;
  GtkSourceLanguage *c_language;
  GtkTextIter start;
  GtkTextIter end;

  manager = gtk_source_language_manager_get_default ();
  c_language = gtk_source_language_manager_get_language (manager, "c");
  if (c_language == NULL)
    g_error ("GtkSourceLanguage for the C language not found. "
             "Check your GtkSourceView installation.");

  gtk_source_buffer_set_language (sub->buffer, c_language);

  gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER (sub->buffer), &start, &end);
  gtk_source_buffer_ensure_highlight (sub->buffer, &start, &end);
}

static void
load_cb (GtkSourceFileLoader *loader,
         GAsyncResult        *result,
         Sub                 *sub)
{
  GError *error = NULL;

  gtk_source_file_loader_load_finish (loader, result, &error);
  g_object_unref (loader);

  if (error != NULL)
    {
      g_warning ("Error when loading file: %s", error->message);
      g_clear_error (&error);
      gtk_main_quit ();
      return;
    }

  set_c_language (sub);
  do_substitution (sub);
  save_file (sub);
}

static void
sub_launch (Sub *sub)
{
  GtkSourceFileLoader *loader;

  loader = gtk_source_file_loader_new (sub->buffer, sub->file);

  gtk_source_file_loader_load_async (loader,
                                     G_PRIORITY_HIGH,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     (GAsyncReadyCallback) load_cb,
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

static void
remove_prefix (const gchar  *text1,
               const gchar  *text2,
               gchar       **new_text1,
               gchar       **new_text2)
{
  gint i;

  g_assert (text1 != NULL);
  g_assert (text2 != NULL);
  g_assert (new_text1 != NULL && *new_text1 == NULL);
  g_assert (new_text2 != NULL && *new_text2 == NULL);

  i = 0;
  while (text1[i] == text2[i] &&
         (g_unichar_isspace (text1[i]) ||
          text1[i] == '*'))
    i++;

  *new_text1 = g_strdup (text1 + i);
  *new_text2 = g_strdup (text2 + i);
}

gint
main (gint   argc,
      gchar *argv[])
{
  const gchar *search_text_path;
  const gchar *replacement_path;
  gchar *full_search_text;
  gchar *full_replacement;
  gchar *search_text = NULL;
  gchar *replacement = NULL;
  GQueue *canonicalized_search_text;
  gint arg_num;

  setlocale (LC_ALL, "");

  gtk_init (NULL, NULL);

  if (argc < 4)
    {
      g_printerr ("Usage: %s <search-text-file> <replacement-file> <file1> [file2] ...\n", argv[0]);
      g_printerr ("WARNING: the script modifies the files!\n");
      return EXIT_FAILURE;
    }

  search_text_path = argv[1];
  replacement_path = argv[2];

  full_search_text = get_file_contents (search_text_path);
  full_replacement = get_file_contents (replacement_path);

  remove_prefix (full_search_text,
                 full_replacement,
                 &search_text,
                 &replacement);

  g_strstrip (search_text);
  g_strstrip (replacement);

  canonicalized_search_text = canonicalize_c_comment (search_text);
#if 0
  print_canonicalized_search_text (canonicalized_search_text);
#endif

  /* Launching a GTask for each file would be so boring, let's have some fun
   * with the GTK main loop!
   */
  for (arg_num = 3; arg_num < argc; arg_num++)
    {
      const gchar *filename = argv[arg_num];
      Sub *sub;

      g_print ("Processing %s\n", filename);

      sub = sub_new (canonicalized_search_text, replacement, filename);
      sub_launch (sub);
      gtk_main ();
      sub_free (sub);
    }

  g_free (full_search_text);
  g_free (full_replacement);
  g_free (search_text);
  g_free (replacement);
  g_queue_free_full (canonicalized_search_text, g_free);

  return EXIT_SUCCESS;
}
