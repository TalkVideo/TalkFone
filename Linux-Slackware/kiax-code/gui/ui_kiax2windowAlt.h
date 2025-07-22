/********************************************************************************
** Form generated from reading UI file 'kiax2windowAlt.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KIAX2WINDOWALT_H
#define UI_KIAX2WINDOWALT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionExit;
    QAction *actionAdd_Contact;
    QAction *actionInvite_Friend;
    QAction *actionEdit_Contact;
    QAction *actionDelete_Contact;
    QAction *actionCall;
    QAction *actionHangup;
    QAction *actionHide;
    QAction *actionShow;
    QAction *actionSettings;
    QAction *actionRegister;
    QAction *actionAccount;
    QAction *actionPassword;
    QWidget *centralwidget;
    QGridLayout *mainLayout;
    QFrame *mainWidgetFrame;
    QGridLayout *mainFrameLayout;
    QFrame *mainFrame;
    QGridLayout *gridLayout;
    QFrame *searchFrame;
    QGridLayout *_2;
    QComboBox *searchComboBox;
    QPushButton *listSwitchButton;
    QListWidget *contactsList;
    QFrame *dialpadFrame;
    QGridLayout *gridLayout_2;
    QLineEdit *numberEdit;
    QFrame *dtmfButtonsFrame;
    QGridLayout *gridLayout_5;
    QFrame *frame;
    QGridLayout *gridLayout_4;
    QPushButton *dtmf1Button;
    QPushButton *dtmf2Button;
    QPushButton *dtmf3Button;
    QPushButton *dtmf4Button;
    QPushButton *dtmf5Button;
    QPushButton *dtmf6Button;
    QPushButton *dtmf7Button;
    QPushButton *dtmf8Button;
    QPushButton *dtmf9Button;
    QPushButton *dtmfStarButton;
    QPushButton *dtmf0Button;
    QPushButton *dtmfHashButton;
    QPushButton *dialButton;
    QFrame *dialPadButtonsFrame;
    QGridLayout *dialpadButtonLayout;
    QPushButton *dialpadButton;
    QFrame *onlineStatusFrame;
    QGridLayout *statusLayout;
    QPushButton *statusButton;
    QSpacerItem *onlineStatusSpacer;
    QPushButton *balanceLabel;
    QMenuBar *menubar;
    QMenu *menuService;
    QMenu *menuAccount;
    QMenu *menuHelp;
    QMenu *menuContacts;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(240, 562);
        MainWindow->setIconSize(QSize(24, 24));
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("icons/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon);
        actionAbout->setMenuRole(QAction::TextHeuristicRole);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icons/bconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon1);
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("icons/bdisconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon2);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon3);
        actionAdd_Contact = new QAction(MainWindow);
        actionAdd_Contact->setObjectName(QString::fromUtf8("actionAdd_Contact"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("icons/contact.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Contact->setIcon(icon4);
        actionInvite_Friend = new QAction(MainWindow);
        actionInvite_Friend->setObjectName(QString::fromUtf8("actionInvite_Friend"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("icons/invite.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInvite_Friend->setIcon(icon5);
        actionEdit_Contact = new QAction(MainWindow);
        actionEdit_Contact->setObjectName(QString::fromUtf8("actionEdit_Contact"));
        actionEdit_Contact->setIcon(icon4);
        actionDelete_Contact = new QAction(MainWindow);
        actionDelete_Contact->setObjectName(QString::fromUtf8("actionDelete_Contact"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("icons/deletecontact.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete_Contact->setIcon(icon6);
        actionCall = new QAction(MainWindow);
        actionCall->setObjectName(QString::fromUtf8("actionCall"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("icons/call.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCall->setIcon(icon7);
        actionHangup = new QAction(MainWindow);
        actionHangup->setObjectName(QString::fromUtf8("actionHangup"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8("icons/hangupsmall.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHangup->setIcon(icon8);
        actionHide = new QAction(MainWindow);
        actionHide->setObjectName(QString::fromUtf8("actionHide"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8("icons/hide.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHide->setIcon(icon9);
        actionShow = new QAction(MainWindow);
        actionShow->setObjectName(QString::fromUtf8("actionShow"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8("icons/show.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShow->setIcon(icon10);
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8("icons/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon11);
        actionRegister = new QAction(MainWindow);
        actionRegister->setObjectName(QString::fromUtf8("actionRegister"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8("icons/newaccount.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRegister->setIcon(icon12);
        actionAccount = new QAction(MainWindow);
        actionAccount->setObjectName(QString::fromUtf8("actionAccount"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8("icons/account.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAccount->setIcon(icon13);
        actionPassword = new QAction(MainWindow);
        actionPassword->setObjectName(QString::fromUtf8("actionPassword"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8("icons/password.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPassword->setIcon(icon14);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setGeometry(QRect(0, 56, 240, 506));
        mainLayout = new QGridLayout(centralwidget);
        mainLayout->setSpacing(1);
        mainLayout->setContentsMargins(5, 5, 5, 5);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainLayout->setHorizontalSpacing(5);
        mainLayout->setVerticalSpacing(56);
        mainWidgetFrame = new QFrame(centralwidget);
        mainWidgetFrame->setObjectName(QString::fromUtf8("mainWidgetFrame"));
        mainWidgetFrame->setFrameShape(QFrame::NoFrame);
        mainWidgetFrame->setFrameShadow(QFrame::Raised);
        mainFrameLayout = new QGridLayout(mainWidgetFrame);
        mainFrameLayout->setSpacing(0);
        mainFrameLayout->setContentsMargins(0, 0, 0, 0);
        mainFrameLayout->setObjectName(QString::fromUtf8("mainFrameLayout"));
        mainFrame = new QFrame(mainWidgetFrame);
        mainFrame->setObjectName(QString::fromUtf8("mainFrame"));
        mainFrame->setEnabled(true);
        mainFrame->setFrameShape(QFrame::NoFrame);
        mainFrame->setFrameShadow(QFrame::Sunken);
        mainFrame->setLineWidth(-1);
        mainFrame->setMidLineWidth(-1);
        gridLayout = new QGridLayout(mainFrame);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        gridLayout->setVerticalSpacing(4);
        searchFrame = new QFrame(mainFrame);
        searchFrame->setObjectName(QString::fromUtf8("searchFrame"));
        searchFrame->setFrameShape(QFrame::NoFrame);
        searchFrame->setFrameShadow(QFrame::Raised);
        searchFrame->setLineWidth(0);
        _2 = new QGridLayout(searchFrame);
        _2->setSpacing(1);
        _2->setContentsMargins(0, 0, 0, 0);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setHorizontalSpacing(5);
        searchComboBox = new QComboBox(searchFrame);
        searchComboBox->setObjectName(QString::fromUtf8("searchComboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(searchComboBox->sizePolicy().hasHeightForWidth());
        searchComboBox->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(133, 134, 140, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        searchComboBox->setPalette(palette);
        searchComboBox->setEditable(true);

        _2->addWidget(searchComboBox, 0, 0, 1, 1);

        listSwitchButton = new QPushButton(searchFrame);
        listSwitchButton->setObjectName(QString::fromUtf8("listSwitchButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listSwitchButton->sizePolicy().hasHeightForWidth());
        listSwitchButton->setSizePolicy(sizePolicy1);
        QIcon icon15;
        icon15.addFile(QString::fromUtf8("icons/calls.png"), QSize(), QIcon::Normal, QIcon::Off);
        listSwitchButton->setIcon(icon15);
        listSwitchButton->setCheckable(true);

        _2->addWidget(listSwitchButton, 0, 1, 1, 1);


        gridLayout->addWidget(searchFrame, 0, 0, 1, 1);

        contactsList = new QListWidget(mainFrame);
        contactsList->setObjectName(QString::fromUtf8("contactsList"));

        gridLayout->addWidget(contactsList, 1, 0, 1, 1);

        dialpadFrame = new QFrame(mainFrame);
        dialpadFrame->setObjectName(QString::fromUtf8("dialpadFrame"));
        dialpadFrame->setFrameShape(QFrame::Box);
        dialpadFrame->setFrameShadow(QFrame::Raised);
        dialpadFrame->setLineWidth(1);
        gridLayout_2 = new QGridLayout(dialpadFrame);
        gridLayout_2->setSpacing(1);
        gridLayout_2->setContentsMargins(9, 9, 9, 9);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        numberEdit = new QLineEdit(dialpadFrame);
        numberEdit->setObjectName(QString::fromUtf8("numberEdit"));
        QPalette palette1;
        QBrush brush2(QColor(153, 149, 154, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        numberEdit->setPalette(palette1);

        gridLayout_2->addWidget(numberEdit, 0, 0, 1, 2);

        dtmfButtonsFrame = new QFrame(dialpadFrame);
        dtmfButtonsFrame->setObjectName(QString::fromUtf8("dtmfButtonsFrame"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dtmfButtonsFrame->sizePolicy().hasHeightForWidth());
        dtmfButtonsFrame->setSizePolicy(sizePolicy2);
        dtmfButtonsFrame->setMinimumSize(QSize(0, 200));
        dtmfButtonsFrame->setFrameShape(QFrame::NoFrame);
        dtmfButtonsFrame->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(dtmfButtonsFrame);
        gridLayout_5->setSpacing(1);
        gridLayout_5->setContentsMargins(9, 9, 9, 9);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(-1, 0, -1, 0);
        frame = new QFrame(dtmfButtonsFrame);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMinimumSize(QSize(0, 180));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame);
        gridLayout_4->setSpacing(1);
        gridLayout_4->setContentsMargins(9, 9, 9, 9);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setHorizontalSpacing(10);
        gridLayout_4->setVerticalSpacing(6);
        gridLayout_4->setContentsMargins(-1, 0, -1, 0);
        dtmf1Button = new QPushButton(frame);
        dtmf1Button->setObjectName(QString::fromUtf8("dtmf1Button"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(dtmf1Button->sizePolicy().hasHeightForWidth());
        dtmf1Button->setSizePolicy(sizePolicy3);
        dtmf1Button->setMinimumSize(QSize(40, 40));
        dtmf1Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf1Button, 0, 0, 1, 1);

        dtmf2Button = new QPushButton(frame);
        dtmf2Button->setObjectName(QString::fromUtf8("dtmf2Button"));
        sizePolicy3.setHeightForWidth(dtmf2Button->sizePolicy().hasHeightForWidth());
        dtmf2Button->setSizePolicy(sizePolicy3);
        dtmf2Button->setMinimumSize(QSize(40, 40));
        dtmf2Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf2Button, 0, 1, 1, 1);

        dtmf3Button = new QPushButton(frame);
        dtmf3Button->setObjectName(QString::fromUtf8("dtmf3Button"));
        sizePolicy3.setHeightForWidth(dtmf3Button->sizePolicy().hasHeightForWidth());
        dtmf3Button->setSizePolicy(sizePolicy3);
        dtmf3Button->setMinimumSize(QSize(40, 40));
        dtmf3Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf3Button, 0, 2, 1, 1);

        dtmf4Button = new QPushButton(frame);
        dtmf4Button->setObjectName(QString::fromUtf8("dtmf4Button"));
        dtmf4Button->setMinimumSize(QSize(40, 40));
        dtmf4Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf4Button, 1, 0, 1, 1);

        dtmf5Button = new QPushButton(frame);
        dtmf5Button->setObjectName(QString::fromUtf8("dtmf5Button"));
        dtmf5Button->setMinimumSize(QSize(40, 40));
        dtmf5Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf5Button, 1, 1, 1, 1);

        dtmf6Button = new QPushButton(frame);
        dtmf6Button->setObjectName(QString::fromUtf8("dtmf6Button"));
        dtmf6Button->setMinimumSize(QSize(40, 40));
        dtmf6Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf6Button, 1, 2, 1, 1);

        dtmf7Button = new QPushButton(frame);
        dtmf7Button->setObjectName(QString::fromUtf8("dtmf7Button"));
        dtmf7Button->setMinimumSize(QSize(40, 40));
        dtmf7Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf7Button, 2, 0, 1, 1);

        dtmf8Button = new QPushButton(frame);
        dtmf8Button->setObjectName(QString::fromUtf8("dtmf8Button"));
        dtmf8Button->setMinimumSize(QSize(40, 40));
        dtmf8Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf8Button, 2, 1, 1, 1);

        dtmf9Button = new QPushButton(frame);
        dtmf9Button->setObjectName(QString::fromUtf8("dtmf9Button"));
        dtmf9Button->setMinimumSize(QSize(40, 40));
        dtmf9Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf9Button, 2, 2, 1, 1);

        dtmfStarButton = new QPushButton(frame);
        dtmfStarButton->setObjectName(QString::fromUtf8("dtmfStarButton"));
        dtmfStarButton->setMinimumSize(QSize(40, 40));
        dtmfStarButton->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmfStarButton, 3, 0, 1, 1);

        dtmf0Button = new QPushButton(frame);
        dtmf0Button->setObjectName(QString::fromUtf8("dtmf0Button"));
        dtmf0Button->setMinimumSize(QSize(40, 40));
        dtmf0Button->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmf0Button, 3, 1, 1, 1);

        dtmfHashButton = new QPushButton(frame);
        dtmfHashButton->setObjectName(QString::fromUtf8("dtmfHashButton"));
        dtmfHashButton->setMinimumSize(QSize(40, 40));
        dtmfHashButton->setMaximumSize(QSize(40, 40));

        gridLayout_4->addWidget(dtmfHashButton, 3, 2, 1, 1);


        gridLayout_5->addWidget(frame, 0, 0, 1, 1);


        gridLayout_2->addWidget(dtmfButtonsFrame, 2, 0, 1, 2);

        dialButton = new QPushButton(dialpadFrame);
        dialButton->setObjectName(QString::fromUtf8("dialButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(dialButton->sizePolicy().hasHeightForWidth());
        dialButton->setSizePolicy(sizePolicy4);
        dialButton->setIcon(icon7);

        gridLayout_2->addWidget(dialButton, 1, 0, 1, 2);


        gridLayout->addWidget(dialpadFrame, 2, 0, 1, 1);


        mainFrameLayout->addWidget(mainFrame, 0, 0, 1, 1);

        dialPadButtonsFrame = new QFrame(mainWidgetFrame);
        dialPadButtonsFrame->setObjectName(QString::fromUtf8("dialPadButtonsFrame"));
        dialPadButtonsFrame->setFrameShape(QFrame::NoFrame);
        dialPadButtonsFrame->setFrameShadow(QFrame::Plain);
        dialpadButtonLayout = new QGridLayout(dialPadButtonsFrame);
        dialpadButtonLayout->setSpacing(1);
        dialpadButtonLayout->setContentsMargins(9, 9, 9, 9);
        dialpadButtonLayout->setObjectName(QString::fromUtf8("dialpadButtonLayout"));
        dialpadButtonLayout->setContentsMargins(0, 3, 0, 2);
        dialpadButton = new QPushButton(dialPadButtonsFrame);
        dialpadButton->setObjectName(QString::fromUtf8("dialpadButton"));
        sizePolicy.setHeightForWidth(dialpadButton->sizePolicy().hasHeightForWidth());
        dialpadButton->setSizePolicy(sizePolicy);
        dialpadButton->setCheckable(true);

        dialpadButtonLayout->addWidget(dialpadButton, 0, 0, 1, 1);


        mainFrameLayout->addWidget(dialPadButtonsFrame, 1, 0, 1, 1);

        onlineStatusFrame = new QFrame(mainWidgetFrame);
        onlineStatusFrame->setObjectName(QString::fromUtf8("onlineStatusFrame"));
        sizePolicy4.setHeightForWidth(onlineStatusFrame->sizePolicy().hasHeightForWidth());
        onlineStatusFrame->setSizePolicy(sizePolicy4);
        onlineStatusFrame->setFrameShape(QFrame::NoFrame);
        onlineStatusFrame->setFrameShadow(QFrame::Raised);
        onlineStatusFrame->setLineWidth(0);
        statusLayout = new QGridLayout(onlineStatusFrame);
        statusLayout->setSpacing(0);
        statusLayout->setContentsMargins(0, 0, 0, 0);
        statusLayout->setObjectName(QString::fromUtf8("statusLayout"));
        statusButton = new QPushButton(onlineStatusFrame);
        statusButton->setObjectName(QString::fromUtf8("statusButton"));
        sizePolicy1.setHeightForWidth(statusButton->sizePolicy().hasHeightForWidth());
        statusButton->setSizePolicy(sizePolicy1);
        statusButton->setMaximumSize(QSize(120, 16777215));
        QFont font;
        font.setPointSize(10);
        statusButton->setFont(font);
        statusButton->setMouseTracking(true);
        statusButton->setContextMenuPolicy(Qt::NoContextMenu);
        statusButton->setLayoutDirection(Qt::LeftToRight);
        QIcon icon16;
        icon16.addFile(QString::fromUtf8("icons/disconnected.png"), QSize(), QIcon::Normal, QIcon::Off);
        statusButton->setIcon(icon16);
        statusButton->setFlat(true);

        statusLayout->addWidget(statusButton, 0, 0, 1, 1);

        onlineStatusSpacer = new QSpacerItem(103, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        statusLayout->addItem(onlineStatusSpacer, 0, 1, 1, 1);

        balanceLabel = new QPushButton(onlineStatusFrame);
        balanceLabel->setObjectName(QString::fromUtf8("balanceLabel"));
        sizePolicy4.setHeightForWidth(balanceLabel->sizePolicy().hasHeightForWidth());
        balanceLabel->setSizePolicy(sizePolicy4);
        balanceLabel->setMaximumSize(QSize(130, 16777215));
        QFont font1;
        font1.setPointSize(10);
        font1.setKerning(true);
        balanceLabel->setFont(font1);
        balanceLabel->setLayoutDirection(Qt::LeftToRight);
        balanceLabel->setFlat(true);

        statusLayout->addWidget(balanceLabel, 0, 2, 1, 1);


        mainFrameLayout->addWidget(onlineStatusFrame, 2, 0, 1, 1);


        mainLayout->addWidget(mainWidgetFrame, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 240, 22));
        menuService = new QMenu(menubar);
        menuService->setObjectName(QString::fromUtf8("menuService"));
        menuAccount = new QMenu(menuService);
        menuAccount->setObjectName(QString::fromUtf8("menuAccount"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8("icons/accountmenu.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuAccount->setIcon(icon17);
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuContacts = new QMenu(menubar);
        menuContacts->setObjectName(QString::fromUtf8("menuContacts"));
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setGeometry(QRect(0, 22, 240, 34));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuService->menuAction());
        menubar->addAction(menuContacts->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuService->addAction(menuAccount->menuAction());
        menuService->addSeparator();
        menuService->addAction(actionConnect);
        menuService->addAction(actionDisconnect);
        menuService->addSeparator();
        menuService->addAction(actionSettings);
        menuService->addSeparator();
        menuService->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuContacts->addAction(actionAdd_Contact);
        toolBar->addAction(actionConnect);
        toolBar->addAction(actionDisconnect);
        toolBar->addAction(actionAdd_Contact);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Softphone", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionConnect->setText(QApplication::translate("MainWindow", "Connect", 0));
        actionConnect->setIconText(QApplication::translate("MainWindow", "Connect", 0));
        actionDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionAdd_Contact->setText(QApplication::translate("MainWindow", "Add Contact", 0));
        actionInvite_Friend->setText(QApplication::translate("MainWindow", "Invite Friend", 0));
        actionEdit_Contact->setText(QApplication::translate("MainWindow", "Edit Contact", 0));
        actionDelete_Contact->setText(QApplication::translate("MainWindow", "Delete Contact", 0));
        actionCall->setText(QApplication::translate("MainWindow", "Call", 0));
        actionHangup->setText(QApplication::translate("MainWindow", "Hangup", 0));
        actionHide->setText(QApplication::translate("MainWindow", "Hide", 0));
        actionShow->setText(QApplication::translate("MainWindow", "Show", 0));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", 0));
        actionRegister->setText(QApplication::translate("MainWindow", "Sign up and register for free", 0));
        actionAccount->setText(QApplication::translate("MainWindow", "Configure Account", 0));
        actionPassword->setText(QApplication::translate("MainWindow", "Change Password", 0));
#ifndef QT_NO_TOOLTIP
        listSwitchButton->setToolTip(QApplication::translate("MainWindow", "Click to view call history", 0));
#endif // QT_NO_TOOLTIP
        listSwitchButton->setText(QString());
        numberEdit->setText(QString());
        dtmf1Button->setText(QApplication::translate("MainWindow", "1", 0));
        dtmf2Button->setText(QApplication::translate("MainWindow", "2\n"
"abc", 0));
        dtmf3Button->setText(QApplication::translate("MainWindow", "3\n"
"def", 0));
        dtmf4Button->setText(QApplication::translate("MainWindow", "4\n"
"ghi", 0));
        dtmf5Button->setText(QApplication::translate("MainWindow", "5\n"
"jkl", 0));
        dtmf6Button->setText(QApplication::translate("MainWindow", "6\n"
"mno", 0));
        dtmf7Button->setText(QApplication::translate("MainWindow", "7\n"
"pqrs", 0));
        dtmf8Button->setText(QApplication::translate("MainWindow", "8\n"
"tuv", 0));
        dtmf9Button->setText(QApplication::translate("MainWindow", "9\n"
"wxyz", 0));
        dtmfStarButton->setText(QApplication::translate("MainWindow", "*", 0));
        dtmf0Button->setText(QApplication::translate("MainWindow", "0", 0));
        dtmfHashButton->setText(QApplication::translate("MainWindow", "#", 0));
        dialButton->setText(QApplication::translate("MainWindow", "Dial", 0));
        dialpadButton->setText(QApplication::translate("MainWindow", "Dial Pad", 0));
        statusButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        balanceLabel->setToolTip(QApplication::translate("MainWindow", "Click here to buy credits or update your profile", 0));
#endif // QT_NO_TOOLTIP
        balanceLabel->setText(QApplication::translate("MainWindow", "Balance:", 0));
        menuService->setTitle(QApplication::translate("MainWindow", "MIXvoip", 0));
        menuAccount->setTitle(QApplication::translate("MainWindow", "Account", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuContacts->setTitle(QApplication::translate("MainWindow", "Contacts", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KIAX2WINDOWALT_H
