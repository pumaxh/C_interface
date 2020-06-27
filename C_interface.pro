TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.c \
    atom/src/atom.c


HEADERS += \
    atom/include/atom.h

INCLUDEPATH += $$PWD/atom/include
