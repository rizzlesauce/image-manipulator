# -------------------------------------------------
# Project created by QtCreator 2010-02-03T17:17:49
# -------------------------------------------------
# QT += phonon
TARGET = qt-project
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    rimage.cpp \
    imagewindow.cpp \
    FileLogger.cc \
    Debugger.cc \
    Utils.cc \
    gnuplot-cpp/gnuplot_i.cpp
HEADERS += mainwindow.h \
    rimage.h \
    gnuplot-cpp/gnuplot_i.hpp \
    imagewindow.h \
    FileLogger.h \
    Debugger.h \
    Utils.h
FORMS += mainwindow.ui \
    imagewindow.ui

# add external library links
LIBS += -ljack \
    -lpthread \
    -lrt
