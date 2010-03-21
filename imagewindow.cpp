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
	int samples3d = 10;

	int width = image.width();
	int height = image.height();

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			xs.push_back((double)x);
			ys.push_back((double)y);
		}
	}

	// don't use this
	int size = width * height;

	fftw_complex *in, *out;
    fftw_plan p;
    double min, max;

    //int numValues = 256;
    string imagePath;
    bool display = true;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    p = fftw_plan_dft_2d(width, height, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

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

    // calculate the fourier transform
    fftw_execute(p);

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
    */

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

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
}

