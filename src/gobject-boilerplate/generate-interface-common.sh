#!/usr/bin/env bash

# configuration

namespace_camel=$1
namespace_uppercase=$(gcu-case-converter --to-uppercase "$namespace_camel")
namespace_lowercase=$(gcu-case-converter --to-lowercase "$namespace_camel")

interfacename_camel=$2
interfacename_uppercase=$(gcu-case-converter --to-uppercase "$interfacename_camel")
interfacename_lowercase=$(gcu-case-converter --to-lowercase "$interfacename_camel")

filename=$3
template_filename=$4

# generate the new interface

cp ${template_filename}.c ${filename}.c
cp ${template_filename}.h ${filename}.h

sed -i "s/NAMESPACE/$namespace_uppercase/g" ${filename}.*
sed -i "s/Namespace/$namespace_camel/g" ${filename}.*
sed -i "s/namespace/$namespace_lowercase/g" ${filename}.*

sed -i "s/INTERFACENAME/$interfacename_uppercase/g" ${filename}.*
sed -i "s/Interfacename/$interfacename_camel/g" ${filename}.*
sed -i "s/interfacename/$interfacename_lowercase/g" ${filename}.*

sed -i "s/filename/$filename/g" ${filename}.*

echo "${filename}.c and ${filename}.h generated."
