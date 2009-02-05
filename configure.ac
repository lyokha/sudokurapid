#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl AC_PREREQ([2.63])
AC_INIT([sudokuRapid], [1.0])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([sudokuRapid.h])
AC_CONFIG_HEADERS([config.h])


AC_ARG_ENABLE([sudokuRapidConsole],
[AC_HELP_STRING([--disable-sudokuRapidConsole],
    [disable building of sudokuRapidConsole])],
[disable_sudokuRapidConsole=yes])

AM_CONDITIONAL([BUILD_CONSOLE], [test "x${disable_sudokuRapidConsole}" != xyes])

if test "x${disable_sudokuRapidConsole}" != xyes; then
    AC_CONFIG_SUBDIRS( [sudokuRapidConsole])
fi

AC_ARG_ENABLE([sudokuRapidQt],
[AC_HELP_STRING([--disable-sudokuRapidQt],
    [disable building of sudokuRapidQt])],
[disable_sudokuRapidQt=yes])

AM_CONDITIONAL([BUILD_QT], [test "x${disable_sudokuRapidQt}" != xyes])

if test "x${disable_sudokuRapidQt}" != xyes; then
    AC_CHECK_PROG(QMAKE, [qmake], [$QTDIR/bin/qmake], [],[$QTDIR/bin])
    if test -z $QMAKE; then
        AC_MSG_ERROR([qmake not found. Try to setup QTDIR variable.])
    fi
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
