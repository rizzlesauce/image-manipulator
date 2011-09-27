#include "addsinecosinedialog.h"
#include "mainwindow.h"
#include <vector>
#include <cmath>
#include "SampledTone.h"

AddSineCosineDialog::AddSineCosineDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

AddSineCosineDialog::~AddSineCosineDialog()
{

}

void AddSineCosineDialog::on_okButton_pressed() {
    MainWindow *win = (MainWindow*)parentWidget();

    int numSamples = 128;
    double cycles = 8.0;
    vector<double> wave(numSamples, 0.0);

    double amountSine = ui.sineSpinBox->value();
    double amountCosine = ui.cosineSpinBox->value();

    // create the wave
    for (int t = 0; t < numSamples; ++t) {
        wave[t] += amountSine * sin(2.0 * SAMPLEDTONE_PI * (double)t * cycles / (double)numSamples);
        wave[t] += amountCosine * cos(2.0 * SAMPLEDTONE_PI * (double)t * cycles / (double)numSamples);
    }

    win->transformAndPlotWave(wave, "part_a_sum_sine_cosine");
}
