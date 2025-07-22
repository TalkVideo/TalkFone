/********************************************************************************
** Form generated from reading UI file 'contactlistitemwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTLISTITEMWIDGET_H
#define UI_CONTACTLISTITEMWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactListItemWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *contactNameLabel;
    QSpacerItem *spacerItem;
    QPushButton *callButton;

    void setupUi(QWidget *ContactListItemWidget)
    {
        if (ContactListItemWidget->objectName().isEmpty())
            ContactListItemWidget->setObjectName(QString::fromUtf8("ContactListItemWidget"));
        ContactListItemWidget->resize(229, 71);
        gridLayout = new QGridLayout(ContactListItemWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        contactNameLabel = new QLabel(ContactListItemWidget);
        contactNameLabel->setObjectName(QString::fromUtf8("contactNameLabel"));

        gridLayout->addWidget(contactNameLabel, 0, 0, 1, 1);

        spacerItem = new QSpacerItem(171, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 0, 1, 1, 1);

        callButton = new QPushButton(ContactListItemWidget);
        callButton->setObjectName(QString::fromUtf8("callButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(callButton->sizePolicy().hasHeightForWidth());
        callButton->setSizePolicy(sizePolicy);
        const QIcon icon = QIcon(QString::fromUtf8("icons/callsmall.png"));
        callButton->setIcon(icon);

        gridLayout->addWidget(callButton, 0, 2, 1, 1);


        retranslateUi(ContactListItemWidget);

        QMetaObject::connectSlotsByName(ContactListItemWidget);
    } // setupUi

    void retranslateUi(QWidget *ContactListItemWidget)
    {
        ContactListItemWidget->setWindowTitle(QApplication::translate("ContactListItemWidget", "Form", 0));
        contactNameLabel->setText(QApplication::translate("ContactListItemWidget", "Contact Name", 0));
        callButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ContactListItemWidget: public Ui_ContactListItemWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTLISTITEMWIDGET_H
