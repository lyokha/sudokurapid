#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl AC_PREREQ([2.63])
AC_INIT([sudokuRapidQtXdg], [1.0])
AM_INIT_AUTOMAKE

AC_CHECK_PROG(XDG_DESKTOP_MENU, [xdg-desktop-menu], [xdg-desktop-menu], [])
if test -z $XDG_DESKTOP_MENU; then
    AC_MSG_ERROR([xdg-desktop-menu not found. Please install xdg-utils.])
fi
AC_CHECK_PROG(XDG_ICON_RESOURCE, [xdg-icon-resource], [xdg-icon-resource], [])
if test -z $XDG_ICON_RESOURCE; then
    AC_MSG_ERROR([xdg-icon-resource not found. Please install xdg-utils.])
fi

# Checks for programs.

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

