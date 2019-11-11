/*
 * This file is part of gnome-c-utils.
 *
 * Copyright © 2018 Sébastien Wilmet <swilmet@gnome.org>
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
 * Align parameters on the opening parenthesis, made to be integrated in a text
 * editor.
 *
 * For example:
 * function_call (param1,
 *                param2,
 *                param3);
 *
 * Reads stdin and writes the result to stdout. The first line must be the line
 * containing the opening parenthesis. The following lines will be aligned
 * according to the first line. As such, the script doesn't work on whole files,
 * only one function call must be given to stdin.
 */

/*
 * Use with Vim:
 *
 * For some programming languages the = command works fine: you select the lines
 * with V, and press =. But for other languages, the = command doesn't always
 * work well (for example it removes the indentation). I personally use the =
 * command only to align parameters on the parenthesis, so the purpose of
 * gcu-align-params-on-parenthesis is to provide that functionality in Vim for
 * languages where = is broken (gcu-align-params-on-parenthesis does only one
 * thing, but (hopefully) does it well).
 *
 * Add to your vimrc:
 *
 * function! AlignParamsOnParenthesis()
 *         execute "normal :'<,'>!gcu-align-params-on-parenthesis\<CR>"
 * endfunction
 *
 * function! MapAlignParamsOnParenthesis()
 *         map = :call AlignParamsOnParenthesis()<CR>
 * endfunction
 *
 * " For example for the Meson build system:
 * autocmd Filetype meson call MapAlignParamsOnParenthesis()
 */

#include <gio/gio.h>
#include <gio/gunixinputstream.h>
#include <stdlib.h>
#include <locale.h>

/* Returns on which column the text of the following lines must be placed. */
static gint
get_column_num (const gchar *first_line)
{
  const gchar *last_opening_paren_pos;
  gssize bytes_length;

  g_assert (first_line != NULL);

  /* This can be improved, to detect closing parentheses etc. */
  last_opening_paren_pos = strrchr (first_line, '(');
  if (last_opening_paren_pos == NULL)
    return -1;

  bytes_length = last_opening_paren_pos + 1 - first_line;
  return g_utf8_strlen (first_line, bytes_length);
}

static gchar *
get_indentation (gint column_num)
{
  /* This can be improved, to support tabs. */
  return g_strnfill (column_num, ' ');
}

static void
print_following_line (const gchar *line,
                      const gchar *indentation)
{
  const gchar *line_text;

  g_assert (line != NULL);
  g_assert (indentation != NULL);

  /* Last line. */
  if (line[0] == '\0')
    return;

  /* Skip spaces. */
  line_text = line;
  while (*line_text != '\0' && g_ascii_isspace (*line_text))
    line_text++;

  g_print ("%s%s\n", indentation, line_text);
}

static void
align_params_on_parenthesis (const gchar *input_str)
{
  gchar **lines;
  gint column_num;
  gchar *indentation = NULL;
  gint i;

  lines = g_strsplit (input_str, "\n", -1);
  if (lines == NULL || lines[0] == NULL)
    goto out;

  column_num = get_column_num (lines[0]);
  if (column_num == -1)
    {
      /* Opening parenthesis not founnd, print the input unmodified. */
      g_print ("%s", input_str);
      goto out;
    }

  indentation = get_indentation (column_num);

  g_print ("%s\n", lines[0]);

  for (i = 1; lines[i] != NULL; i++)
    print_following_line (lines[i], indentation);

out:
  g_strfreev (lines);
  g_free (indentation);
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

int
main (void)
{
  gchar *input_str;

  setlocale (LC_ALL, "");

  input_str = get_stdin_contents ();
  align_params_on_parenthesis (input_str);

  return EXIT_SUCCESS;
}
