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
#include <fstream>
#include <cstdio>
#include <fftw3.h>
#include <cmath>
#include "SampledTone.h"
#include "gnuplot_i.h"
#include "addsinecosinedialog.h"
#include "generatesinusoidimagedialog.h"
#include "FourierUtils.h"

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

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Image"), "./",
        tr("Image Files (*.png *.ppm *.pgm *.jpg *.JPG *.bmp)"));

    if (fileNames.size() == 0) {
            // no file chosen
    } else {
    	for (int i = 0; i < fileNames.size(); ++i) {
	            // file chosen
	        //setImage(new RImage(fileName));
	        RImage img(fileNames.at(i));

	        // create new window
	        ImageWindow* win = new ImageWindow(this);
	        win->setImage(img);
	        win->show();
	    }
    }
}

void MainWindow::on_actionGenerate_sinusoid_image_triggered() {
    // create new window
    GenerateSinusoidImageDialog* win = new GenerateSinusoidImageDialog(this);
    win->show();
}

void MainWindow::on_actionSine_wave_triggered() {
	int numSamples = 128;
	double cycles = 8.0;
    vector<double> wave(numSamples, 0.0);

    // create the wave
    for (int t = 0; t < numSamples; ++t) {
    	wave[t] = sin(2.0 * SAMPLEDTONE_PI * (double)t * cycles / (double)numSamples);
    }

    transformAndPlotWave(wave, "part_a_sine");
}

void MainWindow::on_actionCosine_wave_triggered() {
	int numSamples = 128;
	double cycles = 8.0;
    vector<double> wave(numSamples, 0.0);

    // create the wave
    for (int t = 0; t < numSamples; ++t) {
    	wave[t] = cos(2.0 * SAMPLEDTONE_PI * (double)t * cycles / (double)numSamples);
    }

    transformAndPlotWave(wave, "part_a_cosine");
}

