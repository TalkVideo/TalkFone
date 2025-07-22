/********************************************************************************
** Form generated from reading UI file 'accountdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACCOUNTDIALOG_H
#define UI_ACCOUNTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_AccountDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QLabel *serverLabel;
    QLineEdit *serverEdit;
    QPushButton *getAccountButton;
    QSpacerItem *spacerItem;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AccountDialog)
    {
        if (AccountDialog->objectName().isEmpty())
            AccountDialog->setObjectName(QString::fromUtf8("AccountDialog"));
        AccountDialog->setWindowModality(Qt::WindowModal);
        AccountDialog->resize(434, 160);
        gridLayout = new QGridLayout(AccountDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(AccountDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8("icons/accountlarge.png")));

        gridLayout->addWidget(label, 0, 0, 2, 1);

        usernameLabel = new QLabel(AccountDialog);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(usernameLabel->sizePolicy().hasHeightForWidth());
        usernameLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(usernameLabel, 0, 1, 1, 2);

        usernameEdit = new QLineEdit(AccountDialog);
        usernameEdit->setObjectName(QString::fromUtf8("usernameEdit"));

        gridLayout->addWidget(usernameEdit, 0, 3, 1, 2);

        passwordLabel = new QLabel(AccountDialog);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));
        sizePolicy.setHeightForWidth(passwordLabel->sizePolicy().hasHeightForWidth());
        passwordLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(passwordLabel, 1, 1, 1, 2);

        passwordEdit = new QLineEdit(AccountDialog);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordEdit, 1, 3, 1, 2);

        serverLabel = new QLabel(AccountDialog);
        serverLabel->setObjectName(QString::fromUtf8("serverLabel"));
        sizePolicy.setHeightForWidth(serverLabel->sizePolicy().hasHeightForWidth());
        serverLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(serverLabel, 2, 1, 1, 1);

        serverEdit = new QLineEdit(AccountDialog);
        serverEdit->setObjectName(QString::fromUtf8("serverEdit"));
        serverEdit->setEchoMode(QLineEdit::Normal);

        gridLayout->addWidget(serverEdit, 2, 3, 1, 2);

        getAccountButton = new QPushButton(AccountDialog);
        getAccountButton->setObjectName(QString::fromUtf8("getAccountButton"));

        gridLayout->addWidget(getAccountButton, 3, 0, 1, 2);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 3, 2, 1, 2);

        buttonBox = new QDialogButtonBox(AccountDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 4, 1, 1);

        QWidget::setTabOrder(usernameEdit, passwordEdit);
        QWidget::setTabOrder(passwordEdit, serverEdit);
        QWidget::setTabOrder(serverEdit, buttonBox);
        QWidget::setTabOrder(buttonBox, getAccountButton);

        retranslateUi(AccountDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AccountDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AccountDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AccountDialog);
    } // setupUi

    void retranslateUi(QDialog *AccountDialog)
    {
        AccountDialog->setWindowTitle(QApplication::translate("AccountDialog", "Account Data", 0));
        label->setText(QString());
        usernameLabel->setText(QApplication::translate("AccountDialog", "Username", 0));
        passwordLabel->setText(QApplication::translate("AccountDialog", "Password", 0));
        passwordEdit->setInputMask(QString());
        passwordEdit->setText(QString());
        serverLabel->setText(QApplication::translate("AccountDialog", "Server", 0));
        serverEdit->setInputMask(QString());
        serverEdit->setText(QString());
        getAccountButton->setText(QApplication::translate("AccountDialog", "Sign up and register for free", 0));
    } // retranslateUi

};

namespace Ui {
    class AccountDialog: public Ui_AccountDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCOUNTDIALOG_H
