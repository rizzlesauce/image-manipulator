#include "imagewindow.h"
#include "ui_imagewindow.h"

#include "gnuplot-cpp/gnuplot_i.hpp"
#include "Debugger.h"

ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::ImageWindow)
{
    m_ui->setupUi(this);

    //setCentralWidget(m_ui->label);

    connect(m_ui->actionShow_Histogram, SIGNAL(triggered()), this, SLOT(createHistogram()));
    connect(m_ui->actionEqualize, SIGNAL(triggered()), this, SLOT(equalizeImage()));

    Debugger::getInstance().print("imagewindow initialized\n");
}

ImageWindow::~ImageWindow()
{
    delete m_ui;
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

void ImageWindow::createHistogram() {
    vector<int> counts;
    image.getHistogram(counts);

    int numValues = counts.size();

    string imagePath = "histogram.ps";

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

            gplot.save(imagePath);
            gplot.set_style("histograms");
            //gplot.set_samples(300
            gplot.set_xrange(0, numValues);
            gplot.set_xlabel("Value");
            gplot.set_ylabel("Count");
            gplot.set_yrange(0, biggestCount);
            gplot.plot_x(counts);

            gplot.showonscreen();
            gplot.replot();

            // wait for input
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
            cin.get();

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
