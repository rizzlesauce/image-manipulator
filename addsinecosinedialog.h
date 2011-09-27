#ifndef ADDSINECOSINEDIALOG_H
#define ADDSINECOSINEDIALOG_H

#include <QtGui/QDialog>
#include "ui_addsinecosinedialog.h"

class AddSineCosineDialog : public QDialog
{
    Q_OBJECT

public:
    AddSineCosineDialog(QWidget *parent = 0);
    ~AddSineCosineDialog();

private:
    Ui::AddSineCosineDialogClass ui;

private slots:
    void on_okButton_pressed();
};

#endif // ADDSINECOSINEDIALOG_H
