#include <cstring>
#include "imagewindow.h"
#include "ui_imagewindow.h"
#include "averageimagesdialog.h"
#include "gnuplot_i.h"
#include "Debugger.h"
#include <QFileDialog>
#include "uniformaveragedialog.h"
#include "medianfilterdialog.h"
#include "unsharpmaskdialog.h"
#include "FourierUtils.h"
#include <fftw3.h>

ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::ImageWindow)
{
    m_ui->setupUi(this);

    //setCentralWidget(m_ui->label);

    connect(m_ui->actionEqualize, SIGNAL(triggered()), this, SLOT(equalizeImage()));

    Debugger::getInstance().print("imagewindow initialized\n");
}

ImageWindow::~ImageWindow()
{
    delete m_ui;
}

/**
 * Open up the average images dialog.
 */
void ImageWindow::on_actionAverage_With_Images_triggered() {
	AverageImagesDialog *dialog = new AverageImagesDialog(this);
	dialog->show();
}

void ImageWindow::saveImage() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "./",
        tr("Image Files (*.png *.ppm *.pgm *.jpg *.bmp)"));

    if (fileName == NULL) {
            // no file chosen
    } else {
            // file chosen
        //setImage(new RImage(fileName));
        image.save(fileName);
    }
}

void ImageWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ImageWindow::equalizeImage() {
    ImageWindow *newWindow = new ImageWindow(parentWidget());

    RImage img = image;
    img.equalize();

    newWindow->setImage(img);
    //newWindow->image.equalize();
    //newWindow->reloadPixmap();
    newWindow->show();
    /*
    image.equalize();

    reloadPixmap();
    */
}

void ImageWindow::on_actionClone_triggered() {
	ImageWindow *newWindow = new ImageWindow(parentWidget());
	newWindow->setImage(image);
	newWindow->show();
}

void ImageWindow::createHistogram(bool display, string fileName) {
    vector<int> counts;
    image.getHistogram(counts);

    int numValues = counts.size();

    string imagePath = fileName;

    int biggestCount = 0;

    for (uint x = 0; x < counts.size(); ++x) {
        if (counts[x] > biggestCount) {
            biggestCount = counts[x];
        }
    }

    // create a new process
    pid_t pId = fork();

    if (pId == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("histograms");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("Value");
            gplot.set_ylabel("Count");
            gplot.set_yrange(0, biggestCount);
            gplot.plot_x(counts);

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
    } else {
            // parent
    }
}

void ImageWindow::plotEqualizationTable(bool display, string fileName) {
    RImage img = image;
    vector<int> table;
    img.equalize(table);

    int numValues = table.size();

    string imagePath = fileName;

    int biggestCount = 0;

    for (uint x = 0; x < table.size(); ++x) {
        if (table[x] > biggestCount) {
            biggestCount = table[x];
        }
    }

    // create a new process
    pid_t pId = fork();

    if (pId == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style("points");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("Value");
            gplot.set_ylabel("Mapped Value");
            gplot.set_yrange(0, biggestCount);
            gplot.plot_x(table);

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
    } else {
            // parent
    }
}

void ImageWindow::setImage(RImage &img) {
    image = img;
    //resize(0, 0);
    reloadPixmap();
    resize(image.width() + 100, image.height() + 100);
    //adjustSize();
}

void ImageWindow::reloadPixmap() {
    //setFixedSize(image.width(), image.height());
    m_ui->label->setPixmap(QPixmap::fromImage(image));
    //m_ui->label->adjustSize();
    //m_ui->label->setGeometry(0, 0, image.width(), image.height());
}

void ImageWindow::on_actionSave_triggered()
{
    saveImage();
}

void ImageWindow::on_actionSave_Histogram_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Histogram"), "./",
        tr("Postscript Files (*.ps *.eps)"));

    if (fileName == NULL) {
            // no file chosen
    } else {
            // file chosen
        createHistogram(false, fileName.toStdString());
    }
}

void ImageWindow::on_actionShow_Histogram_triggered()
{
    createHistogram(true, "");
}

void ImageWindow::on_actionShow_Equalization_Table_triggered()
{
    plotEqualizationTable(true, "");
}

void ImageWindow::on_actionSave_Equalization_Table_Plot_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Histogram"), "./",
        tr("Postscript Files (*.ps *.eps)"));

    if (fileName == NULL) {
            // no file chosen
    } else {
            // file chosen
        plotEqualizationTable(false, fileName.toStdString());
    }
}

