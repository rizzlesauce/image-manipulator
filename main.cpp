#include <QtGui/QApplication>
#include "mainwindow.h"
#include "Debugger.h"
#include <cstdio>
#include <sstream>
#include <cmath>
#include <vector>
#include <iostream>
#include "gnuplot_i.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int result = a.exec();

    return result;
}
