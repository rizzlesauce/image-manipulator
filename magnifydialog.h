#ifndef MAGNIFYDIALOG_H
#define MAGNIFYDIALOG_H

#include <QtGui/QDialog>
#include "ui_magnifydialog.h"

class MagnifyDialog : public QDialog
{
    Q_OBJECT

public:
    MagnifyDialog(QWidget *parent = 0);
    ~MagnifyDialog();

private:
    Ui::MagnifyDialogClass ui;

private slots:
	void on_magnifyButton_pressed();
};

#endif // MAGNIFYDIALOG_H
