/********************************************************************************
** Form generated from reading UI file 'reversi.ui'
**
** Created: Tue Feb 14 17:20:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REVERSI_H
#define UI_REVERSI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Reversi
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Reversi)
    {
        if (Reversi->objectName().isEmpty())
            Reversi->setObjectName(QString::fromUtf8("Reversi"));
        Reversi->resize(400, 300);
        menuBar = new QMenuBar(Reversi);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        Reversi->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Reversi);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Reversi->addToolBar(mainToolBar);
        centralWidget = new QWidget(Reversi);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        Reversi->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Reversi);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Reversi->setStatusBar(statusBar);

        retranslateUi(Reversi);

        QMetaObject::connectSlotsByName(Reversi);
    } // setupUi

    void retranslateUi(QMainWindow *Reversi)
    {
        Reversi->setWindowTitle(QApplication::translate("Reversi", "Reversi", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Reversi: public Ui_Reversi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REVERSI_H
