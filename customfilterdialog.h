#ifndef CUSTOMFILTERDIALOG_H
#define CUSTOMFILTERDIALOG_H

#include <QtGui/QDialog>
#include "ui_customfilterdialog.h"

class CustomFilterDialog : public QDialog
{
    Q_OBJECT

public:
    CustomFilterDialog(QWidget *parent = 0);
    ~CustomFilterDialog();

private:
    Ui::CustomFilterDialogClass ui;
};

#endif // CUSTOMFILTERDIALOG_H
