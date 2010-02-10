#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QtGui/QMainWindow>
#include "rimage.h"

namespace Ui {
    class ImageWindow;
}

class ImageWindow : public QMainWindow {
    Q_OBJECT
public:
    ImageWindow(QWidget *parent = 0);
    ~ImageWindow();

    void setImage(RImage& img);
    void createHistogram(bool display = true, string fileName = "");
    void plotEqualizationTable(bool display = true, string fileName = "");
    void saveImage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ImageWindow *m_ui;
    RImage image;

private slots:
    void on_actionSave_Equalization_Table_Plot_triggered();
    void on_actionShow_Equalization_Table_triggered();
    void on_actionShow_Histogram_triggered();
    void on_actionSave_Histogram_triggered();
    void on_actionSave_triggered();
    void equalizeImage();
    void reloadPixmap();
};

#endif // IMAGEWINDOW_H
