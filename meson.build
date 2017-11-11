project('gnome-c-utils', 'c',
	meson_version : '>= 0.43')

gio_deps = [
	dependency('glib-2.0', version : '>= 2.50'),
	dependency('gio-2.0'),
	dependency('gio-unix-2.0')
]

tepl_deps = [
	dependency('gtk+-3.0', version : '>= 3.22'),
	dependency('gtksourceview-4', version : '>= 3.99.6'),
	dependency('tepl-4', version : '>= 3.99.0')
]

add_global_arguments('-DG_LOG_DOMAIN="@0@"'.format(meson.project_name()),
		     language : 'c')

#####
# CFLAGS
# (This is the ugly part of Meson, currently.)
# The same as the AX_COMPILER_FLAGS Autotools macro.

warning_cflags = [
	'-fno-strict-aliasing',
	'-Wcast-align',
	'-Wdeclaration-after-statement',
	'-Wformat-nonliteral',
	'-Wformat=2',
	'-Winit-self',
	'-Winline',
	'-Wmissing-declarations',
	'-Wmissing-format-attribute',
	'-Wmissing-include-dirs',
	'-Wmissing-noreturn',
	'-Wmissing-prototypes',
	'-Wnested-externs',
	'-Wno-error=missing-field-initializers',
	'-Wno-error=unused-parameter',
	'-Wno-missing-field-initializers',
	'-Wno-unused-parameter',
	'-Wold-style-definition',
	'-Wpacked',
	'-Wpointer-arith',
	'-Wredundant-decls',
	'-Wshadow',
	'-Wstrict-prototypes',
	'-Wswitch-enum',
	'-Wundef',
	'-Wunused-but-set-variable',
	'-Wwrite-strings'
]

c_compiler = meson.get_compiler('c')
supported_warning_cflags = c_compiler.get_supported_arguments(warning_cflags)
add_global_arguments(supported_warning_cflags, language : 'c')
##### end CFLAGS

subdir('src')
