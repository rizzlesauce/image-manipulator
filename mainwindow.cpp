#include "mainwindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileChooser()));
    Debugger::getInstance().setEnabled();
    connect(&Debugger::getInstance(), SIGNAL(messagePrinted(QString)), ui->plainTextEdit, SLOT(insertPlainText(QString)));

    Debugger::getInstance().print("mainwindow initialized\n");
}

MainWindow::~MainWindow()
{
    delete ui;
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
        win->show();
    }
}
