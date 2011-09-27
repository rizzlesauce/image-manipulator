#include "averageimagesdialog.h"
#include <QString>
#include <QFileDialog>
#include "Debugger.h"
#include "imagewindow.h"

AverageImagesDialog::AverageImagesDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

AverageImagesDialog::~AverageImagesDialog()
{

}

void AverageImagesDialog::on_averageButton_pressed() {
    if (fileNames.size() == 0) {
            // no files chosen
    } else {
        // open the images

        ImageWindow *pwindow = ((ImageWindow*)this->parentWidget());

        //RImage img;

        vector<RImage> images(fileNames.size());

        for (int i = 0; i < fileNames.size(); ++i) {
            images[i].load(fileNames.at(i));
        }

        pwindow->image.averageWithImages(images);
        pwindow->setImage(pwindow->image);
    }
}

void AverageImagesDialog::on_selectImagesButton_pressed() {

    fileNames = QFileDialog::getOpenFileNames(this, tr("Open Image"), "./",
        tr("Image Files (*.png *.ppm *.pgm *.jpg *.bmp)"));

    if (fileNames.size() == 0) {
            // no files chosen
        Debugger::getInstance().print("no file chosen");

    } else {
            // files chosen
    }
}
