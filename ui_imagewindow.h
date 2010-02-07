/********************************************************************************
** Form generated from reading ui file 'imagewindow.ui'
**
** Created: Sat Feb 6 11:36:43 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IMAGEWINDOW_H
#define UI_IMAGEWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageWindow
{
public:
    QAction *actionSave;
    QAction *actionClose;
    QAction *actionEqualize;
    QAction *actionShow_Histogram;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuImage;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ImageWindow)
    {
        if (ImageWindow->objectName().isEmpty())
            ImageWindow->setObjectName(QString::fromUtf8("ImageWindow"));
        ImageWindow->resize(219, 82);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ImageWindow->sizePolicy().hasHeightForWidth());
        ImageWindow->setSizePolicy(sizePolicy);
        ImageWindow->setDocumentMode(false);
        actionSave = new QAction(ImageWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionClose = new QAction(ImageWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionEqualize = new QAction(ImageWindow);
        actionEqualize->setObjectName(QString::fromUtf8("actionEqualize"));
        actionShow_Histogram = new QAction(ImageWindow);
        actionShow_Histogram->setObjectName(QString::fromUtf8("actionShow_Histogram"));
        centralwidget = new QWidget(ImageWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        ImageWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ImageWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 219, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuImage = new QMenu(menubar);
        menuImage->setObjectName(QString::fromUtf8("menuImage"));
        ImageWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ImageWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(statusbar->sizePolicy().hasHeightForWidth());
        statusbar->setSizePolicy(sizePolicy1);
        ImageWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuImage->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuImage->addAction(actionEqualize);
        menuImage->addAction(actionShow_Histogram);

        retranslateUi(ImageWindow);
        QObject::connect(actionClose, SIGNAL(triggered()), ImageWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(ImageWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ImageWindow)
    {
        ImageWindow->setWindowTitle(QApplication::translate("ImageWindow", "Image Window", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("ImageWindow", "Save", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("ImageWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionEqualize->setText(QApplication::translate("ImageWindow", "Equalize", 0, QApplication::UnicodeUTF8));
        actionShow_Histogram->setText(QApplication::translate("ImageWindow", "Show Histogram", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        menuFile->setTitle(QApplication::translate("ImageWindow", "File", 0, QApplication::UnicodeUTF8));
        menuImage->setTitle(QApplication::translate("ImageWindow", "Image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImageWindow: public Ui_ImageWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEWINDOW_H
