#!/usr/bin/env bash

# configuration

namespace_camel=$1
namespace_uppercase=$(gcu-case-converter --to-uppercase "$namespace_camel")
namespace_lowercase=$(gcu-case-converter --to-lowercase "$namespace_camel")

classname_camel=$2
classname_uppercase=$(gcu-case-converter --to-uppercase "$classname_camel")
classname_lowercase=$(gcu-case-converter --to-lowercase "$classname_camel")

filename=$3
template_filename=$4

# generate the new class

cp ${template_filename}.c ${filename}.c
cp ${template_filename}.h ${filename}.h

ls ${filename}.[ch] | parallel --will-cite gcu-lineup-substitution "NAMESPACE" "$namespace_uppercase"
ls ${filename}.[ch] | parallel --will-cite gcu-lineup-substitution "Namespace" "$namespace_camel"
ls ${filename}.[ch] | parallel --will-cite gcu-lineup-substitution "namespace" "$namespace_lowercase"

ls ${filename}.[ch] | parallel --will-cite gcu-lineup-substitution "CLASSNAME" "$classname_uppercase"
ls ${filename}.[ch] | parallel --will-cite gcu-lineup-substitution "Classname" "$classname_camel"
ls ${filename}.[ch] | parallel --will-cite gcu-lineup-substitution "classname" "$classname_lowercase"

ls ${filename}.[ch] | parallel --will-cite gcu-lineup-substitution "filename" "$filename"

gcu-lineup-parameters ${filename}.c

echo "${filename}.c and ${filename}.h generated."
