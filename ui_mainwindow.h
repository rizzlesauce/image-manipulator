/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sat Feb 6 19:50:51 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDial>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *plainTextEdit;
    QPushButton *printSamplesButton;
    QLabel *imageLabel;
    QDial *dial;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuHi_Buddy;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        verticalLayout->addWidget(plainTextEdit);

        printSamplesButton = new QPushButton(centralWidget);
        printSamplesButton->setObjectName(QString::fromUtf8("printSamplesButton"));

        verticalLayout->addWidget(printSamplesButton);


        horizontalLayout->addLayout(verticalLayout);

        imageLabel = new QLabel(centralWidget);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));

        horizontalLayout->addWidget(imageLabel);

        dial = new QDial(centralWidget);
        dial->setObjectName(QString::fromUtf8("dial"));
        dial->setMaximum(100);
        dial->setWrapping(false);
        dial->setNotchTarget(5);
        dial->setNotchesVisible(true);

        horizontalLayout->addWidget(dial);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 25));
        menuHi_Buddy = new QMenu(menuBar);
        menuHi_Buddy->setObjectName(QString::fromUtf8("menuHi_Buddy"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuHi_Buddy->menuAction());
        menuHi_Buddy->addAction(actionOpen);
        menuHi_Buddy->addSeparator();
        menuHi_Buddy->addAction(actionQuit);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        printSamplesButton->setText(QApplication::translate("MainWindow", "Print Samples", 0, QApplication::UnicodeUTF8));
        imageLabel->setText(QString());
        menuHi_Buddy->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
