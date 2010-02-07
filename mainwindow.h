#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "rimage.h"
//#include <jack/jack.h>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFileChooser();

private:
    Ui::MainWindow *ui;
    RImage *image;

    void setImage(RImage* img);

private slots:
    //void plotHistogram();

private slots:
    void on_dial_valueChanged(int value);
    void on_printSamplesButton_pressed();
};

#endif // MAINWINDOW_H