void ImageWindow::on_actionUniform_Average_triggered() {
	UniformAverageDialog *dialog = new UniformAverageDialog(this);
	dialog->show();
}

void ImageWindow::on_actionMedian_Filter_triggered() {
	MedianFilterDialog *dialog = new MedianFilterDialog(this);
	dialog->show();
}

void ImageWindow::on_actionUnsharp_Mask_triggered() {
	UnsharpMaskDialog *dialog = new UnsharpMaskDialog(this);
	dialog->show();
}

void ImageWindow::on_actionSobelX_triggered() {
	image.sobelX();
	this->reloadPixmap();
}

void ImageWindow::on_actionSobelY_triggered() {
	image.sobelY();
	this->reloadPixmap();
}

void ImageWindow::on_actionLaplacian_triggered() {
	image.laplacian();
	reloadPixmap();
}

void ImageWindow::on_actionGradientMagnitude_triggered() {
	image.gradientMagnitude();
	reloadPixmap();
}

void ImageWindow::on_actionFourier_Transform_triggered() {
	transformAndPlotImage();
}

void ImageWindow::transformAndPlotImage() {

	vector<double> xs, ys, zs;

	string signalName = "myimage";
	string style3d = "lines";
	//int samples3d = 10;

	int width = image.width();
	int height = image.height();

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			xs.push_back((double)x);
			ys.push_back((double)y);
		}
	}

	int size = width * height;

	fftw_complex *in, *out;
    fftw_plan p;
    double min, max;

    int numValues = 256;
    string imagePath;
    //bool display = true;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    p = fftw_plan_dft_2d(width, height, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    /*
    // display the wave
    imagePath = signalName + "_original.eps";

    // plot the wave
    //min = -10;
    //max = 10;
    zs.clear();
    min = max = 0.0;
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double value = (double)image.pixelIndex(x, y);
    		zs.push_back(value);

    		FourierUtils::justRealToComplex(value, in[FFTW_INDEX2D(x, y, height)]);

    		if (value < min) {
    			min = value;
    		}
    		if (value > max) {
    			max = value;
    		}
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
            gplot.set_style(style3d);
            gplot.set_hidden3d();
            //gplot.set_samples(samples3d);
            gplot.set_isosamples(samples3d);
            //gplot.set_ticslevel(0);
            gplot.set_xrange(0, width);
            gplot.set_xlabel("x");
            gplot.set_yrange(0, height);
            gplot.set_ylabel("y");
            gplot.set_zrange(min, max);
            gplot.set_zlabel("f(x, y)");
            gplot.plot_xyz(xs, ys, zs);

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

    // set the input signal
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double value = (double)image.pixelIndex(x, y);
    		FourierUtils::justRealToComplex(value, in[FFTW_INDEX2D(x, y, height)]);
    	}
    }

    // calculate the fourier transform
    fftw_execute(p);
    FourierUtils::normalize1DSignal(out, out, size);

    // create a magnitude representation image
    //min = -10;
    //max = 10;
    bool minSet = false;
    bool maxSet = false;
    vector<vector<double> > magnitude(width, vector<double>(height));
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
	    	FourierUtils::complexToMagnitude(out[FFTW_INDEX2D(x, y, height)], magnitude[x][y]);
	    	if (magnitude[x][y] < min || !minSet) {
	    		minSet = true;
	    		min = magnitude[x][y];
	    	}
	    	if (magnitude[x][y] > max || !maxSet) {
	    		maxSet = true;
	    		max = magnitude[x][y];
	    	}
    	}
    }

    stringstream ss;
    ss << "min: " << min << "\n";
    ss << "max: " << max << "\n";
    Debugger::getInstance().print(ss.str());

    double magRange = max - min;
    double magScale;
    if (magRange == 0.0) {
    	magScale = 0.0;
    } else {
    	magScale = (double)(numValues - 1) / magRange;
    }

    // create the image
    ImageWindow *newWindow = new ImageWindow(parentWidget());

	RImage magImage(width, height, QImage::Format_Indexed8);
	magImage.setGrayColorTable();

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int value = floor((magnitude[x][y] - min) * magScale);
			magImage.setPixel(x, y, value);
		}
	}

    newWindow->setImage(magImage);
    newWindow->show();

    /*
    //min = -10;
    //max = 10;
    min = max = 0.0;
    vector<vector<double>> real(width, vector<double>(height));
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
	    	FourierUtils::complexToReal(out[FFTW_INDEX2D(x, y, height)], real[x][y]);
	    	if (real[x][y] < min) {
	    		min = real[x][y];
	    	}
	    	if (real[x][y] > max) {
	    		max = real[x][y];
	    	}
	    }
    }

    // display the real
    imagePath = signalName + "_real.eps";

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
    imagePath = signalName + "_imaginary.eps";

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
    imagePath = signalName + "_magnitude.eps";

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
    imagePath = signalName + "_phase.eps";

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
    zs.clear();
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double power;
	    	FourierUtils::complexToPower(out[FFTW_INDEX2D(x, y, height)], power);
	    	zs.push_back(power);
	    	if (power < min) {
	    		min = power;
	    	}
	    	if (power > max) {
	    		max = power;
	    	}
    	}
    }

    // display the power
    imagePath = signalName + "_power.eps";

    // create a new process
    pid_t pId6 = fork();

    if (pId6 == 0) {
            // child process
        try {

            Gnuplot gplot;

            if (!imagePath.empty()) {
                gplot.save(imagePath);
            }
            gplot.set_style(style3d);
            gplot.set_hidden3d();
            //gplot.set_samples(samples3d);
            gplot.set_isosamples(samples3d);
            //gplot.set_ticslevel(0);
            gplot.set_xrange(0, width);
            gplot.set_xlabel("x");
            gplot.set_yrange(0, height);
            gplot.set_ylabel("y");
            gplot.set_zrange(min, max);
            gplot.set_zlabel("power(F(u))");
            gplot.plot_xyz(xs, ys, zs);

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
    fftw_free(in);
    fftw_free(out);
}

void ImageWindow::on_actionReverse_Fourier_Transform_triggered() {

	int width = image.width();
	int height = image.height();

	int size = width * height;

	fftw_complex *in, *out;
    fftw_plan p;
    double min, max;

    int numValues = 256;
    string imagePath;
    //bool display = true;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    p = fftw_plan_dft_2d(width, height, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    // set the input signal
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double value = (double)image.pixelIndex(x, y);
    		FourierUtils::justRealToComplex(value, in[FFTW_INDEX2D(x, y, height)]);
    	}
    }

    FourierUtils::normalize1DSignal(out, out, size);

    // calculate the fourier transform
    fftw_execute(p);

    // reconstruct the image
    bool minSet = false;
    bool maxSet = false;
    vector<vector<double> > reals(width, vector<double>(height));
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
	    	FourierUtils::complexToReal(out[FFTW_INDEX2D(x, y, height)], reals[x][y]);
	    	if (reals[x][y] < min || !minSet) {
	    		minSet = true;
	    		min = reals[x][y];
	    	}
	    	if (reals[x][y] > max || !maxSet) {
	    		maxSet = true;
	    		max = reals[x][y];
	    	}
    	}
    }

    stringstream ss;
    ss << "min: " << min << "\n";
    ss << "max: " << max << "\n";
    Debugger::getInstance().print(ss.str());

    double realsRange = max - min;
    double realsScale;
    if (realsRange == 0.0) {
    	realsScale = 0.0;
    } else {
    	realsScale = (double)(numValues - 1) / realsRange;
    }

    // create the image
    ImageWindow *newWindow = new ImageWindow(parentWidget());

	RImage realsImage(width, height, QImage::Format_Indexed8);
	realsImage.setGrayColorTable();

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int value = floor((reals[x][y] - min) * realsScale);
			realsImage.setPixel(x, y, value);
		}
	}

    newWindow->setImage(realsImage);
    newWindow->show();

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
}

void ImageWindow::on_actionUniform_Average_Frequency_triggered() {
	int width = image.width();
	int height = image.height();

	int kernelSize = 3;

	int paddedWidth = width + kernelSize - 1;
	int paddedHeight = height + kernelSize - 1;
	int paddedSize = paddedWidth * paddedHeight;

	// calculate the spatial kernel
	// uniform average 9x9
	//vector<vector<double> > kernel(kernelSize, vector<double>(kernelSize, 1.0 / (double)(kernelSize << 1)));
	vector<vector<double> > kernel(kernelSize, vector<double>(kernelSize, 1.0));

	// "center" the filter in the signal
	vector<vector<double> > filter(paddedWidth, vector<double>(paddedHeight, 0.0));

	int offset = kernelSize / 2;

	for (int x = 0; x < kernelSize; ++x) {
		for (int y = 0; y < kernelSize; ++y) {
			int relX = x - offset;
			int relY = y - offset;

			int actualX, actualY;

			if (relX < 0) {
				actualX = paddedWidth + relX;
			} else {
				actualX = relX;
			}

			if (relY < 0) {
				actualY = paddedHeight + relY;
			} else {
				actualY = relY;
			}

			filter[actualX][actualY] = kernel[x][y];
		}
	}

	// fourier transform the image and the kernel
	fftw_complex *in, *out, *tmpBuffer;
    fftw_plan forward, backward;
    //int signalSize = width * height;
    int signalSize = paddedSize;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * signalSize);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * signalSize);
    tmpBuffer = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * signalSize);
    forward = fftw_plan_dft_2d(paddedWidth, paddedHeight, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    backward = fftw_plan_dft_2d(paddedWidth, paddedHeight, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    // zero out the input buffer
    for (int i = 0; i < signalSize; ++i) {
    	double val = 0.0;
    	FourierUtils::justRealToComplex(val, in[i]);
    }

    // fill the input buffer with image signal
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double value = (double)image.pixelIndex(x, y);
    		FourierUtils::justRealToComplex(value, in[FFTW_INDEX2D(x, y, paddedHeight)]);
    	}
    }

    // calculate the fourier transform of the image
    fftw_execute(forward);
    // normalize result before inverse transform
    //FourierUtils::normalize1DSignal(out, out, signalSize);

    // copy the output to temporary buffer
    memcpy((void*)tmpBuffer, (void*)out, sizeof(fftw_complex) * signalSize);

    // zero out the intput buffer
    for (int i = 0; i < signalSize; ++i) {
    	double val = 0.0;
    	FourierUtils::justRealToComplex(val, in[i]);
    }

    // fill the input buffer with filter signal
    for (int x = 0; x < paddedWidth; ++x) {
    	for (int y = 0; y < paddedHeight; ++y) {
    		double value = filter[x][y];
    		FourierUtils::justRealToComplex(value, in[FFTW_INDEX2D(x, y, paddedHeight)]);
    	}
    }

    // calculate the fourier transform of the filter
    fftw_execute(forward);
    // normalize result before inverse transform
    //FourierUtils::normalize1DSignal(out, out, signalSize);

    // f(x, y) real and even <=> F(u, v) real and even

    /*
    // zero out the real part of H(u)
    for (int i = 0; i < signalSize; ++i) {
    	double val = 0.0;
		FourierUtils::realToComplex(val, out[i]);
    }
    */

    // perform the array multiplication of the transforms
    for (int x = 0; x < paddedWidth; ++x) {
    	for (int y = 0; y < paddedHeight; ++y) {
    		double real;
    		FourierUtils::complexToReal(out[FFTW_INDEX2D(x, y, paddedHeight)], real);
    		FourierUtils::realTimesComplex(real, tmpBuffer[FFTW_INDEX2D(x, y, paddedHeight)],
    				in[FFTW_INDEX2D(x, y, paddedHeight)]);
    		/*
    		FourierUtils::complexTimesComplex(tmpBuffer[FFTW_INDEX2D(x, y, paddedHeight)],
    				out[FFTW_INDEX2D(x, y, paddedHeight)],
    				in[FFTW_INDEX2D(x, y, paddedHeight)]);
    				*/
    	}
    }

    // inverse transform
    fftw_execute(backward);
    // normalize result after backward inverse
    FourierUtils::normalize1DSignal(out, out, signalSize);

    // construct image from inverse transform
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double value;
    		FourierUtils::complexToReal(out[FFTW_INDEX2D(x, y, paddedHeight)], value);

    		if (value < 0.0) {
    			value = 0.0;
    		} else if (value > 255.0) {
    			value = 255.0;
    		}

    		image.setPixel(x, y, (int)value);
    	}
    }

    // refresh image
    reloadPixmap();

    // free memory
    fftw_destroy_plan(forward);
    fftw_destroy_plan(backward);
    fftw_free(in);
    fftw_free(out);
    fftw_free(tmpBuffer);
}

