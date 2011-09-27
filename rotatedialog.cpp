#include "rotatedialog.h"
#include "imagewindow.h"
#include "rimage.h"

RotateDialog::RotateDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

RotateDialog::~RotateDialog()
{

}

void RotateDialog::on_rotateButton_pressed() {
    ImageWindow *win = (ImageWindow*)parentWidget();

    RImage newImage = win->image.rotate(ui.degreesSpinBox->value());

    win->setImage(newImage);
}
