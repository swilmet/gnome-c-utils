#!/usr/bin/env bash

# configuration

#namespace_camel=GtkSource
#namespace_camel=Gedit
namespace_camel=Tepl

classname_camel=File

filename=tepl-file

# generate the new class

template_filename=class

./generate-class-common.sh \
	${namespace_camel} \
	${classname_camel} \
	${filename} \
	${template_filename}
