# -------------------------------------------------
# Project created by QtCreator 2010-02-03T17:17:49
# -------------------------------------------------
# QT += phonon
TARGET = qt-project
TEMPLATE = app
INCLUDEPATH += /opt/local/include
SOURCES += rotatedialog.cpp \
    shrinkdialog.cpp \
    magnifydialog.cpp \
    FourierUtils.cpp \
    generatesinusoidimagedialog.cpp \
    addsinecosinedialog.cpp \
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
HEADERS += rotatedialog.h \
    shrinkdialog.h \
    magnifydialog.h \
    FourierUtils.h \
    generatesinusoidimagedialog.h \
    addsinecosinedialog.h \
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
FORMS += rotatedialog.ui \
    shrinkdialog.ui \
    magnifydialog.ui \
    generatesinusoidimagedialog.ui \
    addsinecosinedialog.ui \
    customfilterdialog.ui \
    unsharpmaskdialog.ui \
    medianfilterdialog.ui \
    uniformaveragedialog.ui \
    averageimagesdialog.ui \
    mainwindow.ui \
    imagewindow.ui

# add external library links
LIBS += -lpthread \
    -L/opt/local/lib -lfftw3 \
    -lm
