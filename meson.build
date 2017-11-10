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

subdir('src')
