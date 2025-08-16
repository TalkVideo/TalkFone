/********************************************************************************
** Form generated from reading UI file 'contactdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTDIALOG_H
#define UI_CONTACTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ContactDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QFrame *frame_2;
    QGridLayout *gridLayout1;
    QLabel *contactNameLabel;
    QLineEdit *contactNameEdit;
    QLabel *extensionLabel;
    QLineEdit *extensionEdit;
    QFrame *frame;
    QGridLayout *gridLayout2;
    QSpacerItem *spacerItem;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ContactDialog)
    {
        if (ContactDialog->objectName().isEmpty())
            ContactDialog->setObjectName(QString::fromUtf8("ContactDialog"));
        ContactDialog->resize(387, 171);
        gridLayout = new QGridLayout(ContactDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ContactDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8("icons/contactlarge.png")));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        frame_2 = new QFrame(ContactDialog);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout1 = new QGridLayout(frame_2);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        contactNameLabel = new QLabel(frame_2);
        contactNameLabel->setObjectName(QString::fromUtf8("contactNameLabel"));

        gridLayout1->addWidget(contactNameLabel, 0, 0, 1, 1);

        contactNameEdit = new QLineEdit(frame_2);
        contactNameEdit->setObjectName(QString::fromUtf8("contactNameEdit"));

        gridLayout1->addWidget(contactNameEdit, 0, 1, 1, 1);

        extensionLabel = new QLabel(frame_2);
        extensionLabel->setObjectName(QString::fromUtf8("extensionLabel"));

        gridLayout1->addWidget(extensionLabel, 1, 0, 1, 1);

        extensionEdit = new QLineEdit(frame_2);
        extensionEdit->setObjectName(QString::fromUtf8("extensionEdit"));

        gridLayout1->addWidget(extensionEdit, 1, 1, 1, 1);


        gridLayout->addWidget(frame_2, 0, 1, 1, 1);

        frame = new QFrame(ContactDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setLayoutDirection(Qt::LeftToRight);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout2 = new QGridLayout(frame);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        spacerItem = new QSpacerItem(151, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout2->addItem(spacerItem, 0, 0, 1, 1);

        saveButton = new QPushButton(frame);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        gridLayout2->addWidget(saveButton, 0, 1, 1, 1);

        cancelButton = new QPushButton(frame);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout2->addWidget(cancelButton, 0, 2, 1, 1);


        gridLayout->addWidget(frame, 1, 0, 1, 2);


        retranslateUi(ContactDialog);

        QMetaObject::connectSlotsByName(ContactDialog);
    } // setupUi

    void retranslateUi(QDialog *ContactDialog)
    {
        ContactDialog->setWindowTitle(QApplication::translate("ContactDialog", "Contact Data", 0));
        label->setText(QString());
        contactNameLabel->setText(QApplication::translate("ContactDialog", "Contact Name", 0));
        extensionLabel->setText(QApplication::translate("ContactDialog", "Number", 0));
        saveButton->setText(QApplication::translate("ContactDialog", "Save", 0));
        cancelButton->setText(QApplication::translate("ContactDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ContactDialog: public Ui_ContactDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTDIALOG_H
