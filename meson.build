project('gnome-c-utils', 'c',
	meson_version : '>= 0.43')

gio_deps = [
	dependency('glib-2.0', version : '>= 2.50'),
	dependency('gio-2.0'),
	dependency('gio-unix-2.0')
]

# The Tepl dependencies are optional, since it can be difficult to install all
# of them.
tepl_deps = [
	dependency('gtk+-3.0',		version : '>= 3.22',	required : false),
	dependency('gtksourceview-4',	version : '>= 3.99.6',	required : false),
	dependency('tepl-4',		version : '>= 3.99.0',	required : false)
]

# If you know a better way to compute all_tepl_deps_found, contribution
# welcome.
tepl_deps_found = []
foreach tepl_dep : tepl_deps
	tepl_deps_found += tepl_dep.found()
endforeach

all_tepl_deps_found = not (tepl_deps_found.contains(false))

add_global_arguments('-DG_LOG_DOMAIN="@0@"'.format(meson.project_name()),
		     language : 'c')

#####
# CFLAGS
# (This is the ugly part of Meson, currently.)
# The same as the AX_COMPILER_FLAGS Autotools macro.

warning_cflags = [
	'-fno-strict-aliasing',
	'-Wall',
	'-Wextra',
	'-Wundef',
	'-Wnested-externs',
	'-Wwrite-strings',
	'-Wpointer-arith',
	'-Wmissing-declarations',
	'-Wmissing-prototypes',
	'-Wstrict-prototypes',
	'-Wredundant-decls',
	'-Wno-unused-parameter',
	'-Wno-missing-field-initializers',
	'-Wdeclaration-after-statement',
	'-Wformat=2',
	'-Wold-style-definition',
	'-Wcast-align',
	'-Wformat-nonliteral',
	'-Wformat-security',
	'-Wsign-compare',
	'-Wstrict-aliasing',
	'-Wshadow',
	'-Winline',
	'-Wpacked',
	'-Wmissing-format-attribute',
	'-Wmissing-noreturn',
	'-Winit-self',
	'-Wredundant-decls',
	'-Wmissing-include-dirs',
	'-Wunused-but-set-variable',
	'-Warray-bounds',
	'-Wimplicit-function-declaration',
	'-Wreturn-type',
	'-Wswitch-enum',
	'-Wswitch-default',
	'-Wduplicated-cond',
	'-Wduplicated-branches',
	'-Wlogical-op',
	'-Wrestrict',
	'-Wnull-dereference',
	'-Wjump-misses-init',
	'-Wdouble-promotion'
]

c_compiler = meson.get_compiler('c')
supported_warning_cflags = c_compiler.get_supported_arguments(warning_cflags)
add_global_arguments(supported_warning_cflags, language : 'c')
##### end CFLAGS

subdir('src')

# Print a summary of the configuration
output = 'Configuration:\n'
output += '    Build programs depending on GIO:  true\n'
output += '    Build programs depending on Tepl: @0@'.format(all_tepl_deps_found)
message(output)
