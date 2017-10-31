dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.64)

AC_INIT([gnome-c-utils],
	[0.1],
	[https://github.com/swilmet/gnome-c-utils/issues],
	[gnome-c-utils],
	[https://github.com/swilmet/gnome-c-utils])

AX_REQUIRE_DEFINED([AX_IS_RELEASE])
AX_IS_RELEASE([git-directory])

AC_CONFIG_SRCDIR([src/gcu-lineup-parameters.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 tar-ustar dist-xz no-dist-gzip foreign -Wno-portability])
AM_SILENT_RULES([yes])

# Check for programs
AC_PROG_CC
AC_PROG_INSTALL

# Compile warnings. There can be deprecation warnings at any time, so disable
# -Werror by default with the [yes] argument.
AX_REQUIRE_DEFINED([AX_COMPILER_FLAGS])
AX_COMPILER_FLAGS([WARN_CFLAGS], [WARN_LDFLAGS], [yes])

# Dependencies
AX_REQUIRE_DEFINED([PKG_CHECK_MODULES])
PKG_CHECK_MODULES([GIO_DEPS],
		  [glib-2.0 >= 2.50
		   gio-2.0
		   gio-unix-2.0])

PKG_CHECK_MODULES([TEPL_DEPS],
		  [gtk+-3.0 >= 3.22
		   gtksourceview-4 >= 3.99.6
		   tepl-4 >= 3.99.0])

# Output files
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	tests/Makefile
])

AC_OUTPUT

echo "
Configuration:

	${PACKAGE_NAME} version ${PACKAGE_VERSION}

	Source code location:	${srcdir}
	Prefix:			${prefix}
	Compiler:		${CC}
"