void MainWindow::transformAndPlotWave(vector<double> wave, string waveName) {
	int numSamples = wave.size();
	fftw_complex *in, *out;
    fftw_plan p;
    double min, max;

    int numValues = numSamples;
    string imagePath;
    bool display = true;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numSamples);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numSamples);
    p = fftw_plan_dft_1d(numSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // display the wave
    imagePath = waveName + "_original.eps";

    // plot the wave
    //min = -10;
    //max = 10;
    min = max = 0.0;
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::justRealToComplex(wave[i], in[i]);
    	if (wave[i] < min) {
    		min = wave[i];
    	}
    	if (wave[i] > max) {
    		max = wave[i];
    	}
    }

    // create a new process
    pid_t pId1 = fork();

    if (pId1 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("f(t)");
            gplot.set_yrange(min, max);
            gplot.plot_x(wave);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // calculate the fourier transform
    fftw_execute(p);

    // normalize transform
    FourierUtils::normalize1DSignal(out, out, numSamples);

    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> real(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToReal(out[i], real[i]);
    	if (real[i] < min) {
    		min = real[i];
    	}
    	if (real[i] > max) {
    		max = real[i];
    	}
    }

    // display the real
    imagePath = waveName + "_real.eps";

    // create a new process
    pid_t pId2 = fork();

    if (pId2 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("real(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(real);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // plot imaginary
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> imaginary(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToImaginary(out[i], imaginary[i]);
    	if (imaginary[i] < min) {
    		min = imaginary[i];
    	}
    	if (imaginary[i] > max) {
    		max = imaginary[i];
    	}
    }

    // display the imaginary
    imagePath = waveName + "_imaginary.eps";

    // create a new process
    pid_t pId3 = fork();

    if (pId3 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("imaginary(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(imaginary);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // plot magnitude
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> magnitude(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToMagnitude(out[i], magnitude[i]);
    	if (magnitude[i] < min) {
    		min = magnitude[i];
    	}
    	if (magnitude[i] > max) {
    		max = magnitude[i];
    	}
    }

    // display the magnitude
    imagePath = waveName + "_magnitude.eps";

    // create a new process
    pid_t pId4 = fork();

    if (pId4 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("magnitude(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(magnitude);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // plot phase
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> phase(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToPhase(out[i], phase[i]);
    	if (phase[i] < min) {
    		min = phase[i];
    	}
    	if (phase[i] > max) {
    		max = phase[i];
    	}
    }

    // display the phase
    imagePath = waveName + "_phase.eps";

    // create a new process
    pid_t pId5 = fork();

    if (pId5 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("phase(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(phase);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // plot power
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> power(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToPower(out[i], power[i]);
    	if (power[i] < min) {
    		min = power[i];
    	}
    	if (power[i] > max) {
    		max = power[i];
    	}
    }

    // display the power
    imagePath = waveName + "_power.eps";

    // create a new process
    pid_t pId6 = fork();

    if (pId6 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("power(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(power);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
}

void MainWindow::on_actionSummed_sine_cosine_triggered() {
    AddSineCosineDialog* win = new AddSineCosineDialog(this);
    win->show();
}

void MainWindow::on_actionSignal_from_file_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Signal"), "./",
        tr("Signal Files (*.dat)"));

    if (fileName == NULL) {
            // no file chosen
    } else {
            // file chosen
    	vector<double> signal;

		string line;
		ifstream myfile(fileName.toStdString().c_str());
		if (myfile.is_open()) {
			while (!myfile.eof()) {
				getline(myfile, line);
				signal.push_back(atof(line.c_str()));
			}
			myfile.close();
		} else {
			Debugger::getInstance().print("Unable to open signal file");
		}

		transformAndPlotWave(signal, fileName.toStdString());
	}
}

void MainWindow::on_action1D_Low_Pass_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Signal"), "./",
        tr("Signal Files (*.dat)"));

    if (fileName == NULL) {
            // no file chosen
    } else {
            // file chosen
    	vector<double> signal;

		string line;
		ifstream myfile(fileName.toStdString().c_str());
		if (myfile.is_open()) {
			while (!myfile.eof()) {
				getline(myfile, line);
				signal.push_back(atof(line.c_str()));
			}
			myfile.close();
		} else {
			Debugger::getInstance().print("Unable to open signal file");
		}

		lowPassFilter(signal, fileName.toStdString());
	}
}

void MainWindow::lowPassFilter(vector<double> wave, string waveName) {
	int numSamples = wave.size();
	fftw_complex *in, *out;
    fftw_plan p, backward;
    double min, max;

    int numValues = numSamples;
    string imagePath;
    bool display = true;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numSamples);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numSamples);
    p = fftw_plan_dft_1d(numSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    backward = fftw_plan_dft_1d(numSamples, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    // display the wave
    imagePath = waveName + "_original.eps";

    // plot the wave
    //min = -10;
    //max = 10;
    min = max = 0.0;
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::justRealToComplex(wave[i], in[i]);
    	if (wave[i] < min) {
    		min = wave[i];
    	}
    	if (wave[i] > max) {
    		max = wave[i];
    	}
    }

    // create a new process
    pid_t pId1 = fork();

    if (pId1 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("f(t)");
            gplot.set_yrange(min, max);
            gplot.plot_x(wave);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // calculate the fourier transform
    fftw_execute(p);

    // normalize result
    FourierUtils::normalize1DSignal(out, out, numSamples);

    vector<double> lowPass;
    lowPass = butterworth1D(numSamples, (double)numSamples / 6.0, 2.0);

    // plot low pass filter
    min = max = 0.0;
    for (unsigned int i = 0; i < lowPass.size(); ++i) {
    	if (lowPass[i] < min) {
    		min = lowPass[i];
    	}
    	if (lowPass[i] > max) {
    		max = lowPass[i];
    	}
    }

    // display the real
    imagePath = waveName + "_real.eps";

    // create a new process
    pid_t pId2 = fork();

    if (pId2 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("freq");
            gplot.set_ylabel("low pass filter");
            gplot.set_yrange(min, max);
            gplot.plot_x(lowPass);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // calculate the new frequency function for the signal
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::realTimesComplex(lowPass[i], out[i], in[i]);
    }

    // inverse fourier transform
    fftw_execute(backward);


    // plot the filtered wave
    imagePath = waveName + "_lowpass.eps";

    vector<double> filteredWave(numSamples);

    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToReal(out[i], filteredWave[i]);
    }

    // plot the wave
    //min = -10;
    //max = 10;
    min = max = 0.0;
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::justRealToComplex(wave[i], in[i]);
    	if (filteredWave[i] < min) {
    		min = filteredWave[i];
    	}
    	if (filteredWave[i] > max) {
    		max = filteredWave[i];
    	}
    }

    // create a new process
    pid_t pId3 = fork();

    if (pId3 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("f(t)");
            gplot.set_yrange(min, max);
            gplot.plot_x(filteredWave);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }


    /*
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> real(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToReal(out[i], real[i]);
    	if (real[i] < min) {
    		min = real[i];
    	}
    	if (real[i] > max) {
    		max = real[i];
    	}
    }

    // display the real
    imagePath = waveName + "_real.eps";

    // create a new process
    pid_t pId2 = fork();

    if (pId2 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("real(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(real);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // plot imaginary
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> imaginary(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToImaginary(out[i], imaginary[i]);
    	if (imaginary[i] < min) {
    		min = imaginary[i];
    	}
    	if (imaginary[i] > max) {
    		max = imaginary[i];
    	}
    }

    // display the imaginary
    imagePath = waveName + "_imaginary.eps";

    // create a new process
    pid_t pId3 = fork();

    if (pId3 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("imaginary(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(imaginary);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // plot magnitude
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> magnitude(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToMagnitude(out[i], magnitude[i]);
    	if (magnitude[i] < min) {
    		min = magnitude[i];
    	}
    	if (magnitude[i] > max) {
    		max = magnitude[i];
    	}
    }

    // display the magnitude
    imagePath = waveName + "_magnitude.eps";

    // create a new process
    pid_t pId4 = fork();

    if (pId4 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("magnitude(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(magnitude);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // plot phase
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> phase(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToPhase(out[i], phase[i]);
    	if (phase[i] < min) {
    		min = phase[i];
    	}
    	if (phase[i] > max) {
    		max = phase[i];
    	}
    }

    // display the phase
    imagePath = waveName + "_phase.eps";

    // create a new process
    pid_t pId5 = fork();

    if (pId5 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("phase(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(phase);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }

    // plot power
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<double> power(numSamples);
    for (int i = 0; i < numSamples; ++i) {
    	FourierUtils::complexToPower(out[i], power[i]);
    	if (power[i] < min) {
    		min = power[i];
    	}
    	if (power[i] > max) {
    		max = power[i];
    	}
    }

    // display the power
    imagePath = waveName + "_power.eps";

    // create a new process
    pid_t pId6 = fork();

    if (pId6 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("lines");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("t");
            gplot.set_ylabel("power(F(t))");
            gplot.set_yrange(min, max);
            gplot.plot_x(power);

            if (display) {
                gplot.showonscreen();
                gplot.replot();

                // wait for input
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                cin.get();
            }

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    }
    */

    fftw_destroy_plan(p);
    fftw_destroy_plan(backward);
    fftw_free(in);
    fftw_free(out);

}

vector<double> MainWindow::butterworth1D(int size, double cutoff, double sharpness) {
	vector<double> butterworth(size);
	double cutoffSquared = pow(cutoff, 2);

	for (int i = 0; i < size; ++i) {
		butterworth[i] = 1.0 / (1.0 + pow(pow(i, 2) / cutoffSquared, sharpness));
	}

	return butterworth;
}
