#!/usr/bin/env bash

old_classname_camel=TabList
old_classname_upper=$(gcu-case-converter --to-uppercase "$old_classname_camel")
old_classname_lower=$(gcu-case-converter --to-lowercase "$old_classname_camel")

new_classname_camel=TabGroup
new_classname_upper=$(gcu-case-converter --to-uppercase "$new_classname_camel")
new_classname_lower=$(gcu-case-converter --to-lowercase "$new_classname_camel")

ls *.[ch] | parallel --will-cite gcu-lineup-substitution "$old_classname_upper" "$new_classname_upper"
ls *.[ch] | parallel --will-cite gcu-lineup-substitution "$old_classname_camel" "$new_classname_camel"
ls *.[ch] | parallel --will-cite gcu-lineup-substitution "$old_classname_lower" "$new_classname_lower"
