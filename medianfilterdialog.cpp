#include "medianfilterdialog.h"
#include "imagewindow.h"
#include <vector>

MedianFilterDialog::MedianFilterDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

MedianFilterDialog::~MedianFilterDialog()
{

}

void MedianFilterDialog::on_applyFilterButton_pressed() {

	int neighborhoodSize = ui.neighborhoodSizeBox->value();

	ImageWindow *pwindow = (ImageWindow*)this->parentWidget();
	pwindow->image.medianFilter(neighborhoodSize);
	pwindow->setImage(pwindow->image);
}
