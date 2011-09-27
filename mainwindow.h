#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "rimage.h"
#include <cmath>
#include <fftw3.h>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void transformAndPlotWave(vector<double>, string);
    void lowPassFilter(vector<double>, string);
    vector<double> butterworth1D(int size, double cutoff, double sharpness);

public slots:
    void openFileChooser();

private:
    Ui::MainWindow *ui;
    void complexToPower(fftw_complex&, double&);
    void complexToMagnitude(fftw_complex&, double&);
    void complexToPhase(fftw_complex&, double&);
    void complexToReal(fftw_complex&, double&);
    void complexToImaginary(fftw_complex&, double&);
    void realToComplex(double&, fftw_complex&);
    void justRealToComplex(double&, fftw_complex&);
    void imaginaryToComplex(double&, fftw_complex&);
    void realImaginaryToComplex(double&, double&, fftw_complex&);

private slots:
    void on_actionSine_wave_triggered();
    void on_actionCosine_wave_triggered();
    void on_actionSummed_sine_cosine_triggered();
    void on_actionSignal_from_file_triggered();
    void on_actionGenerate_sinusoid_image_triggered();
    void on_action1D_Low_Pass_triggered();
};

#endif // MAINWINDOW_H
