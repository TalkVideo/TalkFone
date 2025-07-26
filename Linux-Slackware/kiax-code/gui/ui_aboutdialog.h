/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *productLabel;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *logoLabel;
    QLabel *urlLabel;
    QLabel *licenseLabel;
    QSpacerItem *spacerItem;
    QPushButton *okButton;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->setWindowModality(Qt::WindowModal);
        AboutDialog->resize(278, 252);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        AboutDialog->setPalette(palette);
        AboutDialog->setStyleSheet(QString::fromUtf8("background: url(./icons/about.png);"));
        gridLayout = new QGridLayout(AboutDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        productLabel = new QLabel(AboutDialog);
        productLabel->setObjectName(QString::fromUtf8("productLabel"));
        QFont font;
        font.setPointSize(13);
        productLabel->setFont(font);
        productLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(productLabel, 0, 0, 1, 2);

        frame = new QFrame(AboutDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        logoLabel = new QLabel(frame);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(logoLabel->sizePolicy().hasHeightForWidth());
        logoLabel->setSizePolicy(sizePolicy);
        logoLabel->setFrameShape(QFrame::StyledPanel);
        logoLabel->setPixmap(QPixmap(QString::fromUtf8("icons/logo.png")));

        horizontalLayout->addWidget(logoLabel);


        gridLayout->addWidget(frame, 1, 0, 1, 2);

        urlLabel = new QLabel(AboutDialog);
        urlLabel->setObjectName(QString::fromUtf8("urlLabel"));
        urlLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(urlLabel, 2, 0, 1, 2);

        licenseLabel = new QLabel(AboutDialog);
        licenseLabel->setObjectName(QString::fromUtf8("licenseLabel"));
        licenseLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(licenseLabel, 3, 0, 1, 2);

        spacerItem = new QSpacerItem(81, 25, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 4, 0, 1, 1);

        okButton = new QPushButton(AboutDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 4, 1, 1, 1);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About", 0));
        productLabel->setText(QApplication::translate("AboutDialog", "Product Name Ver. 1.0", 0));
        logoLabel->setText(QString());
        urlLabel->setText(QApplication::translate("AboutDialog", "URL", 0));
        licenseLabel->setText(QApplication::translate("AboutDialog", "License", 0));
        okButton->setText(QApplication::translate("AboutDialog", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
