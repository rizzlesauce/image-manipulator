#include "imagewindow.h"
#include "ui_imagewindow.h"
#include "averageimagesdialog.h"
#include "gnuplot_i.h"
#include "Debugger.h"
#include <QFileDialog>
#include "uniformaveragedialog.h"

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
