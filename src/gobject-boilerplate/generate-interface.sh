#!/usr/bin/env bash

# configuration

namespace_uppercase=GTK_SOURCE
namespace_camel=GtkSource
namespace_lowercase=_gtk_source

#namespace_uppercase=GEDIT
#namespace_camel=Gedit
#namespace_lowercase=gedit

interfacename_uppercase=SEARCH_SCAN
interfacename_camel=SearchScan
interfacename_lowercase=search_scan

filename=gtksourcesearchscan

# generate the new interface

cp interface.c ${filename}.c
cp interface.h ${filename}.h

sed -i "s/NAMESPACE/$namespace_uppercase/g" ${filename}.*
sed -i "s/Namespace/$namespace_camel/g" ${filename}.*
sed -i "s/namespace/$namespace_lowercase/g" ${filename}.*

sed -i "s/INTERFACENAME/$interfacename_uppercase/g" ${filename}.*
sed -i "s/Interfacename/$interfacename_camel/g" ${filename}.*
sed -i "s/interfacename/$interfacename_lowercase/g" ${filename}.*

sed -i "s/filename/$filename/g" ${filename}.*

echo "${filename}.c and ${filename}.h generated."
