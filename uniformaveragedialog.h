#ifndef UNIFORMAVERAGEDIALOG_H
#define UNIFORMAVERAGEDIALOG_H

#include <QtGui/QDialog>
#include "ui_uniformaveragedialog.h"
#include <vector>

class UniformAverageDialog : public QDialog
{
    Q_OBJECT

public:
    UniformAverageDialog(QWidget *parent = 0);
    ~UniformAverageDialog();

private:
    Ui::UniformAverageDialogClass ui;

private slots:
    void on_applyFilterButton_pressed();

};


#endif // UNIFORMAVERAGEDIALOG_H
