/*
 * This file is part of gnome-c-utils.
 *
 * Copyright © 2013 Sébastien Wilmet <swilmet@gnome.org>
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
 * Line up parameters of function declarations.
 *
 * Usage: lineup-parameters [file]
 * If the file is given, its contents is replaced.
 * If the file is not given, stdin is read and the result is printed to stdout.
 *
 * The restrictions:
 * - The function name must be at column 0, followed by a space and an opening
 *   parenthesis;
 * - One parameter per line;
 * - A parameter must follow certain rules (see the regex in the code), but it
 *   doesn't accept all possibilities of the C language.
 * - The opening curly brace ("{") of the function must also be at column 0.
 *
 * If one restriction is missing, the function declaration is not modified.
 *
 * Example:
 *
 * gboolean
 * frobnitz (Frobnitz *frobnitz,
 *           gint magic_number,
 *           GError **error)
 * {
 *   ...
 * }
 *
 * Becomes:
 *
 * gboolean
 * frobnitz (Frobnitz  *frobnitz,
 *           gint       magic_number,
 *           GError   **error)
 * {
 *   ...
 * }
 */

/*
 * Use with Vim:
 *
 * Although this script can be used in Vim (or other text editors), a Vim plugin
 * exists:
 * http://damien.lespiau.name/blog/2009/12/07/aligning-c-function-parameters-with-vim/
 *
 * You can use a selection:
 * - place the cursor at the function's name;
 * - press V to start the line selection;
 * - press ]] to go to the "{";
 * - type ":" followed by "!lineup-parameters".
 *
 * Note: the "{" is required in the selection, to detect that we are in a
 * function declaration.
 *
 * You can easily map these steps with a keybinding (F8 in the example below).
 * Note that I'm not a Vim expert, so there is maybe a better way to configure
 * this stuff.
 *
 * function! LineupParameters()
 *         let l:winview = winsaveview()
 *         execute "normal {V]]:!lineup-parameters\<CR>"
 *         call winrestview(l:winview)
 * endfunction
 *
 * autocmd Filetype c map <F8> :call LineupParameters()<CR>
 */

/* TODO support "..." vararg parameter. */

#include <gio/gio.h>
#include <gio/gunixinputstream.h>
#include <gio/gunixoutputstream.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#define USE_TABS FALSE

typedef struct
{
  gchar *type;
  guint nb_stars;
  gchar *name;
} ParameterInfo;

static void
parameter_info_free (ParameterInfo *param_info)
{
  g_free (param_info->type);
  g_free (param_info->name);
  g_slice_free (ParameterInfo, param_info);
}

static void
write_to_output_stream (GOutputStream *output_stream,
                        const gchar   *str)
{
  gsize bytes_written;
  GError *error = NULL;

  g_output_stream_write_all (output_stream,
                             str,
                             strlen (str),
                             &bytes_written,
                             NULL,
                             &error);
  g_assert_no_error (error);
}

static gboolean
match_function_name (const gchar  *line,
                     gchar       **function_name,
                     gint         *first_param_pos)
{
  static GRegex *regex = NULL;
  GMatchInfo *match_info;
  gint end_pos;
  gboolean match = FALSE;

  if (G_UNLIKELY (regex == NULL))
    regex = g_regex_new ("^(\\w+) ?\\(", G_REGEX_OPTIMIZE, 0, NULL);

  g_regex_match (regex, line, 0, &match_info);

  if (g_match_info_matches (match_info) &&
      g_match_info_fetch_pos (match_info, 1, NULL, &end_pos) &&
      g_match_info_fetch_pos (match_info, 0, NULL, first_param_pos))
    {
      match = TRUE;

      if (function_name != NULL)
        *function_name = g_strndup (line, end_pos);
    }

  g_match_info_free (match_info);
  return match;
}

static gboolean
match_parameter (gchar          *line,
                 ParameterInfo **info,
                 gboolean       *is_last_parameter)
{
  static GRegex *regex = NULL;
  GMatchInfo *match_info;
  gint start_pos = 0;

  if (G_UNLIKELY (regex == NULL))
    regex = g_regex_new ("^\\s*(?<type>(const\\s+)?\\w+)\\s+(?<stars>\\**)\\s*(?<name>\\w+)\\s*(?<end>,|\\))\\s*$",
                         G_REGEX_OPTIMIZE,
                         0,
                         NULL);

  if (is_last_parameter != NULL)
    *is_last_parameter = FALSE;

  match_function_name (line, NULL, &start_pos);

  g_regex_match (regex, line + start_pos, 0, &match_info);

  if (!g_match_info_matches (match_info))
    {
      g_match_info_free (match_info);
      return FALSE;
    }

  if (info != NULL)
    {
      gchar *stars;

      *info = g_slice_new0 (ParameterInfo);

      (*info)->type = g_match_info_fetch_named (match_info, "type");
      (*info)->name = g_match_info_fetch_named (match_info, "name");
      g_assert ((*info)->type != NULL);
      g_assert ((*info)->name != NULL);

      stars = g_match_info_fetch_named (match_info, "stars");
      (*info)->nb_stars = strlen (stars);
      g_free (stars);
    }

  if (is_last_parameter != NULL)
    {
      gchar *end = g_match_info_fetch_named (match_info, "end");
      *is_last_parameter = g_str_equal (end, ")");
      g_free (end);
    }

  g_match_info_free (match_info);
  return TRUE;
}

