TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    atom/src/atom.c \
    exception/src/exception.c \
    main.c \
    mem/src/mem.c \
    mem/src/memchk.c \
    arena/src/arena.c


HEADERS += \
    atom/include/atom.h \
    exception/include/exception.h \
    mem/include/mem.h \
    arena/include/arena.h

INCLUDEPATH += $$PWD/atom/include
INCLUDEPATH += $$PWD/exception/include
INCLUDEPATH += $$PWD/mem/include
INCLUDEPATH += $$PWD/arena/include

DEFINES += MEM_ARENA
