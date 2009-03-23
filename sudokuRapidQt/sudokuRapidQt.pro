TEMPLATE = app
TARGET =
DEPENDPATH += . .
INCLUDEPATH += . ..
isEmpty(PREFIX) {
    PREFIX = /usr
}
target.path = $${PREFIX}/bin
INSTALLS = target

# Input
HEADERS += sudokuForm.h sudokuScene.h sudokuCell.h
FORMS += sudokuForm.ui
SOURCES += main.cc sudokuForm.cc sudokuScene.cc sudokuCell.cc
LIBS += -L.. -lsudokuRapid
