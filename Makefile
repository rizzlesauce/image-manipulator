#############################################################################
# Makefile for building: qt-project
# Generated by qmake (2.01a) (Qt 4.5.2) on: Tue Feb 9 23:07:56 2010
# Project:  qt-project.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile qt-project.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib -lQtGui -lQtCore -lpthread
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		mainwindow.cpp \
		rimage.cpp \
		imagewindow.cpp \
		Utils.cpp \
		gnuplot_i.cpp \
		FileLogger.cpp \
		Debugger.cpp moc_mainwindow.cpp \
		moc_imagewindow.cpp \
		moc_Debugger.cpp
OBJECTS       = main.o \
		mainwindow.o \
		rimage.o \
		imagewindow.o \
		Utils.o \
		gnuplot_i.o \
		FileLogger.o \
		Debugger.o \
		moc_mainwindow.o \
		moc_imagewindow.o \
		moc_Debugger.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		qt-project.pro
QMAKE_TARGET  = qt-project
DESTDIR       = 
TARGET        = qt-project

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_mainwindow.h ui_imagewindow.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: qt-project.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile qt-project.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile qt-project.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/qt-project1.0.0 || $(MKDIR) .tmp/qt-project1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/qt-project1.0.0/ && $(COPY_FILE) --parents mainwindow.h rimage.h imagewindow.h Debugger.h Utils.h gnuplot_i.h .tmp/qt-project1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp rimage.cpp imagewindow.cpp Utils.cpp gnuplot_i.cpp FileLogger.cpp Debugger.cpp .tmp/qt-project1.0.0/ && $(COPY_FILE) --parents mainwindow.ui imagewindow.ui .tmp/qt-project1.0.0/ && (cd `dirname .tmp/qt-project1.0.0` && $(TAR) qt-project1.0.0.tar qt-project1.0.0 && $(COMPRESS) qt-project1.0.0.tar) && $(MOVE) `dirname .tmp/qt-project1.0.0`/qt-project1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/qt-project1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp moc_imagewindow.cpp moc_Debugger.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_imagewindow.cpp moc_Debugger.cpp
moc_mainwindow.cpp: rimage.h \
		mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_imagewindow.cpp: rimage.h \
		imagewindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) imagewindow.h -o moc_imagewindow.cpp

moc_Debugger.cpp: Debugger.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) Debugger.h -o moc_Debugger.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h ui_imagewindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h ui_imagewindow.h
ui_mainwindow.h: mainwindow.ui
	/usr/bin/uic-qt4 mainwindow.ui -o ui_mainwindow.h

ui_imagewindow.h: imagewindow.ui
	/usr/bin/uic-qt4 imagewindow.ui -o ui_imagewindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		rimage.h \
		Debugger.h \
		gnuplot_i.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		rimage.h \
		ui_mainwindow.h \
		imagewindow.h \
		Debugger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

rimage.o: rimage.cpp rimage.h \
		Debugger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o rimage.o rimage.cpp

imagewindow.o: imagewindow.cpp imagewindow.h \
		rimage.h \
		ui_imagewindow.h \
		gnuplot_i.h \
		Debugger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o imagewindow.o imagewindow.cpp

Utils.o: Utils.cpp Utils.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Utils.o Utils.cpp

gnuplot_i.o: gnuplot_i.cpp gnuplot_i.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gnuplot_i.o gnuplot_i.cpp

FileLogger.o: FileLogger.cpp FileLogger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FileLogger.o FileLogger.cpp

Debugger.o: Debugger.cpp Debugger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Debugger.o Debugger.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_imagewindow.o: moc_imagewindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_imagewindow.o moc_imagewindow.cpp

moc_Debugger.o: moc_Debugger.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Debugger.o moc_Debugger.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

