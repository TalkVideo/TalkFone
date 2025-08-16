/********************************************************************************
** Form generated from reading UI file 'loginscreen.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINSCREEN_H
#define UI_LOGINSCREEN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_LoginScreen
{
public:
    QGridLayout *gridLayout_3;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QLabel *logoLabel;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QFrame *frame_3;
    QGridLayout *gridLayout_2;
    QLabel *loginStatusLabel;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QPushButton *getAccountButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QPushButton *loginButton;

    void setupUi(QFrame *LoginScreen)
    {
        if (LoginScreen->objectName().isEmpty())
            LoginScreen->setObjectName(QString::fromUtf8("LoginScreen"));
        LoginScreen->resize(248, 506);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LoginScreen->sizePolicy().hasHeightForWidth());
        LoginScreen->setSizePolicy(sizePolicy);
        LoginScreen->setStyleSheet(QString::fromUtf8("background-image:url(icons/dialer/fondo.png);"));
        LoginScreen->setFrameShape(QFrame::NoFrame);
        LoginScreen->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(LoginScreen);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        frame_2 = new QFrame(LoginScreen);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        logoLabel = new QLabel(frame_2);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(logoLabel->sizePolicy().hasHeightForWidth());
        logoLabel->setSizePolicy(sizePolicy1);
        logoLabel->setPixmap(QPixmap(QString::fromUtf8("icons/logo.png")));
        logoLabel->setScaledContents(false);

        horizontalLayout->addWidget(logoLabel);


        gridLayout_3->addWidget(frame_2, 0, 0, 1, 3);

        frame = new QFrame(LoginScreen);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy2);
        frame->setAutoFillBackground(false);
        frame->setStyleSheet(QString::fromUtf8("background-image:none;\n"
"background:transparent;\n"
""));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        usernameLabel = new QLabel(frame);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));

        gridLayout->addWidget(usernameLabel, 0, 0, 1, 1);

        usernameLineEdit = new QLineEdit(frame);
        usernameLineEdit->setObjectName(QString::fromUtf8("usernameLineEdit"));

        gridLayout->addWidget(usernameLineEdit, 1, 0, 1, 1);

        passwordLabel = new QLabel(frame);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));

        gridLayout->addWidget(passwordLabel, 2, 0, 1, 1);

        passwordLineEdit = new QLineEdit(frame);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordLineEdit, 3, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 4, 0, 1, 1);


        gridLayout_3->addWidget(frame, 1, 0, 1, 3);

        verticalSpacer_4 = new QSpacerItem(85, 45, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_4, 3, 1, 1, 1);

        frame_3 = new QFrame(LoginScreen);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setAutoFillBackground(false);
        frame_3->setStyleSheet(QString::fromUtf8("background:transparent;"));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        loginStatusLabel = new QLabel(frame_3);
        loginStatusLabel->setObjectName(QString::fromUtf8("loginStatusLabel"));
        QFont font;
        font.setPointSize(12);
        loginStatusLabel->setFont(font);
        loginStatusLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(loginStatusLabel, 0, 0, 1, 1);


        gridLayout_3->addWidget(frame_3, 4, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 5, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(47, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 6, 0, 1, 1);

        getAccountButton = new QPushButton(LoginScreen);
        getAccountButton->setObjectName(QString::fromUtf8("getAccountButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(getAccountButton->sizePolicy().hasHeightForWidth());
        getAccountButton->setSizePolicy(sizePolicy3);
        getAccountButton->setMinimumSize(QSize(0, 50));
        getAccountButton->setAutoFillBackground(false);
        getAccountButton->setStyleSheet(QString::fromUtf8("background:transparent;"));
        getAccountButton->setFlat(true);

        gridLayout_3->addWidget(getAccountButton, 6, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(47, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 6, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 7, 1, 1, 1);

        loginButton = new QPushButton(LoginScreen);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setMinimumSize(QSize(91, 91));
        loginButton->setMaximumSize(QSize(91, 91));
        loginButton->setAutoFillBackground(false);
        loginButton->setStyleSheet(QString::fromUtf8("QButton : {\n"
"   background:transparent\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("icons/dialer/login.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8("icons/dialer/loginover.png"), QSize(), QIcon::Normal, QIcon::On);
        loginButton->setIcon(icon);
        loginButton->setIconSize(QSize(91, 91));
        loginButton->setFlat(true);

        gridLayout_3->addWidget(loginButton, 2, 1, 1, 1);


        retranslateUi(LoginScreen);

        QMetaObject::connectSlotsByName(LoginScreen);
    } // setupUi

    void retranslateUi(QFrame *LoginScreen)
    {
        LoginScreen->setWindowTitle(QApplication::translate("LoginScreen", "Login", 0));
        logoLabel->setText(QString());
        usernameLabel->setText(QApplication::translate("LoginScreen", "Username", 0));
        passwordLabel->setText(QApplication::translate("LoginScreen", "Password", 0));
        loginStatusLabel->setText(QApplication::translate("LoginScreen", "Logging In", 0));
        getAccountButton->setText(QApplication::translate("LoginScreen", "Sign Up", 0));
        loginButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginScreen: public Ui_LoginScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINSCREEN_H
