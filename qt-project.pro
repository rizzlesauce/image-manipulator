# -------------------------------------------------
# Project created by QtCreator 2010-02-03T17:17:49
# -------------------------------------------------
# QT += phonon
TARGET = qt-project
TEMPLATE = app
SOURCES += addsinecosinedialog.cpp \
    SampledChord.cpp \
    SampledTone.cpp \
    customfilterdialog.cpp \
    unsharpmaskdialog.cpp \
    medianfilterdialog.cpp \
    uniformaveragedialog.cpp \
    averageimagesdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    rimage.cpp \
    imagewindow.cpp \
    Utils.cpp \
    gnuplot_i.cpp \
    FileLogger.cpp \
    Debugger.cpp
HEADERS += addsinecosinedialog.h \
    SampledChord.h \
    SampledTone.h \
    customfilterdialog.h \
    unsharpmaskdialog.h \
    medianfilterdialog.h \
    uniformaveragedialog.h \
    averageimagesdialog.h \
    mainwindow.h \
    rimage.h \
    imagewindow.h \
    Debugger.h \
    Utils.h \
    gnuplot_i.h
FORMS += addsinecosinedialog.ui \
    customfilterdialog.ui \
    unsharpmaskdialog.ui \
    medianfilterdialog.ui \
    uniformaveragedialog.ui \
    averageimagesdialog.ui \
    mainwindow.ui \
    imagewindow.ui

# add external library links
LIBS += -ljack \
    -lpthread \
    -lrt \
    -lfftw3 \
    -lm
