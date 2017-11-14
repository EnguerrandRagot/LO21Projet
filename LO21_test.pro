QT += widgets
QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11
CONFIG += console

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    notes.cpp \
    viewer.cpp \
    command.cpp \
    newwidgets.cpp \
    loadxml.cpp

HEADERS += \
    mainwindow.h \
    notes.h \
    viewer.h \
    command.h \
    newwidgets.h \
    loadxml.h
