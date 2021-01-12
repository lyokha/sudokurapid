#!/bin/sh

qt_xdg_dir=sudokuRapidQt/xdg

for dir in . sudokuRapidConsole $qt_xdg_dir ; do
    echo Doing all in ${dir} ...
    (cd $dir
    echo '  'Creating files needed by automake ...
    touch NEWS README AUTHORS ChangeLog
    echo '  'Running aclocal ...
    aclocal
    echo '  'Generating configure ...
    autoconf
    if [ $dir != $qt_xdg_dir ] ; then
        echo '  'Generating config.h.in ...
        autoheader
    fi
    echo '  'Generating Makefile.in ...
    automake -a)
done

