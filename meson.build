# Convention:
# - Local variables in lower_case.
# - Global variables in UPPER_CASE.
# See: https://github.com/mesonbuild/meson/issues/2607

project(
  'gnome-c-utils', 'c',
  meson_version : '>= 0.43',
  default_options : ['warning_level=2']
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
  dependency('tepl-5',          version : '>= 5.0',   required : false)
]

ALL_TEPL_DEPS_FOUND = true
foreach tepl_dep : TEPL_DEPS
  if not tepl_dep.found()
    ALL_TEPL_DEPS_FOUND = false
  endif
endforeach

add_project_arguments(
  '-DG_LOG_DOMAIN="@0@"'.format(meson.project_name()),
  language : 'c'
)

#####
# CFLAGS
# Try to mimic the AX_COMPILER_FLAGS Autotools macro.
# Some flags are missing when using only the builtin warning_level meson option,
# even at the maximum level.
# The following warning_cflags suppose that warning_level=2.

warning_cflags = [
  '-fno-strict-aliasing',
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
add_project_arguments(supported_warning_cflags, language : 'c')
##### end CFLAGS

subdir('src')

# Print a summary of the configuration
output = 'Configuration:\n'
output += '    Build programs depending on GIO:  true\n'
output += '    Build programs depending on Tepl: @0@'.format(ALL_TEPL_DEPS_FOUND)
message(output)
