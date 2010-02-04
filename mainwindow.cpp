#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileChooser()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFileChooser()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "./",
        tr("Image Files (*.png *.ppm *.pmg *.jpg *.bmp)"));

    if (fileName == NULL) {
            // no file chosen
    } else {
            // file chosen

        QImage* img = new QImage(fileName);

        QPixmap* pixmap = new QPixmap(QPixmap::fromImage(*img));

        QLabel* label = new QLabel(this);
        label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        //label->setText("first line\nsecond line");
        label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
        label->setGeometry(50, 100, img->width(),img->height());
        label->setPixmap(*pixmap);
        label->show();

 /*
        QLabel* imgLabel = new QLabel(fileName, this);
        imgLabel->setGeometry(50, 50, img->width(), img->height());
        imgLabel->move(100, 100);
        imgLabel->setPixmap(*pixmap);
        imgLabel->show();
        */
    }
}
