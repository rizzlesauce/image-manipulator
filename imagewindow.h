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

	void transformAndPlotImage();
    void setImage(RImage& img);
    void createHistogram(bool display = true, string fileName = "");
    void plotEqualizationTable(bool display = true, string fileName = "");
    void saveImage();

    RImage image;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ImageWindow *m_ui;

private slots:
    void on_actionSave_Equalization_Table_Plot_triggered();
    void on_actionShow_Equalization_Table_triggered();
    void on_actionShow_Histogram_triggered();
    void on_actionSave_Histogram_triggered();
    void on_actionSave_triggered();
    void equalizeImage();
    void reloadPixmap();
    void on_actionAverage_With_Images_triggered();
    void on_actionClone_triggered();
    void on_actionUniform_Average_triggered();
    void on_actionMedian_Filter_triggered();
    void on_actionUnsharp_Mask_triggered();
    void on_actionSobelX_triggered();
    void on_actionSobelY_triggered();
    void on_actionLaplacian_triggered();
    void on_actionGradientMagnitude_triggered();
    void on_actionFourier_Transform_triggered();
};

#endif // IMAGEWINDOW_H
