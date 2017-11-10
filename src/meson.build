programs_depending_on_gio = [
	# executable name, sources
	['gcu-case-converter', ['gcu-case-converter.c']],
	['gcu-lineup-parameters', ['gcu-lineup-parameters.c']]
]

foreach prog : programs_depending_on_gio
	executable(prog[0],
		   prog[1],
		   dependencies : gio_deps,
		   install : true)
endforeach
