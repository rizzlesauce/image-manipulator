#include "uniformaveragedialog.h"
#include "imagewindow.h"
using namespace std;


UniformAverageDialog::UniformAverageDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

UniformAverageDialog::~UniformAverageDialog()
{

}

void UniformAverageDialog::on_applyFilterButton_pressed() {

	int neighborHoodSize = ui.neighborhoodSizeBox->value();

	vector<vector<int> > weights(neighborHoodSize, vector<int>(neighborHoodSize));

	for (int x = 0; x < neighborHoodSize; ++x) {
		//weights[x].resize(neighborHoodSize);
		for (int y = 0; y < neighborHoodSize; ++y) {
			weights[x][y] = 1;
		}
	}

	ImageWindow *pwindow = (ImageWindow*)this->parentWidget();
	pwindow->image.weightedFilter(weights);
	pwindow->setImage(pwindow->image);
}
