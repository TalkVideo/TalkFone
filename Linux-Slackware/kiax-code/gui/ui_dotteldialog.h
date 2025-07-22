/********************************************************************************
** Form generated from reading UI file 'dotteldialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOTTELDIALOG_H
#define UI_DOTTELDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DotTelDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *infoTextLabel;
    QListWidget *numbersList;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *dialButton;

    void setupUi(QDialog *DotTelDialog)
    {
        if (DotTelDialog->objectName().isEmpty())
            DotTelDialog->setObjectName(QString::fromUtf8("DotTelDialog"));
        DotTelDialog->resize(364, 306);
        gridLayout = new QGridLayout(DotTelDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DotTelDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 3);

        infoTextLabel = new QLabel(DotTelDialog);
        infoTextLabel->setObjectName(QString::fromUtf8("infoTextLabel"));

        gridLayout->addWidget(infoTextLabel, 1, 0, 1, 3);

        numbersList = new QListWidget(DotTelDialog);
        numbersList->setObjectName(QString::fromUtf8("numbersList"));

        gridLayout->addWidget(numbersList, 2, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(121, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        cancelButton = new QPushButton(DotTelDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 3, 1, 1, 1);

        dialButton = new QPushButton(DotTelDialog);
        dialButton->setObjectName(QString::fromUtf8("dialButton"));

        gridLayout->addWidget(dialButton, 3, 2, 1, 1);


        retranslateUi(DotTelDialog);

        QMetaObject::connectSlotsByName(DotTelDialog);
    } // setupUi

    void retranslateUi(QDialog *DotTelDialog)
    {
        DotTelDialog->setWindowTitle(QApplication::translate("DotTelDialog", ".tel Dial Mode", 0));
        label->setText(QApplication::translate("DotTelDialog", ".tel Information", 0));
        infoTextLabel->setText(QApplication::translate("DotTelDialog", "Company information", 0));
        cancelButton->setText(QApplication::translate("DotTelDialog", "Cancel", 0));
        dialButton->setText(QApplication::translate("DotTelDialog", "Dial Selected Number", 0));
    } // retranslateUi

};

namespace Ui {
    class DotTelDialog: public Ui_DotTelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOTTELDIALOG_H
