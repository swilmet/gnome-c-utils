#!/usr/bin/env bash

# configuration

namespace_camel=Tepl
#namespace_camel=Gspell
#namespace_camel=GtkSource
#namespace_camel=Gedit

interfacename_camel=TabList

filename=tepl-tab-list

# generate the new interface

template_filename=interface-old-style

./generate-interface-common.sh	\
	${namespace_camel}	\
	${interfacename_camel}	\
	${filename}		\
	${template_filename}
