#ifndef UNSHARPMASKDIALOG_H
#define UNSHARPMASKDIALOG_H

#include <QtGui/QDialog>
#include "ui_unsharpmaskdialog.h"

class UnsharpMaskDialog : public QDialog
{
    Q_OBJECT

public:
    UnsharpMaskDialog(QWidget *parent = 0);
    ~UnsharpMaskDialog();

private:
    Ui::UnsharpMaskDialogClass ui;

private slots:
	void on_applyFilterButton_pressed();

};

#endif // UNSHARPMASKDIALOG_H