void ImageWindow::on_actionRemove_Interference_triggered() {
	int width = image.width();
	int height = image.height();
	int signalSize = width * height;
	fftw_complex *in, *out;
    fftw_plan forward, backward;
    //double min, max;

    //int numValues = signalSize;
    //string imagePath;
    //bool display = true;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * signalSize);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * signalSize);
    forward = fftw_plan_dft_1d(signalSize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    backward = fftw_plan_dft_1d(signalSize, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    // fill the input buffer
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double value = (double)image.pixelIndex(x, y);
    		FourierUtils::justRealToComplex(value, in[FFTW_INDEX2D(x, y, height)]);
    	}
    }

    // calculate the fourier transform
    fftw_execute(forward);

    // normalize result and copy back to input buffer
    FourierUtils::normalize1DSignal(out, out, signalSize);

    // deal with the wayward frequencies
    //double newMagnitude = 0.0;
    //FourierUtils::realToComplex(newMagnitude, in[0]);
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		fftw_complex top, bottom, left, right;
    		double r_top, r_bottom, r_left, r_right, r_average;
    		double i_top, i_bottom, i_left, i_right, i_average;

    		if (y == 0) {
    			FourierUtils::copyComplex(out[FFTW_INDEX2D(x, y, height)], top);
    		} else {
    			FourierUtils::copyComplex(out[FFTW_INDEX2D(x, y - 1, height)], top);
    		}
    		if (y == height - 1) {
    			FourierUtils::copyComplex(out[FFTW_INDEX2D(x, y, height)], bottom);
    		} else {
    			FourierUtils::copyComplex(out[FFTW_INDEX2D(x, y + 1, height)], bottom);
    		}
    		if (x == 0) {
    			FourierUtils::copyComplex(out[FFTW_INDEX2D(x, y, height)], left);
    		} else {
    			FourierUtils::copyComplex(out[FFTW_INDEX2D(x - 1, y, height)], left);
    		}
    		if (x == width - 1) {
    			FourierUtils::copyComplex(out[FFTW_INDEX2D(x, y, height)], right);
    		} else {
    			FourierUtils::copyComplex(out[FFTW_INDEX2D(x + 1, y, height)], right);
    		}

    		FourierUtils::complexToRealImaginary(top, r_top, i_top);
    		FourierUtils::complexToRealImaginary(bottom, r_bottom, i_bottom);
    		FourierUtils::complexToRealImaginary(left, r_left, i_left);
    		FourierUtils::complexToRealImaginary(right, r_right, i_right);

    		r_average = (r_top + r_bottom + r_left + r_right) / 4.0;
    		i_average = (i_top + i_bottom + i_left + i_right) / 4.0;

    		FourierUtils::realImaginaryToComplex(r_average, i_average, in[FFTW_INDEX2D(x, y, height)]);
    	}
    }
    int numValues = 256;
    double max, min;
    // create a magnitude representation image
    //min = -10;
    //max = 10;
    bool minSet = false;
    bool maxSet = false;
    vector<vector<double> > magnitude(width, vector<double>(height));
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
	    	FourierUtils::complexToMagnitude(out[FFTW_INDEX2D(x, y, height)], magnitude[x][y]);
	    	if (magnitude[x][y] < min || !minSet) {
	    		minSet = true;
	    		min = magnitude[x][y];
	    	}
	    	if (magnitude[x][y] > max || !maxSet) {
	    		maxSet = true;
	    		max = magnitude[x][y];
	    	}
    	}
    }

    stringstream ss;
    ss << "min: " << min << "\n";
    ss << "max: " << max << "\n";
    Debugger::getInstance().print(ss.str());

    double magRange = max - min;
    double magScale;
    if (magRange == 0.0) {
    	magScale = 0.0;
    } else {
    	magScale = (double)(numValues - 1) / magRange;
    }

    // create the image
    ImageWindow *newWindow = new ImageWindow(parentWidget());

	RImage magImage(width, height, QImage::Format_Indexed8);
	magImage.setGrayColorTable();

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int value = floor((magnitude[x][y] - min) * magScale);
			magImage.setPixel(x, y, value);
		}
	}

    newWindow->setImage(magImage);
    newWindow->show();


    // inverse fourier transform
    fftw_execute(backward);

    // set the pixels
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double value;
    		FourierUtils::complexToReal(out[FFTW_INDEX2D(x, y, height)], value);
    		if (value < 0.0) {
    			value = 0.0;
    		}
    		if (value > 255.0) {
    			value = 255.0;
    		}
    		image.setPixel(x, y, (int)value);
    	}
    }
    reloadPixmap();

    fftw_destroy_plan(forward);
    fftw_destroy_plan(backward);
    fftw_free(in);
    fftw_free(out);
}
