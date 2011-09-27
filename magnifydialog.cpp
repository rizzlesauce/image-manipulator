#include "magnifydialog.h"
#include "imagewindow.h"
#include "rimage.h"

MagnifyDialog::MagnifyDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

MagnifyDialog::~MagnifyDialog()
{

}

void MagnifyDialog::on_magnifyButton_pressed() {
    ImageWindow *win = (ImageWindow*)parentWidget();

    ImageWindow *newWindow = new ImageWindow(win->parentWidget());
    RImage newImage = win->image.magnify(ui.factorSpinBox->value());

    newWindow->setImage(newImage);

    newWindow->show();
}
