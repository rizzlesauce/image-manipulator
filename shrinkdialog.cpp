#include "shrinkdialog.h"
#include "imagewindow.h"
#include "rimage.h"

ShrinkDialog::ShrinkDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

ShrinkDialog::~ShrinkDialog()
{

}

void ShrinkDialog::on_shrinkButton_pressed() {
    ImageWindow *win = (ImageWindow*)parentWidget();

    ImageWindow *newWindow = new ImageWindow(win->parentWidget());
    RImage newImage = win->image.shrink(ui.shrinkFactorSpinBox->value());

    newWindow->setImage(newImage);

    newWindow->show();
}
