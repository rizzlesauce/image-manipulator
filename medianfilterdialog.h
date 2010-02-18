#ifndef MEDIANFILTERDIALOG_H
#define MEDIANFILTERDIALOG_H

#include <QtGui/QDialog>
#include "ui_medianfilterdialog.h"

class MedianFilterDialog : public QDialog
{
    Q_OBJECT

public:
    MedianFilterDialog(QWidget *parent = 0);
    ~MedianFilterDialog();

private:
    Ui::MedianFilterDialogClass ui;

private slots:
	void on_applyFilterButton_pressed();

};

#endif // MEDIANFILTERDIALOG_H
