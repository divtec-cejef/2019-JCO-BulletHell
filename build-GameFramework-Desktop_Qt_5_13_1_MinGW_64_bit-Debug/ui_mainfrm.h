/********************************************************************************
** Form generated from reading UI file 'mainfrm.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFRM_H
#define UI_MAINFRM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gameview.h"

QT_BEGIN_NAMESPACE

class Ui_MainFrm
{
public:
    QVBoxLayout *verticalLayout;
    GameView *grvGame;

    void setupUi(QWidget *MainFrm)
    {
        if (MainFrm->objectName().isEmpty())
            MainFrm->setObjectName(QString::fromUtf8("MainFrm"));
        MainFrm->resize(720, 900);
        verticalLayout = new QVBoxLayout(MainFrm);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        grvGame = new GameView(MainFrm);
        grvGame->setObjectName(QString::fromUtf8("grvGame"));

        verticalLayout->addWidget(grvGame);


        retranslateUi(MainFrm);

        QMetaObject::connectSlotsByName(MainFrm);
    } // setupUi

    void retranslateUi(QWidget *MainFrm)
    {
        MainFrm->setWindowTitle(QCoreApplication::translate("MainFrm", "BulletHell", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainFrm: public Ui_MainFrm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFRM_H
