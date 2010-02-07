#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "gnuplot-cpp/gnuplot_i.hpp"
//#include "rimage.h"
//#include <jack/jack.h>

#include "imagewindow.h"

#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <iostream>
#include <vector>
#include "Debugger.h"
#include <sstream>
#include <cstdio>

using namespace std;

extern bool printSamples;
extern float audioFactor;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileChooser()));
    Debugger::getInstance().setEnabled();
    connect(&Debugger::getInstance(), SIGNAL(messagePrinted(QString)), ui->plainTextEdit, SLOT(insertPlainText(QString)));//appendPlainText(QString)));

    image = NULL;

    Debugger::getInstance().print("mainwindow initialized\n");
}

MainWindow::~MainWindow()
{
    delete ui;
    if (image == NULL) {
        delete image;
    }
}

void MainWindow::setImage(RImage* img) {
    if (image == NULL) {
        delete image;
    }

    image = img;
}

void MainWindow::openFileChooser()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "./",
        tr("Image Files (*.png *.ppm *.pgm *.jpg *.bmp)"));

    if (fileName == NULL) {
            // no file chosen
    } else {
            // file chosen
        //setImage(new RImage(fileName));
        RImage img(fileName);

        // create new window
        ImageWindow* win = new ImageWindow(this);
        win->setImage(img);
        //win->move(
        //win->setGeometry(100, 100, img->width(), img->height());

        //QLabel* label = new QLabel(d);
        //label->setGeometry(0, 0, image->width(), image->height());
        //label->setPixmap(QPixmap::fromImage(*image));//*pixmap);

        //d->show();
        win->show();

        //ui->imageLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        //label->setText("first line\nsecond line");
        //ui->imageLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

        //ui->imageLabel->setGeometry(50, 100, image->width(),image->height());
        //ui->imageLabel->setPixmap(*pixmap);

        //imageLabel->show();

 /*
        QLabel* imgLabel = new QLabel(fileName, this);
        imgLabel->setGeometry(50, 50, img->width(), img->height());
        imgLabel->move(100, 100);
        imgLabel->setPixmap(*pixmap);
        imgLabel->show();
        */
        //plotHistogram();
    }
}

void MainWindow::on_printSamplesButton_pressed()
{
    printSamples = true;
}

void MainWindow::on_dial_valueChanged(int value)
{
    stringstream ss;
    ss << "dial change: " << value << endl;

    Debugger::getInstance().print(ss.str());

    audioFactor = value / 50.0;

    ss.clear();
    ss << "audio factor: " << audioFactor << endl;
    Debugger::getInstance().print(ss.str());
}
