#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QtGui/QMainWindow>
#include "rimage.h"
#include <cmath>

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
    void on_actionRemove_Interference_triggered();
    void on_actionReverse_Fourier_Transform_triggered();
    void on_actionUniform_Average_Frequency_triggered();
    void on_actionCrop_and_Resize_triggered();
    void on_actionColor_Equalize_triggered();
    void on_actionPredictive_Encode_triggered();
    void on_actionPredictive_Decode_triggered();
    void on_actionMagnify_triggered();
    void on_actionShrink_triggered();
    void on_actionRotate_triggered();
};

#endif // IMAGEWINDOW_H
