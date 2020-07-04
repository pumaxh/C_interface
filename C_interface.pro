TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    atom/src/atom.c \
    exception/src/exception.c \
    main.c


HEADERS += \
    atom/include/atom.h \
    exception/include/exception.h

INCLUDEPATH += $$PWD/atom/include
INCLUDEPATH += $$PWD/exception/include
