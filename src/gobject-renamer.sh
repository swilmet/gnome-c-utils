#!/usr/bin/env bash

old_classname_camel=MenuFactory
old_classname_upper=$(gcu-case-converter --to-uppercase "$old_classname_camel")
old_classname_lower=$(gcu-case-converter --to-lowercase "$old_classname_camel")

new_classname_camel=Factory
new_classname_upper=$(gcu-case-converter --to-uppercase "$new_classname_camel")
new_classname_lower=$(gcu-case-converter --to-lowercase "$new_classname_camel")

filename=amtk-factory

ls $filename.[ch] | parallel --will-cite gcu-lineup-substitution "$old_classname_upper" "$new_classname_upper"
ls $filename.[ch] | parallel --will-cite gcu-lineup-substitution "$old_classname_camel" "$new_classname_camel"
ls $filename.[ch] | parallel --will-cite gcu-lineup-substitution "$old_classname_lower" "$new_classname_lower"
