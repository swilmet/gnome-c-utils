#!/usr/bin/env bash

# TODO
# - Take also the namespace, and substitute:
# "${old_namespace_upper}_IS_${old_classname_upper}"
# "${old_namespace_upper}_TYPE_${old_classname_upper}"
#
# - Call gcu-lineup-parameters on the file if it is a *.c file.
#
# - Take input arguments from the command line, check the args, print usage,
#   rename the script to gcu-gobject-renamer and install it.

filename=$1

old_classname_camel=Foo
old_classname_upper=$(gcu-case-converter --to-uppercase "$old_classname_camel")
old_classname_lower=$(gcu-case-converter --to-lowercase "$old_classname_camel")

new_classname_camel=Bar
new_classname_upper=$(gcu-case-converter --to-uppercase "$new_classname_camel")
new_classname_lower=$(gcu-case-converter --to-lowercase "$new_classname_camel")

gcu-lineup-substitution "$old_classname_upper" "$new_classname_upper" "$filename"
gcu-lineup-substitution "$old_classname_camel" "$new_classname_camel" "$filename"
gcu-lineup-substitution "$old_classname_lower" "$new_classname_lower" "$filename"
