#ifndef SHRINKDIALOG_H
#define SHRINKDIALOG_H

#include <QtGui/QDialog>
#include "ui_shrinkdialog.h"

class ShrinkDialog : public QDialog
{
    Q_OBJECT

public:
    ShrinkDialog(QWidget *parent = 0);
    ~ShrinkDialog();

private:
    Ui::ShrinkDialogClass ui;

private slots:
	void on_shrinkButton_pressed();
};

#endif // SHRINKDIALOG_H
