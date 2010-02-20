#include "uniformaveragedialog.h"
#include "imagewindow.h"
#include "Debugger.h"
#include <sstream>
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

	int neighborhoodSize = ui.neighborhoodSizeBox->value();
	stringstream ss;
	ss << "neighborhood size: " << neighborhoodSize << endl;
	Debugger::getInstance().print(ss.str());

	vector<vector<int> > weights(neighborhoodSize, vector<int>(neighborhoodSize, 1));
	ImageWindow *pwindow = (ImageWindow*)this->parentWidget();
	pwindow->image.weightedFilter(weights);
	pwindow->setImage(pwindow->image);
}
