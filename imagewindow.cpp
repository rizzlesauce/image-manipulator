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
#include "SampledTone.h"
#include <fftw3.h>
#include <algorithm>

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
	bool first;
	double min, max;

	unsigned int kernelSize = 9;

	int paddedWidth = width + kernelSize - 1;
	int paddedHeight = height + kernelSize - 1;
	int paddedSize = paddedWidth * paddedHeight;

	// calculate the spatial kernel
	// uniform average 9x9
	//vector<vector<double> > kernel(kernelSize, vector<double>(kernelSize, 1.0 / (double)(kernelSize << 1)));
	vector<vector<double> > kernel(kernelSize, vector<double>(kernelSize, 1.0));

	// "center" the filter in the spatial domain
	vector<vector<double> > filter(paddedWidth, vector<double>(paddedHeight, 0.0));

	int offset = kernelSize >> 1; // divided by 2

	max = 1.0;
	min = 0.0;
	first = true;
	for (unsigned int x = 0; x < kernelSize; ++x) {
		for (unsigned int y = 0; y < kernelSize; ++y) {
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

			if (first) {
				max = kernel[x][y];
				first = false;
			} else {
				if (kernel[x][y] < min) {
					min = kernel[x][y];
				}
				if (kernel[x][y] > max) {
					max = kernel[x][y];
				}
			}
		}
	}

	stringstream ss;
	ss << "filter: min: " << min << ", max: " << max << endl;
	Debugger::getInstance().print(ss.str());

	// for debugging purposes display the filter
	RImage filterImg(paddedWidth, paddedHeight, QImage::Format_Indexed8);
	filterImg.setGrayColorTable();
	for (int x = 0; x < paddedWidth; ++x) {
		for (int y = 0; y < paddedHeight; ++y) {
			double val = (filter[x][y] - min) / (max - min) * 255.0;
			filterImg.setPixel(x, y, (int)val);
		}
	}
	ImageWindow *filterWin = new ImageWindow(this->parentWidget());
	filterWin->setImage(filterImg);
	filterWin->show();

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

	// for debugging purposes display the padded image
	RImage paddedImg(paddedWidth, paddedHeight, QImage::Format_Indexed8);
	paddedImg.setGrayColorTable();
	for (int x = 0; x < paddedWidth; ++x) {
		for (int y = 0; y < paddedHeight; ++y) {
			double val;
			FourierUtils::complexToReal(in[FFTW_INDEX2D(x, y, paddedHeight)], val);
			paddedImg.setPixel(x, y, (int)val);
		}
	}
	ImageWindow *paddedWin = new ImageWindow(this->parentWidget());
	paddedWin->setImage(paddedImg);
	paddedWin->show();

    // calculate the fourier transform of the image
    fftw_execute(forward);
    // normalize result before inverse transform
    FourierUtils::normalize1DSignal(out, out, signalSize);

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

    // f(x, y) real and even <=> F(u, v) real and even

    /*
    // zero out the imaginary part of H(u)
    for (int i = 0; i < signalSize; ++i) {
    	double val = 0.0;
		//FourierUtils::realToComplex(val, out[i]);
    	FourierUtils::imaginaryToComplex(val, out[i]);
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

    // get min and max
    first = true;
    for (int i = 0; i < signalSize; ++i) {
    	double val;
    	FourierUtils::complexToReal(out[i], val);
    	val = val / (double)pow(kernelSize, 2);
    	if (first) {
    		min = max = val;
    		first = false;
    	} else {
    		if (val < min) {
    			min = val;
    		}
    		if (val > max) {
    			max = val;
    		}
    	}
    }

    // debugging info
	ss.clear();
	ss << "reverse transform: min: " << min << ", max: " << max << endl;
	Debugger::getInstance().print(ss.str());

    // construct image from inverse transform
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {
    		double value;
    		FourierUtils::complexToReal(out[FFTW_INDEX2D(x, y, paddedHeight)], value);

    		// since averaging, divide result by kernelSize^2
    		value = value / (double)pow(kernelSize, 2);

    		/*
    		// scale value
    		value = (value - min) / (max - min) * 255.0;
    		*/

    		// clip value
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

    vector<int> maximaIndices;
    maximaIndices.push_back(FFTW_INDEX2D(0, 0, height));
    maximaIndices.push_back(FFTW_INDEX2D(width - 1, 0, height));
    maximaIndices.push_back(FFTW_INDEX2D(width - 1, height - 1, height));
    maximaIndices.push_back(FFTW_INDEX2D(0, height - 1, height));

    int neighborhoodSize = 3;
    int potentialNeighborhoodSize = (int)pow(neighborhoodSize, 2);
    fftw_complex neighborhood[potentialNeighborhoodSize];

    int ignoreDistance = 0;//15;

    int offset = neighborhoodSize / 2;
    int numIncluded;
    bool tooClose;
    int maximaX, maximaY, minX, maxX, minY, maxY;

    // copy the output to the input
    for (int i = 0; i < signalSize; ++i) {
    	FourierUtils::copyComplex(out[i], in[i]);
    }

    // deal with the wayward frequencies
    //double newMagnitude = 0.0;
    //FourierUtils::realToComplex(newMagnitude, in[0]);
    for (int x = 0; x < width; ++x) {
    	for (int y = 0; y < height; ++y) {

    		// skip points within a distance of local maxima
			tooClose = false;
			for (unsigned int i = 0; i < maximaIndices.size(); ++i) {
				maximaX = FFTW_INDEX2D_X(maximaIndices[i], height);
				maximaY = FFTW_INDEX2D_Y(maximaIndices[i], height);

				minX = maximaX - ignoreDistance;
				maxX = maximaX + ignoreDistance;
				minY = maximaY - ignoreDistance;
				maxY = maximaY + ignoreDistance;

				if (x >= minX && x <= maxX &&
						y >= minY && y <= maxY) {
					tooClose = true;
					break;
				}
			}

			if (tooClose) {
				continue;
			}

    		// gather the neighborhood
    		numIncluded = 0;
    		for (int j = 0 - offset; j <= offset; ++j) {
				int realX = x + j;

				if (realX < 0 || realX > width) {
						// point is outside the image
					// don't include these
					continue;
				}

    			for (int k = 0 - offset; k <= offset; ++k) {
    				int realY = y + k;

    				if (j == 0 && k == 0) {
						// don't include origin of the neighborhood
    					continue;
    				}

    				if (realY < 0 || realY > height) {
							// point is outside the image
    					// don't include it
    					continue;
    				}

		    		// don't include points within a distance of local maxima
					tooClose = false;
					for (unsigned int i = 0; i < maximaIndices.size(); ++i) {
						maximaX = FFTW_INDEX2D_X(maximaIndices[i], height);
						maximaY = FFTW_INDEX2D_Y(maximaIndices[i], height);

						minX = maximaX - ignoreDistance;
						maxX = maximaX + ignoreDistance;
						minY = maximaY - ignoreDistance;
						maxY = maximaY + ignoreDistance;

						if (realX >= minX && realX <= maxX &&
								realY >= minY && realY <= maxY) {
							tooClose = true;
							break;
						}
					}

					if (tooClose) {
						continue;
					}

    				// otherwise, this point qualifies, so include it
    				FourierUtils::copyComplex(out[FFTW_INDEX2D(realX, realY, height)], neighborhood[numIncluded]);
    				++numIncluded;
    			}
    		}

    		//calculate the neighborhood average
    		fftw_complex neighborhoodAverage;
    		double tmpReal, tmpImaginary;
    		double averageReal = 0.0;
    		double averageImaginary = 0.0;
    		for (int i = 0; i < numIncluded; ++i) {
	    		FourierUtils::complexToRealImaginary(neighborhood[i], tmpReal, tmpImaginary);
	    		averageReal += tmpReal;
	    		averageImaginary += tmpImaginary;
    		}
    		if (numIncluded > 0) {
    			averageReal = averageReal / (double)numIncluded;
    			averageImaginary = averageImaginary / (double)numIncluded;
    		} else {
	    		FourierUtils::complexToRealImaginary(out[FFTW_INDEX2D(x, y, height)], averageReal,
	    				averageImaginary);
    		}
    		FourierUtils::realImaginaryToComplex(averageReal, averageImaginary, neighborhoodAverage);

    		// check if point is above normal magnitude
    		double averageMagnitude, thisMagnitude;
    		FourierUtils::complexToMagnitude(neighborhoodAverage, averageMagnitude);
    		FourierUtils::complexToMagnitude(out[FFTW_INDEX2D(x, y, height)], thisMagnitude);

    		/*
    		stringstream magss;
    		magss << "magnitude amplitude range: " << thisMagnitude << endl;
    		Debugger::getInstance().print(magss.str());
    		*/

    		if (thisMagnitude > (averageMagnitude * 1050.0)) {
					// this is a local maximum
    			Debugger::getInstance().print("local maximum found\n");
    			stringstream ss7;
    			ss7 << "x: " << x << ", y: " << y << ", thisMag: " << thisMagnitude
    					<< ", avgMag: " << averageMagnitude << endl;
    			Debugger::getInstance().print(ss7.str());

    			//maximaIndices.push_back(FFTW_INDEX2D(x, y, height));
	    		FourierUtils::copyComplex(neighborhoodAverage, in[FFTW_INDEX2D(x, y, height)]);
    		} else {
    			// leave it the way it is
    		}
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
	    	FourierUtils::complexToMagnitude(in[FFTW_INDEX2D(x, y, height)], magnitude[x][y]);
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
    ss << "spectrum: min: " << min << ", max: " << max << endl;
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

void ImageWindow::on_actionCrop_and_Resize_triggered() {

	// first find the dimensions of the chess piece
	int left, right, top, bottom;
	int width = image.width();
	int height = image.height();
	left = right = top = bottom = 0;
	int foregroundIndex = 0;
	int backgroundIndex = 1;
	bool found;
	int destWidth = 100;
	int destHeight = 100;

	// convert to monochrome
	QImage monochrome = QImage(width, height, QImage::Format_Mono);
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int index = image.pixelIndex(x, y);
			if (index > (255 / 2)) {
				index = 1;
			} else {
				index = 0;
			}
			monochrome.setPixel(x, y, index);
		}
	}

	// find the top
	found = false;
	for (int y = 0; y < height && !found; ++y) {
		for (int x = 0; x < width && !found; ++x) {
			if (monochrome.pixelIndex(x, y) == foregroundIndex) {
				top = y;
				found = true;
			}
		}
	}

	// find the bottom
	found = false;
	for (int y = height - 1; y >= 0 && !found; --y) {
		for (int x = width - 1; x >= 0 && !found; --x) {
			if (monochrome.pixelIndex(x, y) == foregroundIndex) {
				bottom = y;
				found = true;
			}
		}
	}

	// find the left
	found = false;
	for (int x = 0; x < width && !found; ++x) {
		for (int y = 0; y < height && !found; ++y) {
			if (monochrome.pixelIndex(x, y) == foregroundIndex) {
				left = x;
				found = true;
			}
		}
	}

	// find the right
	found = false;
	for (int x = width - 1; x >= 0 && !found; --x) {
		for (int y = height - 1; y >= 0 && !found; --y) {
			if (monochrome.pixelIndex(x, y) == foregroundIndex) {
				right = x;
				found = true;
			}
		}
	}

	int pieceWidth = right - left + 1;
	int pieceHeight = bottom - top + 1;

	// which dimension is greater?
	bool widthGreater = false;
	if (pieceWidth > pieceHeight) {
		widthGreater = true;
	}

	QImage cropped = monochrome.copy(left, top, pieceWidth, pieceHeight);
	QImage scaled = cropped.scaledToHeight(destHeight);

	RImage padded = RImage(destWidth, destHeight, QImage::Format_Mono);
	for (int x = 0; x < destWidth; ++x) {
		for (int y = 0; y < destHeight; ++y) {
			int index;

			if (x >= scaled.width()) {
				index = backgroundIndex;
			} else {
				index = scaled.pixelIndex(x, y);
			}

			padded.setPixel(x, y, index);
		}
	}

	this->setImage(padded);
}

