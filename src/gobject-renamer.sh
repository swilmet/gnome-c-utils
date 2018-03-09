#!/usr/bin/env bash

# TODO
# - Call gcu-lineup-parameters on the file if it is a *.c file.
#
# - Take input arguments from the command line, check the args, print usage,
#   rename the script to gcu-gobject-renamer and install it.

filename=$1

old_namespace_camel=Dh
old_namespace_upper=$(gcu-case-converter --to-uppercase "$old_namespace_camel")
old_namespace_lower=$(gcu-case-converter --to-lowercase "$old_namespace_camel")

new_namespace_camel=Dh
new_namespace_upper=$(gcu-case-converter --to-uppercase "$new_namespace_camel")
new_namespace_lower=$(gcu-case-converter --to-lowercase "$new_namespace_camel")

old_classname_camel=Settings
old_classname_upper=$(gcu-case-converter --to-uppercase "$old_classname_camel")
old_classname_lower=$(gcu-case-converter --to-lowercase "$old_classname_camel")

new_classname_camel=SettingsApp
new_classname_upper=$(gcu-case-converter --to-uppercase "$new_classname_camel")
new_classname_lower=$(gcu-case-converter --to-lowercase "$new_classname_camel")

# UPPER
gcu-lineup-substitution "${old_namespace_upper}_${old_classname_upper}" \
			"${new_namespace_upper}_${new_classname_upper}" \
			"$filename"

gcu-lineup-substitution "${old_namespace_upper}_IS_${old_classname_upper}" \
			"${new_namespace_upper}_IS_${new_classname_upper}" \
			"$filename"

gcu-lineup-substitution "${old_namespace_upper}_TYPE_${old_classname_upper}" \
			"${new_namespace_upper}_TYPE_${new_classname_upper}" \
			"$filename"

# Camel
gcu-lineup-substitution "${old_namespace_camel}${old_classname_camel}" \
			"${new_namespace_camel}${new_classname_camel}" \
			"$filename"

# lower
gcu-lineup-substitution "${old_namespace_lower}_${old_classname_lower}" \
			"${new_namespace_lower}_${new_classname_lower}" \
			"$filename"
