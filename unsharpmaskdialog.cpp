#include "unsharpmaskdialog.h"
#include "imagewindow.h"

UnsharpMaskDialog::UnsharpMaskDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

UnsharpMaskDialog::~UnsharpMaskDialog()
{

}

void UnsharpMaskDialog::on_applyFilterButton_pressed() {

	int neighborhoodSize = ui.neighborhoodSizeBox->value();
	int sharpeningLevel = ui.sharpeningLevelBox->value();

	ImageWindow *pwindow = (ImageWindow*)this->parentWidget();
	pwindow->image.unsharpMask(neighborhoodSize, sharpeningLevel);
	pwindow->setImage(pwindow->image);
}
