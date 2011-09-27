#include "generatesinusoidimagedialog.h"
#include <cmath>
#include "rimage.h"
#include "imagewindow.h"
#include "SampledTone.h"

GenerateSinusoidImageDialog::GenerateSinusoidImageDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

GenerateSinusoidImageDialog::~GenerateSinusoidImageDialog()
{

}

void GenerateSinusoidImageDialog::on_generateButton_pressed() {
    //RImage img
    int width = ui.widthSpinBox->value();
    int height = ui.heightSpinBox->value();
    double N = ui.nSpinBox->value();
    double s = ui.sSpinBox->value();
    int numValues = 256;
    int maxValue = numValues - 1;
    double halfMaxValue = (double)maxValue / 2.0;

    //vector<int> pattern(width, 0);
    RImage img(width, height, QImage::Format_Indexed8);
    img.setGrayColorTable();
    int value;

    for (int x = 0; x < width; ++x) {
        //pattern[x] = sin(2.0 * SAMPLETONE_PI * s * x / N);
        value = floor(halfMaxValue + (halfMaxValue * sin(2.0 * SAMPLEDTONE_PI * s * x / N)));
        //value = floor((double)x / (double)width * 256);

        for (int y = 0; y < height; ++y) {
            img.setPixel(x, y, value);
        }
    }

    // create new window
    ImageWindow* win = new ImageWindow(parentWidget());
    win->setImage(img);
    win->show();
}