void ImageWindow::on_actionColor_Equalize_triggered() {
	// rgb to hsi

	int width = image.width();
	int height = image.height();

	vector<vector<double> > hues(width, vector<double>(height));
	vector<vector<double> > saturations(width, vector<double>(height));
	vector<vector<double> > intensities(width, vector<double>(height));
	//vector<vector<double> > reds(width, vector<double>(height));
	//vector<vector<double> > greens(width, vector<double>(height));
	//vector<vector<double> > blues(width, vector<double>(height));

	//QColor color;
	QRgb rgb;

	double hue, saturation, intensity, red, green, blue,
			total_rgb, denominator;

	double *min_value;

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {

			// get color info
			rgb = image.pixel(x, y);

			red = (double)qRed(rgb);
			green = (double)qGreen(rgb);
			blue = (double)qBlue(rgb);

			// normalize rgb
			total_rgb = red + green + blue;

			red = red / total_rgb;
			green = green / total_rgb;
			blue = blue / total_rgb;

			denominator = 2.0 * sqrt(pow((red - green), 2) + ((red - blue) * (green - blue)));

			if (PRACTICALLY_ZERO(denominator)) {
				hue = SAMPLEDTONE_PI / 2.0;
				//hue = 0;
			} else {
				// compute hue
				hue = acos(((red - green) + (red - blue)) / denominator);

				if (blue > green) {
					hue = (2.0 * SAMPLEDTONE_PI) - hue;
				}
			}
			// h ∈ [0, π] for b ≤ g
			// h ∈ [π , 2π] for b > g

			min_value = &red;
			if (green < *min_value) {
				min_value = &green;
			}
			if (blue < *min_value) {
				min_value = &blue;
			}

			// compute saturation
			saturation = 1.0 - 3.0 * (*min_value); // s ∈ [0,1]

			// intensity
			intensity = total_rgb / (3.0 * 255.0); // i ∈ [0,1]

			hues[x][y] = hue;
			saturations[x][y] = saturation;
			intensities[x][y] = intensity;
		}
	}

	// histogram equalize intensities
	RImage intensityImage(width, height, QImage::Format_Indexed8);
	intensityImage.setGrayColorTable();

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			intensityImage.setPixel(x, y, (int)(255.0 * intensities[x][y]));
		}
	}
	intensityImage.equalize();

	/*
	ImageWindow *win = new ImageWindow(parentWidget());
	win->setImage(intensityImage);
	win->show();
	*/

	// scale intensities
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			intensity = (double)intensityImage.pixelIndex(x, y) / 255.0;
			intensities[x][y] = intensity;
		}
	}

	// increase saturation
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			saturation = saturations[x][y];
			saturation = saturation + 0.05;
			if (saturation > 1.0) {
				saturation = 1.0;
			}
			saturations[x][y] = saturation;
		}
	}

	/*
	// set hue (colorize)
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			//hue = hues[x][y];
			hue = 2.0 * SAMPLEDTONE_PI / 3.0;
			hues[x][y] = hue;
		}
	}
	*/

	// convert HSI to RGB
	double *a, *b, *c;

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {

			hue = hues[x][y];
			saturation = saturations[x][y];
			intensity = intensities[x][y];

			if (hue < (2.0 / 3.0 * SAMPLEDTONE_PI)) {
				a = &blue;
				b = &red;
				c = &green;
			} else if (hue < (4.0 / 3.0 * SAMPLEDTONE_PI)) {
				hue = hue - (2.0 * SAMPLEDTONE_PI / 3.0);
				a = &red;
				b = &green;
				c = &blue;
			} else {
				hue = hue - (4.0 / 3.0 * SAMPLEDTONE_PI);
				a = &green;
				b = &blue;
				c = &red;
			}

			*a = intensity * (1.0 - saturation);
			denominator = cos((SAMPLEDTONE_PI / 3.0) - hue);

			if (PRACTICALLY_ZERO(denominator)) {
				//*b = 0.0;
				*b = 0.5;
			} else {
				*b = intensity * ((1 + (saturation * cos(hue)) / denominator));
			}

			*c = (3.0 * intensity) - (*a + *b);

			red = red * 255.0;
			green = green * 255.0;
			blue = blue * 255.0;

			//reds[x][y] = red;
			//greens[x][y] = green;
			//blues[x][y] = blue;

			if (red > 255.0) {
				red = 255.0;
			} else if (red < 0.0) {
				red = 0.0;
			}
			if (green > 255.0) {
				green = 255.0;
			} else if (green < 0.0) {
				green = 0.0;
			}
			if (blue > 255.0) {
				blue = 255.0;
			} else if (blue < 0.0) {
				blue = 0.0;
			}

			image.setPixel(x, y, qRgb((int)red, (int)green, (int)blue));
		}
	}

	reloadPixmap();
}

void ImageWindow::on_actionPredictive_Encode_triggered() {
	image.predictiveEncode();
	reloadPixmap();
}

void ImageWindow::on_actionPredictive_Decode_triggered() {
	image.predictiveDecode();
	reloadPixmap();
}
