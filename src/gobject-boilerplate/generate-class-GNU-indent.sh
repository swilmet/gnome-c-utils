#!/usr/bin/env bash

# configuration

namespace_camel=Latexila
classname_camel=PostProcessor

filename=latexila-post-processor

# generate the new class

template_filename=class-GNU-indent

./generate-class-common.sh \
	${namespace_camel} \
	${classname_camel} \
	${filename} \
	${template_filename}
