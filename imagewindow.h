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

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ImageWindow *m_ui;
    RImage image;

private slots:
    void equalizeImage();
    void createHistogram();
    void reloadPixmap();
};

#endif // IMAGEWINDOW_H
