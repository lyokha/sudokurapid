if BUILD_CONSOLE
DIR_CONSOLE_APP = sudokuRapidConsole
endif

if BUILD_QT
DIR_QT_APP = sudokuRapidQt
endif

lib_LIBRARIES = libsudokuRapid.a
include_HEADERS = sudokuRapid.h sudokuRapidCommon.h
libsudokuRapid_a_SOURCES = sudokuRapid.cc sudokuRapid.h sudokuRapidCommon.h
SUBDIRS = . $(DIR_CONSOLE_APP) $(DIR_QT_APP)

DIR_QT_DIST = sudokuRapidQt
EXTRA_DIST = $(DIR_QT_DIST)/main.cc \
             $(DIR_QT_DIST)/sudokuForm.cc $(DIR_QT_DIST)/sudokuForm.h \
             $(DIR_QT_DIST)/sudokuScene.cc $(DIR_QT_DIST)/sudokuScene.h \
             $(DIR_QT_DIST)/sudokuCell.cc $(DIR_QT_DIST)/sudokuCell.h \
             $(DIR_QT_DIST)/sudokuRapidQt.pro $(DIR_QT_DIST)/sudokuForm.ui \
             $(DIR_QT_DIST)/sudokuRapidQt.desktop $(DIR_QT_DIST)/sudokuRapidQt-48x48.png
DIST_SUBDIRS = . sudokuRapidConsole
dist_pkgdata_DATA = examples/board1.txt examples/board2.txt sudokuRapidConsole/main.cc \
                    $(DIR_QT_DIST)/sudokuRapidQt.desktop $(DIR_QT_DIST)/sudokuRapidQt-48x48.png

if BUILD_QT
all: sudokuRapidQt/Makefile
sudokuRapidQt/Makefile:
	cd sudokuRapidQt; $(QMAKE) "PREFIX=${prefix}" sudokuRapidQt.pro

install-data-hook:
	cd sudokuRapidQt; $(XDG_DESKTOP_MENU) install --novendor ./sudokuRapidQt.desktop
	cd sudokuRapidQt; $(XDG_ICON_RESOURCE) install --novendor --size 48 ./sudokuRapidQt-48x48.png sudokuRapidQt
uninstall-hook:
	$(XDG_ICON_RESOURCE) uninstall --size 48 sudokuRapidQt
	$(XDG_DESKTOP_MENU) uninstall sudokuRapidQt.desktop
endif
