programs_depending_on_gio = [
        # executable name, sources
        ['gcu-align-params-on-parenthesis', ['gcu-align-params-on-parenthesis.c']],
        ['gcu-case-converter', ['gcu-case-converter.c']],
        ['gcu-lineup-parameters', ['gcu-lineup-parameters.c']]
]

programs_depending_on_tepl = [
        # executable name, sources
        ['gcu-check-chain-ups', ['gcu-check-chain-ups.c']],
        ['gcu-include-config-h', ['gcu-include-config-h.c']],
        ['gcu-lineup-substitution', ['gcu-lineup-substitution.c']],
        ['gcu-multi-line-substitution', ['gcu-multi-line-substitution.c']],
        ['gcu-smart-c-comment-substitution', ['gcu-smart-c-comment-substitution.c']],
]

foreach prog : programs_depending_on_gio
        executable(prog[0],
                   prog[1],
                   dependencies : GIO_DEPS,
                   install : true)
endforeach

if ALL_TEPL_DEPS_FOUND
        foreach prog : programs_depending_on_tepl
                executable(prog[0],
                           prog[1],
                           dependencies : [GIO_DEPS, TEPL_DEPS],
                           install : true)
        endforeach
endif
