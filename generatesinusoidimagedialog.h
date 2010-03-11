#ifndef GENERATESINUSOIDIMAGEDIALOG_H
#define GENERATESINUSOIDIMAGEDIALOG_H

#include <QtGui/QDialog>
#include "ui_generatesinusoidimagedialog.h"

class GenerateSinusoidImageDialog : public QDialog
{
    Q_OBJECT

public:
    GenerateSinusoidImageDialog(QWidget *parent = 0);
    ~GenerateSinusoidImageDialog();

private:
    Ui::GenerateSinusoidImageDialogClass ui;

private slots:
	void on_generateButton_pressed();
};

#endif // GENERATESINUSOIDIMAGEDIALOG_H
