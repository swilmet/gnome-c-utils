#!/usr/bin/env bash

# configuration

namespace_camel=Dh
classname_camel=WebView

filename=dh-web-view

# generate the new class

template_filename=class-devhelp

./generate-class-common.sh \
	${namespace_camel} \
	${classname_camel} \
	${filename} \
	${template_filename}
