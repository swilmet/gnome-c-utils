# Convention:
# - Local variables in lower_case.
# - Global variables in UPPER_CASE.
# See: https://github.com/mesonbuild/meson/issues/2607

project(
  'gnome-c-utils', 'c',
  meson_version : '>= 0.43',
  default_options : ['warning_level=3']
)

GIO_DEPS = [
  dependency('glib-2.0', version : '>= 2.50'),
  dependency('gio-2.0'),
  dependency('gio-unix-2.0')
]

# The Tepl dependencies are optional, since it can be difficult to install all
# of them.
TEPL_DEPS = [
  dependency('gtk+-3.0',        version : '>= 3.22',  required : false),
  dependency('gtksourceview-4', version : '>= 4.0',   required : false),
  dependency('tepl-4',          version : '>= 4.0',   required : false)
]

ALL_TEPL_DEPS_FOUND = true
foreach tepl_dep : TEPL_DEPS
  if not tepl_dep.found()
    ALL_TEPL_DEPS_FOUND = false
  endif
endforeach

add_global_arguments(
  '-DG_LOG_DOMAIN="@0@"'.format(meson.project_name()),
  language : 'c'
)

subdir('src')

# Print a summary of the configuration
output = 'Configuration:\n'
output += '    Build programs depending on GIO:  true\n'
output += '    Build programs depending on Tepl: @0@'.format(ALL_TEPL_DEPS_FOUND)
message(output)