static gboolean
match_opening_curly_brace (const gchar *line)
{
  static GRegex *regex = NULL;

  if (G_UNLIKELY (regex == NULL))
    regex = g_regex_new ("^{\\s*$", G_REGEX_OPTIMIZE, 0, NULL);

  return g_regex_match (regex, line, 0, NULL);
}

/* Returns the number of lines that take the function declaration.
 * Returns 0 if not a function declaration. */
static guint
get_function_declaration_length (gchar **lines)
{
  guint nb_lines = 1;
  gchar **cur_line = lines;

  while (*cur_line != NULL)
    {
      gboolean match_param;
      gboolean is_last_param;

      match_param = match_parameter (*cur_line, NULL, &is_last_param);

      if (is_last_param)
        {
          gchar *next_line = *(cur_line + 1);

          if (next_line == NULL ||
              !match_opening_curly_brace (next_line))
            return 0;

          return nb_lines;
        }

      if (!match_param)
        return 0;

      nb_lines++;
      cur_line++;
    }

  /* Should not be reachable - but silences a compiler warning. */
  return 0;
}

static GSList *
get_list_parameter_infos (gchar **lines,
                          guint   length)
{
  GSList *list = NULL;
  gint i;

  for (i = length - 1; i >= 0; i--)
    {
      ParameterInfo *info = NULL;

      match_parameter (lines[i], &info, NULL);
      g_assert (info != NULL);

      list = g_slist_prepend (list, info);
    }

  return list;
}

static void
compute_spacing (GSList *parameter_infos,
                 guint  *max_type_length,
                 guint  *max_stars_length)
{
  GSList *l;
  *max_type_length = 0;
  *max_stars_length = 0;

  for (l = parameter_infos; l != NULL; l = l->next)
    {
      ParameterInfo *info = l->data;
      guint type_length = strlen (info->type);

      if (type_length > *max_type_length)
        *max_type_length = type_length;

      if (info->nb_stars > *max_stars_length)
        *max_stars_length = info->nb_stars;
    }
}

static void
print_parameter (GOutputStream *output_stream,
                 ParameterInfo *info,
                 guint          max_type_length,
                 guint          max_stars_length)
{
  gint type_length;
  gint nb_spaces;
  gchar *spaces;
  gchar *stars;

  write_to_output_stream (output_stream, info->type);

  type_length = strlen (info->type);
  nb_spaces = max_type_length - type_length;
  g_assert (nb_spaces >= 0);

  spaces = g_strnfill (nb_spaces, ' ');
  write_to_output_stream (output_stream, spaces);
  write_to_output_stream (output_stream, " ");
  g_free (spaces);

  nb_spaces = max_stars_length - info->nb_stars;
  g_assert (nb_spaces >= 0);
  spaces = g_strnfill (nb_spaces, ' ');
  write_to_output_stream (output_stream, spaces);
  g_free (spaces);

  stars = g_strnfill (info->nb_stars, '*');
  write_to_output_stream (output_stream, stars);
  g_free (stars);

  write_to_output_stream (output_stream, info->name);
}

static void
print_function_declaration (GOutputStream  *output_stream,
                            gchar         **lines,
                            guint           length)
{
  gchar **cur_line = lines;
  gchar *function_name;
  gint nb_spaces_to_parenthesis;
  GSList *parameter_infos;
  GSList *l;
  guint max_type_length;
  guint max_stars_length;
  gchar *spaces;

  if (!match_function_name (*cur_line, &function_name, NULL))
    g_error ("The line doesn't match a function name.");

  write_to_output_stream (output_stream, function_name);
  write_to_output_stream (output_stream, " (");

  nb_spaces_to_parenthesis = strlen (function_name) + 2;

  if (USE_TABS)
    {
      gchar *tabs = g_strnfill (nb_spaces_to_parenthesis / 8, '\t');
      gchar *spaces_after_tabs = g_strnfill (nb_spaces_to_parenthesis % 8, ' ');

      spaces = g_strdup_printf ("%s%s", tabs, spaces_after_tabs);

      g_free (tabs);
      g_free (spaces_after_tabs);
    }
  else
    {
      spaces = g_strnfill (nb_spaces_to_parenthesis, ' ');
    }

  parameter_infos = get_list_parameter_infos (lines, length);
  compute_spacing (parameter_infos, &max_type_length, &max_stars_length);

  for (l = parameter_infos; l != NULL; l = l->next)
    {
      ParameterInfo *info = l->data;

      if (l != parameter_infos)
        write_to_output_stream (output_stream, spaces);

      print_parameter (output_stream, info, max_type_length, max_stars_length);

      if (l->next != NULL)
        write_to_output_stream (output_stream, ",\n");
    }

  write_to_output_stream (output_stream, ")\n");

  g_free (function_name);
  g_free (spaces);
  g_slist_free_full (parameter_infos, (GDestroyNotify)parameter_info_free);
}

