#!/usr/bin/env bash

# configuration

namespace_uppercase=G
namespace_camel=G
namespace_lowercase=g

interfacename_uppercase=SPELL_CHECKER
interfacename_camel=SpellChecker
interfacename_lowercase=spell_checker

filename=gspellchecker

# generate the new interface

cp interface-GNU-indent.c ${filename}.c
cp interface-GNU-indent.h ${filename}.h

sed -i "s/NAMESPACE/$namespace_uppercase/g" ${filename}.*
sed -i "s/Namespace/$namespace_camel/g" ${filename}.*
sed -i "s/namespace/$namespace_lowercase/g" ${filename}.*

sed -i "s/INTERFACENAME/$interfacename_uppercase/g" ${filename}.*
sed -i "s/Interfacename/$interfacename_camel/g" ${filename}.*
sed -i "s/interfacename/$interfacename_lowercase/g" ${filename}.*

sed -i "s/filename/$filename/g" ${filename}.*

echo "${filename}.c and ${filename}.h generated."
