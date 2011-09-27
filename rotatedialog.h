#ifndef ROTATEDIALOG_H
#define ROTATEDIALOG_H

#include <QtGui/QDialog>
#include "ui_rotatedialog.h"

class RotateDialog : public QDialog
{
    Q_OBJECT

public:
    RotateDialog(QWidget *parent = 0);
    ~RotateDialog();

private:
    Ui::RotateDialogClass ui;

private slots:
    void on_rotateButton_pressed();
};

#endif // ROTATEDIALOG_H