static void
parse_contents (GOutputStream  *output_stream,
                gchar         **lines)
{
  gchar **cur_line = lines;

  /* Skip the empty last line, to avoid adding an extra \n. */
  for (cur_line = lines; cur_line[0] != NULL && cur_line[1] != NULL; cur_line++)
    {
      guint length;

      if (!match_function_name (*cur_line, NULL, NULL))
        {
          write_to_output_stream (output_stream, *cur_line);
          write_to_output_stream (output_stream, "\n");
          continue;
        }

      length = get_function_declaration_length (cur_line);

      if (length == 0)
        {
          write_to_output_stream (output_stream, *cur_line);
          write_to_output_stream (output_stream, "\n");
          continue;
        }

      print_function_declaration (output_stream, cur_line, length);

      cur_line += length - 1;
    }
}

static gchar *
get_file_contents (GFile *file)
{
  gchar *path;
  gchar *contents;
  GError *error = NULL;

  path = g_file_get_path (file);
  g_file_get_contents (path, &contents, NULL, &error);

  if (error != NULL)
    g_error ("Impossible to get file contents: %s", error->message);

  g_free (path);
  return contents;
}

static gchar *
get_stdin_contents (void)
{
  GInputStream *stream;
  GString *string;
  GError *error = NULL;

  stream = g_unix_input_stream_new (STDIN_FILENO, FALSE);
  string = g_string_new ("");

  while (TRUE)
    {
      gchar buffer[4097] = { '\0' };
      gssize nb_bytes_read = g_input_stream_read (stream, buffer, 4096, NULL, &error);

      if (nb_bytes_read == 0)
        break;

      if (error != NULL)
        g_error ("Impossible to read stdin: %s", error->message);

      g_string_append (string, buffer);
    }

  g_input_stream_close (stream, NULL, NULL);
  g_object_unref (stream);

  return g_string_free (string, FALSE);
}

static GOutputStream *
get_file_output_stream (GFile *file)
{
  GFileOutputStream *output_stream;
  GError *error = NULL;

  output_stream = g_file_replace (file,
                                  NULL,
                                  FALSE,
                                  G_FILE_CREATE_NONE,
                                  NULL,
                                  &error);
  g_assert_no_error (error);

  return G_OUTPUT_STREAM (output_stream);
}

static GOutputStream *
get_stdout_output_stream (void)
{
  return g_unix_output_stream_new (STDOUT_FILENO, FALSE);
}

static void
handle_stdin (void)
{
  gchar *contents;
  gchar **contents_lines;
  GOutputStream *output_stream;
  GError *error = NULL;

  contents = get_stdin_contents ();
  contents_lines = g_strsplit (contents, "\n", 0);
  g_free (contents);

  output_stream = get_stdout_output_stream ();

  parse_contents (output_stream, contents_lines);

  g_output_stream_close (output_stream, NULL, &error);
  g_assert_no_error (error);
  g_object_unref (output_stream);
}

static void
handle_file (GFile *file)
{
  gchar *contents;
  gchar **contents_lines;
  GOutputStream *output_stream;
  GError *error = NULL;

  contents = get_file_contents (file);
  contents_lines = g_strsplit (contents, "\n", 0);
  g_free (contents);

  output_stream = get_file_output_stream (file);

  parse_contents (output_stream, contents_lines);

  g_output_stream_close (output_stream, NULL, &error);
  g_assert_no_error (error);
  g_object_unref (output_stream);
}

gint
main (gint   argc,
      gchar *argv[])
{
  setlocale (LC_ALL, "");

  if (argc > 2)
    {
      g_printerr ("Usage: %s [file]\n", argv[0]);
      return EXIT_FAILURE;
    }

  if (argc == 2)
    {
      GFile *file;

      file = g_file_new_for_commandline_arg (argv[1]);
      handle_file (file);
      g_object_unref (file);
    }
  else
    {
      handle_stdin ();
    }

  return EXIT_SUCCESS;
}
