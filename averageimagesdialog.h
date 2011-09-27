#ifndef AVERAGEIMAGESDIALOG_H
#define AVERAGEIMAGESDIALOG_H

#include <QtGui/QDialog>
#include "ui_averageimagesdialog.h"

class AverageImagesDialog : public QDialog
{
    Q_OBJECT

public:
    AverageImagesDialog(QWidget *parent = 0);
    ~AverageImagesDialog();

private:
    Ui::AverageImagesDialogClass ui;
    QStringList fileNames;

private slots:
    void on_selectImagesButton_pressed();
    void on_averageButton_pressed();

};


#endif // AVERAGEIMAGESDIALOG_H
