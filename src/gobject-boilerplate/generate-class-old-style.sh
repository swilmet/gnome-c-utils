#!/usr/bin/env bash

# configuration

#namespace_camel=GtkSource
#namespace_camel=Gedit
#namespace_camel=Amtk
namespace_camel=Tepl
#namespace_camel=Gcsv
#namespace_camel=Gnote
#namespace_camel=Latexila

classname_camel=AbstractFactory

filename=tepl-abstract-factory

# generate the new class

template_filename=class-old-style

./generate-class-common.sh \
	${namespace_camel} \
	${classname_camel} \
	${filename} \
	${template_filename}
