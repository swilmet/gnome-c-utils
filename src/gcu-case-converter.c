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
 * $ gcu-case-converter (--to-uppercase|-u|--to-camelcase|-c|--to-lowercase|-l) word
 *
 * 'word' can initially be in UPPER_CASE, lower_case or CamelCase, it is
 * automatically detected.
 * Only one option must be provided.
 * The converted word is printed on stdout.
 * There can be warnings printed on stderr.
 */

#include <stdlib.h>
#include <locale.h>
#include <glib.h>

typedef enum
{
  GCU_CASE_TO_UPPERCASE,
  GCU_CASE_TO_CAMELCASE,
  GCU_CASE_TO_LOWERCASE,
} GcuCase;

static gboolean to_uppercase;
static gboolean to_camelcase;
static gboolean to_lowercase;

static GOptionEntry option_entries[] =
{
  { "to-uppercase", 'u', 0, G_OPTION_ARG_NONE, &to_uppercase, "To UPPER_CASE", NULL },
  { "to-camelcase", 'c', 0, G_OPTION_ARG_NONE, &to_camelcase, "To CamelCase", NULL },
  { "to-lowercase", 'l', 0, G_OPTION_ARG_NONE, &to_lowercase, "To lower_case", NULL },
  { NULL }
};

static void
print_usage (char **argv)
{
  g_printerr ("Usage: %s (--to-uppercase|-u|--to-camelcase|-c|--to-lowercase|-l) word\n",
              argv[0]);
}

static GcuCase
get_case (char **argv)
{
  gboolean found = FALSE;
  GcuCase to_case = GCU_CASE_TO_UPPERCASE;

  if (to_uppercase)
    {
      to_case = GCU_CASE_TO_UPPERCASE;
      found = TRUE;
    }

  if (to_camelcase)
    {
      if (found)
        {
          g_printerr ("Only one option can be provided.\n");
          print_usage (argv);
          exit (EXIT_FAILURE);
        }

      to_case = GCU_CASE_TO_CAMELCASE;
      found = TRUE;
    }

  if (to_lowercase)
    {
      if (found)
        {
          g_printerr ("Only one option can be provided.\n");
          print_usage (argv);
          exit (EXIT_FAILURE);
        }

      to_case = GCU_CASE_TO_LOWERCASE;
      found = TRUE;
    }

  if (!found)
    {
      g_printerr ("An option must be provided.\n");
      print_usage (argv);
      exit (EXIT_FAILURE);
    }

  return to_case;
}

static gboolean
starts_subword (gchar prev_char,
                gchar cur_char)
{
  /* cur_char is the first char */
  if (prev_char == '\0' && g_ascii_isalnum (cur_char))
    return TRUE;

  if (prev_char == '_' && g_ascii_isalnum (cur_char))
    return TRUE;

  if (g_ascii_islower (prev_char) && g_ascii_isupper (cur_char))
    return TRUE;

  return FALSE;
}

/* Returns: the converted word. Free with g_free(). */
static gchar *
convert_word (const gchar *word,
              GcuCase      to_case)
{
  GString *converted_word;
  gint pos;
  gboolean warning_printed = FALSE;

  g_assert (word != NULL);

  converted_word = g_string_new (NULL);

  for (pos = 0; word[pos] != '\0'; pos++)
    {
      gchar prev_char = '\0';
      gchar cur_char = word[pos];

      if (pos > 0)
        prev_char = word[pos-1];

      if (prev_char == '_' && cur_char == '_' && !warning_printed)
        {
          g_printerr ("Two contiguous underscores are not well supported, check the result.\n");
          warning_printed = TRUE;
        }

      if (cur_char == '_')
        continue;

      if (starts_subword (prev_char, cur_char))
        {
          switch (to_case)
            {
            case GCU_CASE_TO_UPPERCASE:
              if (pos > 0)
                g_string_append_c (converted_word, '_');
              g_string_append_c (converted_word, g_ascii_toupper (cur_char));
              break;

            case GCU_CASE_TO_CAMELCASE:
              g_string_append_c (converted_word, g_ascii_toupper (cur_char));
              break;

            case GCU_CASE_TO_LOWERCASE:
              if (pos > 0)
                g_string_append_c (converted_word, '_');
              g_string_append_c (converted_word, g_ascii_tolower (cur_char));
              break;

            default:
              g_assert_not_reached ();
            }
        }
      else
        {
          switch (to_case)
            {
            case GCU_CASE_TO_UPPERCASE:
              g_string_append_c (converted_word, g_ascii_toupper (cur_char));
              break;

            case GCU_CASE_TO_CAMELCASE:
              g_string_append_c (converted_word, g_ascii_tolower (cur_char));
              break;

            case GCU_CASE_TO_LOWERCASE:
              g_string_append_c (converted_word, g_ascii_tolower (cur_char));
              break;

            default:
              g_assert_not_reached ();
            }
        }
    }

  return g_string_free (converted_word, FALSE);
}

int
main (int    argc,
      char **argv)
{
  GOptionContext *option_context;
  GError *error = NULL;
  GcuCase to_case;
  const gchar *word;
  gchar *converted_word;
  int ret = EXIT_SUCCESS;

  setlocale (LC_ALL, "");

  option_context = g_option_context_new ("- case converter");
  g_option_context_add_main_entries (option_context, option_entries, NULL);
  if (!g_option_context_parse (option_context, &argc, &argv, &error))
    {
      g_printerr ("Option parsing failed: %s\n", error->message);
      print_usage (argv);
      ret = EXIT_FAILURE;
      goto exit;
    }

  if (argc != 2)
    {
      print_usage (argv);
      ret = EXIT_FAILURE;
      goto exit;
    }

  to_case = get_case (argv);
  word = argv[1];
  converted_word = convert_word (word, to_case);
  g_print ("%s\n", converted_word);
  g_free (converted_word);

exit:
  g_option_context_free (option_context);
  g_clear_error (&error);
  return ret;
}
