gnome-c-utils
=============

Various utilities, useful when programming with
[GLib/GTK+](https://www.gtk.org/) in C.

Some scripts are more general and can be useful for other tasks.

Dependencies
------------

- GLib
- GTK+
- GtkSourceView
- [Tepl](https://wiki.gnome.org/Projects/Tepl)

Installation
------------

```
$ cd build/
$ meson
$ ninja
[ Become root if necessary ]
$ ninja install
```

Tests/sample files
------------------

There are sample files in the `tests/` directory.

Running the scripts on several files at once
--------------------------------------------

It is recommended to use e.g.
[GNU Parallel](https://www.gnu.org/software/parallel/) to run the scripts on
several files at once. In a previous version of gnome-c-utils it was possible
to directly pass several file arguments to some gcu scripts, but this
possibility has been removed to simplify the code, and with GNU Parallel it is
anyway almost as convenient to use, with the benefit that it runs in parallel.

For example:
```
$ find . -name "*.c" | parallel gcu-lineup-parameters
```

gcu-lineup-parameters
---------------------

Line up parameters of function declarations.

Example:

```
gboolean
frobnitz (Frobnitz *frobnitz,
          gint magic_number,
          GError **error)
{
  ...
}
```

Becomes:

```
gboolean
frobnitz (Frobnitz  *frobnitz,
          gint       magic_number,
          GError   **error)
{
  ...
}
```

Read the top of `gcu-lineup-parameters.c` for more details.

gcu-lineup-substitution
-----------------------

Do a substitution and at the same time keep a good alignment of parameters on
the parenthesis.

Example:

```
function_call (param1,
               param2,
               param3);
```

Rename `function_call` to `another_beautiful_name`:

```
another_beautiful_name (param1,
                        param2,
                        param3);
```

gcu-lineup-substitution can be useful to rename a GObject class, or to change
the namespace of a group of GObjects, while still keeping a good
indentation/alignment of the code (in combination with gcu-lineup-parameters).

Read the top of `gcu-lineup-substitution.c` for more details.

gcu-case-converter
------------------

Converts a word to `lower_case`, `UPPER_CASE` or `CamelCase`.

Read the top of `gcu-case-converter.c` for more details.

gcu-multi-line-substitution
---------------------------

Does a multi-line substitution. Or, in other words, a multi-line search and
replace.

Read the top of `gcu-multi-line-substitution.c` for more details.

gcu-smart-c-comment-substitution
--------------------------------

Smart substitution (or, search and replace) in C comments. Can be useful to
change license headers. The script ignores spacing differences and ignores the
positions of newlines (where a sentence is split).

Read the top of `gcu-smart-c-comment-substitution.c` for more details.

gcu-check-chain-ups
-------------------

Basic check of GObject virtual function chain-ups.

Read the top of `gcu-check-chain-ups.c` for more details.

gcu-include-config-h
--------------------

Ensures that `config.h` is `#included` in `*.c` files.

Read the top of `gcu-include-config-h.c` for more details.
