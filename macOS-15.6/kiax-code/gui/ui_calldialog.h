/********************************************************************************
** Form generated from reading UI file 'calldialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALLDIALOG_H
#define UI_CALLDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

QT_BEGIN_NAMESPACE

class Ui_CallDialog
{
public:
    QGridLayout *dialogLayout;
    QLabel *contactNameLabel;
    QFrame *frame_2;
    QGridLayout *gridLayout;
    QLabel *userIconLabel;
    QLabel *statusLabel;
    QFrame *levelsFrame;
    QGridLayout *gridLayout1;
    QLabel *micLevelLabel;
    QProgressBar *micLevelProgressBar;
    QLabel *audioLevelLabel;
    QProgressBar *audioLevelProgressBar;
    QSlider *micSlider;
    QSlider *audioSlider;
    QFrame *frame;
    QGridLayout *callButtonsLayout;
    QPushButton *muteButton;
    QPushButton *dtmfButton;
    QPushButton *callButton;
    QFrame *dtmfFrame;
    QGridLayout *dtmfFrameLayout;
    QFrame *dtmfButtonsFrame;
    QGridLayout *dtmfButtonsLayout;
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
    QLabel *contactNumberLabel;

    void setupUi(QDialog *CallDialog)
    {
        if (CallDialog->objectName().isEmpty())
            CallDialog->setObjectName(QString::fromUtf8("CallDialog"));
        CallDialog->resize(281, 555);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CallDialog->sizePolicy().hasHeightForWidth());
        CallDialog->setSizePolicy(sizePolicy);
        CallDialog->setMinimumSize(QSize(281, 350));
        CallDialog->setMaximumSize(QSize(281, 555));
        dialogLayout = new QGridLayout(CallDialog);
        dialogLayout->setObjectName(QString::fromUtf8("dialogLayout"));
        contactNameLabel = new QLabel(CallDialog);
        contactNameLabel->setObjectName(QString::fromUtf8("contactNameLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(contactNameLabel->sizePolicy().hasHeightForWidth());
        contactNameLabel->setSizePolicy(sizePolicy1);
        contactNameLabel->setMinimumSize(QSize(0, 0));
        contactNameLabel->setMaximumSize(QSize(16777215, 30));
        QFont font;
        font.setPointSize(14);
        contactNameLabel->setFont(font);

        dialogLayout->addWidget(contactNameLabel, 0, 0, 1, 1);

        frame_2 = new QFrame(CallDialog);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy2);
        frame_2->setMinimumSize(QSize(0, 89));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_2);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        userIconLabel = new QLabel(frame_2);
        userIconLabel->setObjectName(QString::fromUtf8("userIconLabel"));
        sizePolicy2.setHeightForWidth(userIconLabel->sizePolicy().hasHeightForWidth());
        userIconLabel->setSizePolicy(sizePolicy2);
        userIconLabel->setMinimumSize(QSize(0, 64));
        userIconLabel->setPixmap(QPixmap(QString::fromUtf8("icons/contactlarge.png")));
        userIconLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(userIconLabel, 0, 0, 1, 1);

        statusLabel = new QLabel(frame_2);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(statusLabel, 1, 0, 1, 1);

        levelsFrame = new QFrame(frame_2);
        levelsFrame->setObjectName(QString::fromUtf8("levelsFrame"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(levelsFrame->sizePolicy().hasHeightForWidth());
        levelsFrame->setSizePolicy(sizePolicy3);
        levelsFrame->setFrameShape(QFrame::NoFrame);
        levelsFrame->setFrameShadow(QFrame::Raised);
        gridLayout1 = new QGridLayout(levelsFrame);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        gridLayout1->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout1->setHorizontalSpacing(3);
        gridLayout1->setVerticalSpacing(0);
        gridLayout1->setContentsMargins(-1, 12, -1, 0);
        micLevelLabel = new QLabel(levelsFrame);
        micLevelLabel->setObjectName(QString::fromUtf8("micLevelLabel"));

        gridLayout1->addWidget(micLevelLabel, 0, 0, 2, 1);

        micLevelProgressBar = new QProgressBar(levelsFrame);
        micLevelProgressBar->setObjectName(QString::fromUtf8("micLevelProgressBar"));
        micLevelProgressBar->setMaximumSize(QSize(16777215, 10));
        micLevelProgressBar->setValue(0);
        micLevelProgressBar->setTextVisible(false);

        gridLayout1->addWidget(micLevelProgressBar, 0, 1, 1, 1);

        audioLevelLabel = new QLabel(levelsFrame);
        audioLevelLabel->setObjectName(QString::fromUtf8("audioLevelLabel"));

        gridLayout1->addWidget(audioLevelLabel, 2, 0, 2, 1);

        audioLevelProgressBar = new QProgressBar(levelsFrame);
        audioLevelProgressBar->setObjectName(QString::fromUtf8("audioLevelProgressBar"));
        audioLevelProgressBar->setMaximumSize(QSize(16777215, 10));
        audioLevelProgressBar->setValue(0);
        audioLevelProgressBar->setTextVisible(false);

        gridLayout1->addWidget(audioLevelProgressBar, 2, 1, 1, 1);

        micSlider = new QSlider(levelsFrame);
        micSlider->setObjectName(QString::fromUtf8("micSlider"));
        micSlider->setMaximumSize(QSize(16777215, 16777215));
        micSlider->setOrientation(Qt::Horizontal);
        micSlider->setTickPosition(QSlider::TicksAbove);

        gridLayout1->addWidget(micSlider, 1, 1, 1, 1);

        audioSlider = new QSlider(levelsFrame);
        audioSlider->setObjectName(QString::fromUtf8("audioSlider"));
        audioSlider->setOrientation(Qt::Horizontal);
        audioSlider->setTickPosition(QSlider::TicksAbove);

        gridLayout1->addWidget(audioSlider, 3, 1, 1, 1);


        gridLayout->addWidget(levelsFrame, 2, 0, 1, 1);


        dialogLayout->addWidget(frame_2, 2, 0, 1, 1);

        frame = new QFrame(CallDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy3.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy3);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        callButtonsLayout = new QGridLayout(frame);
        callButtonsLayout->setObjectName(QString::fromUtf8("callButtonsLayout"));
        muteButton = new QPushButton(frame);
        muteButton->setObjectName(QString::fromUtf8("muteButton"));
        muteButton->setCheckable(true);
        muteButton->setChecked(false);

        callButtonsLayout->addWidget(muteButton, 0, 0, 1, 1);

        dtmfButton = new QPushButton(frame);
        dtmfButton->setObjectName(QString::fromUtf8("dtmfButton"));
        dtmfButton->setCheckable(true);

        callButtonsLayout->addWidget(dtmfButton, 0, 1, 1, 1);

        callButton = new QPushButton(frame);
        callButton->setObjectName(QString::fromUtf8("callButton"));

        callButtonsLayout->addWidget(callButton, 0, 2, 1, 1);


        dialogLayout->addWidget(frame, 3, 0, 1, 1);

        dtmfFrame = new QFrame(CallDialog);
        dtmfFrame->setObjectName(QString::fromUtf8("dtmfFrame"));
        sizePolicy2.setHeightForWidth(dtmfFrame->sizePolicy().hasHeightForWidth());
        dtmfFrame->setSizePolicy(sizePolicy2);
        dtmfFrame->setMinimumSize(QSize(0, 200));
        dtmfFrame->setMaximumSize(QSize(289, 200));
        dtmfFrame->setFrameShape(QFrame::NoFrame);
        dtmfFrame->setFrameShadow(QFrame::Raised);
        dtmfFrameLayout = new QGridLayout(dtmfFrame);
        dtmfFrameLayout->setObjectName(QString::fromUtf8("dtmfFrameLayout"));
        dtmfButtonsFrame = new QFrame(dtmfFrame);
        dtmfButtonsFrame->setObjectName(QString::fromUtf8("dtmfButtonsFrame"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(dtmfButtonsFrame->sizePolicy().hasHeightForWidth());
        dtmfButtonsFrame->setSizePolicy(sizePolicy4);
        dtmfButtonsFrame->setMinimumSize(QSize(0, 180));
        dtmfButtonsFrame->setFrameShape(QFrame::NoFrame);
        dtmfButtonsFrame->setFrameShadow(QFrame::Raised);
        dtmfButtonsLayout = new QGridLayout(dtmfButtonsFrame);
        dtmfButtonsLayout->setObjectName(QString::fromUtf8("dtmfButtonsLayout"));
        dtmf1Button = new QPushButton(dtmfButtonsFrame);
        dtmf1Button->setObjectName(QString::fromUtf8("dtmf1Button"));
        dtmf1Button->setMinimumSize(QSize(40, 40));
        dtmf1Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf1Button, 0, 0, 1, 1);

        dtmf2Button = new QPushButton(dtmfButtonsFrame);
        dtmf2Button->setObjectName(QString::fromUtf8("dtmf2Button"));
        dtmf2Button->setMinimumSize(QSize(40, 40));
        dtmf2Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf2Button, 0, 1, 1, 1);

        dtmf3Button = new QPushButton(dtmfButtonsFrame);
        dtmf3Button->setObjectName(QString::fromUtf8("dtmf3Button"));
        dtmf3Button->setMinimumSize(QSize(40, 40));
        dtmf3Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf3Button, 0, 2, 1, 1);

        dtmf4Button = new QPushButton(dtmfButtonsFrame);
        dtmf4Button->setObjectName(QString::fromUtf8("dtmf4Button"));
        dtmf4Button->setMinimumSize(QSize(40, 40));
        dtmf4Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf4Button, 1, 0, 1, 1);

        dtmf5Button = new QPushButton(dtmfButtonsFrame);
        dtmf5Button->setObjectName(QString::fromUtf8("dtmf5Button"));
        dtmf5Button->setMinimumSize(QSize(40, 40));
        dtmf5Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf5Button, 1, 1, 1, 1);

        dtmf6Button = new QPushButton(dtmfButtonsFrame);
        dtmf6Button->setObjectName(QString::fromUtf8("dtmf6Button"));
        dtmf6Button->setMinimumSize(QSize(40, 40));
        dtmf6Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf6Button, 1, 2, 1, 1);

        dtmf7Button = new QPushButton(dtmfButtonsFrame);
        dtmf7Button->setObjectName(QString::fromUtf8("dtmf7Button"));
        dtmf7Button->setMinimumSize(QSize(40, 40));
        dtmf7Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf7Button, 2, 0, 1, 1);

        dtmf8Button = new QPushButton(dtmfButtonsFrame);
        dtmf8Button->setObjectName(QString::fromUtf8("dtmf8Button"));
        dtmf8Button->setMinimumSize(QSize(40, 40));
        dtmf8Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf8Button, 2, 1, 1, 1);

        dtmf9Button = new QPushButton(dtmfButtonsFrame);
        dtmf9Button->setObjectName(QString::fromUtf8("dtmf9Button"));
        dtmf9Button->setMinimumSize(QSize(40, 40));
        dtmf9Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf9Button, 2, 2, 1, 1);

        dtmfStarButton = new QPushButton(dtmfButtonsFrame);
        dtmfStarButton->setObjectName(QString::fromUtf8("dtmfStarButton"));
        dtmfStarButton->setMinimumSize(QSize(40, 40));
        dtmfStarButton->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmfStarButton, 3, 0, 1, 1);

        dtmf0Button = new QPushButton(dtmfButtonsFrame);
        dtmf0Button->setObjectName(QString::fromUtf8("dtmf0Button"));
        dtmf0Button->setMinimumSize(QSize(40, 40));
        dtmf0Button->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmf0Button, 3, 1, 1, 1);

        dtmfHashButton = new QPushButton(dtmfButtonsFrame);
        dtmfHashButton->setObjectName(QString::fromUtf8("dtmfHashButton"));
        dtmfHashButton->setMinimumSize(QSize(40, 40));
        dtmfHashButton->setMaximumSize(QSize(40, 40));

        dtmfButtonsLayout->addWidget(dtmfHashButton, 3, 2, 1, 1);


        dtmfFrameLayout->addWidget(dtmfButtonsFrame, 0, 0, 1, 1);


        dialogLayout->addWidget(dtmfFrame, 4, 0, 1, 1);

        contactNumberLabel = new QLabel(CallDialog);
        contactNumberLabel->setObjectName(QString::fromUtf8("contactNumberLabel"));
        sizePolicy2.setHeightForWidth(contactNumberLabel->sizePolicy().hasHeightForWidth());
        contactNumberLabel->setSizePolicy(sizePolicy2);
        contactNumberLabel->setMinimumSize(QSize(0, 20));
        contactNumberLabel->setMaximumSize(QSize(16777215, 20));
        QFont font1;
        font1.setPointSize(11);
        contactNumberLabel->setFont(font1);

        dialogLayout->addWidget(contactNumberLabel, 1, 0, 1, 1);

        QWidget::setTabOrder(muteButton, dtmfButton);

        retranslateUi(CallDialog);

        QMetaObject::connectSlotsByName(CallDialog);
    } // setupUi

    void retranslateUi(QDialog *CallDialog)
    {
        CallDialog->setWindowTitle(QApplication::translate("CallDialog", "Call", 0));
        contactNameLabel->setText(QApplication::translate("CallDialog", "Contact Name", 0));
        userIconLabel->setText(QString());
        statusLabel->setText(QApplication::translate("CallDialog", "Calling", 0));
        micLevelLabel->setText(QApplication::translate("CallDialog", "Mic", 0));
        audioLevelLabel->setText(QApplication::translate("CallDialog", "Audio", 0));
        muteButton->setText(QApplication::translate("CallDialog", "Hold", 0));
        dtmfButton->setText(QApplication::translate("CallDialog", "Dial Pad", 0));
        callButton->setText(QApplication::translate("CallDialog", "Dial", 0));
        dtmf1Button->setText(QApplication::translate("CallDialog", "1", 0));
        dtmf2Button->setText(QApplication::translate("CallDialog", "2\n"
"abc", 0));
        dtmf3Button->setText(QApplication::translate("CallDialog", "3\n"
"def", 0));
        dtmf4Button->setText(QApplication::translate("CallDialog", "4\n"
"ghi", 0));
        dtmf5Button->setText(QApplication::translate("CallDialog", "5\n"
"jkl", 0));
        dtmf6Button->setText(QApplication::translate("CallDialog", "6\n"
"mno", 0));
        dtmf7Button->setText(QApplication::translate("CallDialog", "7\n"
"pqrs", 0));
        dtmf8Button->setText(QApplication::translate("CallDialog", "8\n"
"tuv", 0));
        dtmf9Button->setText(QApplication::translate("CallDialog", "9\n"
"wxyz", 0));
        dtmfStarButton->setText(QApplication::translate("CallDialog", "*", 0));
        dtmf0Button->setText(QApplication::translate("CallDialog", "0", 0));
        dtmfHashButton->setText(QApplication::translate("CallDialog", "#", 0));
        contactNumberLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CallDialog: public Ui_CallDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALLDIALOG_H
