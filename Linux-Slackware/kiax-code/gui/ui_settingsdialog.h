/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QGridLayout *gridLayout_3;
    QTabWidget *tabWidget;
    QWidget *general;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_6;
    QCheckBox *dialWinOnTop;
    QCheckBox *mainWinOnTop;
    QGroupBox *devicesBox;
    QGridLayout *_2;
    QLabel *inputDeviceIcon;
    QLabel *label;
    QComboBox *inputDeviceComboBox;
    QLabel *outputDeviceIcon;
    QLabel *label_2;
    QComboBox *outputDeviceComboBox;
    QLabel *ringDeviceIcon;
    QLabel *label_3;
    QComboBox *ringDeviceComboBox;
    QGroupBox *networkGroupBox;
    QGridLayout *gridLayout;
    QCheckBox *networkAutoCheckBox;
    QLabel *codecLabel;
    QComboBox *codecComboBox;
    QCheckBox *versionInfoCheckBox;
    QGroupBox *startupCheckBoxes;
    QGridLayout *gridLayout1;
    QCheckBox *autoCheckBox;
    QCheckBox *onStartupCheckBox;
    QCheckBox *hideCheckBox;
    QWidget *advanced;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QLabel *startupLabel;
    QComboBox *startupComboBox;
    QLabel *routingLabel;
    QComboBox *routingComboBox;
    QSpacerItem *verticalSpacer;
    QSpacerItem *spacer;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(413, 458);
        gridLayout_3 = new QGridLayout(SettingsDialog);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabWidget = new QTabWidget(SettingsDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        general = new QWidget();
        general->setObjectName(QString::fromUtf8("general"));
        gridLayout_2 = new QGridLayout(general);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox_2 = new QGroupBox(general);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_6 = new QGridLayout(groupBox_2);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        dialWinOnTop = new QCheckBox(groupBox_2);
        dialWinOnTop->setObjectName(QString::fromUtf8("dialWinOnTop"));
        dialWinOnTop->setChecked(true);

        gridLayout_6->addWidget(dialWinOnTop, 0, 0, 1, 1);

        mainWinOnTop = new QCheckBox(groupBox_2);
        mainWinOnTop->setObjectName(QString::fromUtf8("mainWinOnTop"));
        mainWinOnTop->setChecked(true);

        gridLayout_6->addWidget(mainWinOnTop, 0, 1, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 0, 0, 1, 1);

        devicesBox = new QGroupBox(general);
        devicesBox->setObjectName(QString::fromUtf8("devicesBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(devicesBox->sizePolicy().hasHeightForWidth());
        devicesBox->setSizePolicy(sizePolicy);
        _2 = new QGridLayout(devicesBox);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setContentsMargins(-1, 3, -1, 3);
        inputDeviceIcon = new QLabel(devicesBox);
        inputDeviceIcon->setObjectName(QString::fromUtf8("inputDeviceIcon"));
        inputDeviceIcon->setPixmap(QPixmap(QString::fromUtf8("icons/outputdevice.png")));

        _2->addWidget(inputDeviceIcon, 0, 0, 1, 1);

        label = new QLabel(devicesBox);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        _2->addWidget(label, 0, 1, 1, 1);

        inputDeviceComboBox = new QComboBox(devicesBox);
        inputDeviceComboBox->setObjectName(QString::fromUtf8("inputDeviceComboBox"));

        _2->addWidget(inputDeviceComboBox, 0, 2, 1, 1);

        outputDeviceIcon = new QLabel(devicesBox);
        outputDeviceIcon->setObjectName(QString::fromUtf8("outputDeviceIcon"));
        outputDeviceIcon->setPixmap(QPixmap(QString::fromUtf8("icons/outputdevice.png")));

        _2->addWidget(outputDeviceIcon, 1, 0, 1, 1);

        label_2 = new QLabel(devicesBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        _2->addWidget(label_2, 1, 1, 1, 1);

        outputDeviceComboBox = new QComboBox(devicesBox);
        outputDeviceComboBox->setObjectName(QString::fromUtf8("outputDeviceComboBox"));

        _2->addWidget(outputDeviceComboBox, 1, 2, 1, 1);

        ringDeviceIcon = new QLabel(devicesBox);
        ringDeviceIcon->setObjectName(QString::fromUtf8("ringDeviceIcon"));
        ringDeviceIcon->setPixmap(QPixmap(QString::fromUtf8("icons/ringdevice.png")));

        _2->addWidget(ringDeviceIcon, 2, 0, 1, 1);

        label_3 = new QLabel(devicesBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        _2->addWidget(label_3, 2, 1, 1, 1);

        ringDeviceComboBox = new QComboBox(devicesBox);
        ringDeviceComboBox->setObjectName(QString::fromUtf8("ringDeviceComboBox"));

        _2->addWidget(ringDeviceComboBox, 2, 2, 1, 1);


        gridLayout_2->addWidget(devicesBox, 1, 0, 1, 1);

        networkGroupBox = new QGroupBox(general);
        networkGroupBox->setObjectName(QString::fromUtf8("networkGroupBox"));
        gridLayout = new QGridLayout(networkGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(-1, 3, -1, 3);
        networkAutoCheckBox = new QCheckBox(networkGroupBox);
        networkAutoCheckBox->setObjectName(QString::fromUtf8("networkAutoCheckBox"));
        networkAutoCheckBox->setChecked(true);

        gridLayout->addWidget(networkAutoCheckBox, 0, 0, 1, 1);

        codecLabel = new QLabel(networkGroupBox);
        codecLabel->setObjectName(QString::fromUtf8("codecLabel"));
        codecLabel->setEnabled(false);

        gridLayout->addWidget(codecLabel, 0, 1, 1, 1);

        codecComboBox = new QComboBox(networkGroupBox);
        codecComboBox->setObjectName(QString::fromUtf8("codecComboBox"));
        codecComboBox->setEnabled(false);

        gridLayout->addWidget(codecComboBox, 0, 2, 1, 1);

        versionInfoCheckBox = new QCheckBox(networkGroupBox);
        versionInfoCheckBox->setObjectName(QString::fromUtf8("versionInfoCheckBox"));
        versionInfoCheckBox->setChecked(true);

        gridLayout->addWidget(versionInfoCheckBox, 1, 0, 1, 3);


        gridLayout_2->addWidget(networkGroupBox, 2, 0, 1, 1);

        startupCheckBoxes = new QGroupBox(general);
        startupCheckBoxes->setObjectName(QString::fromUtf8("startupCheckBoxes"));
        gridLayout1 = new QGridLayout(startupCheckBoxes);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        gridLayout1->setContentsMargins(9, 3, -1, 3);
        autoCheckBox = new QCheckBox(startupCheckBoxes);
        autoCheckBox->setObjectName(QString::fromUtf8("autoCheckBox"));
        autoCheckBox->setChecked(true);

        gridLayout1->addWidget(autoCheckBox, 0, 0, 1, 1);

        onStartupCheckBox = new QCheckBox(startupCheckBoxes);
        onStartupCheckBox->setObjectName(QString::fromUtf8("onStartupCheckBox"));
        onStartupCheckBox->setEnabled(false);
        onStartupCheckBox->setChecked(true);

        gridLayout1->addWidget(onStartupCheckBox, 0, 1, 1, 1);

        hideCheckBox = new QCheckBox(startupCheckBoxes);
        hideCheckBox->setObjectName(QString::fromUtf8("hideCheckBox"));
        hideCheckBox->setEnabled(false);
        hideCheckBox->setChecked(true);

        gridLayout1->addWidget(hideCheckBox, 0, 2, 1, 1);


        gridLayout_2->addWidget(startupCheckBoxes, 3, 0, 1, 1);

        tabWidget->addTab(general, QString());
        advanced = new QWidget();
        advanced->setObjectName(QString::fromUtf8("advanced"));
        gridLayout_5 = new QGridLayout(advanced);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBox = new QGroupBox(advanced);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(-1, -1, -1, 12);
        startupLabel = new QLabel(groupBox);
        startupLabel->setObjectName(QString::fromUtf8("startupLabel"));

        gridLayout_4->addWidget(startupLabel, 0, 0, 1, 1);

        startupComboBox = new QComboBox(groupBox);
        startupComboBox->setObjectName(QString::fromUtf8("startupComboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(startupComboBox->sizePolicy().hasHeightForWidth());
        startupComboBox->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(startupComboBox, 0, 1, 1, 1);

        routingLabel = new QLabel(groupBox);
        routingLabel->setObjectName(QString::fromUtf8("routingLabel"));

        gridLayout_4->addWidget(routingLabel, 1, 0, 1, 1);

        routingComboBox = new QComboBox(groupBox);
        routingComboBox->setObjectName(QString::fromUtf8("routingComboBox"));
        sizePolicy1.setHeightForWidth(routingComboBox->sizePolicy().hasHeightForWidth());
        routingComboBox->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(routingComboBox, 1, 1, 1, 1);


        gridLayout_5->addWidget(groupBox, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 179, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 1, 0, 1, 1);

        tabWidget->addTab(advanced, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 3);

        spacer = new QSpacerItem(139, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(spacer, 1, 0, 1, 1);

        saveButton = new QPushButton(SettingsDialog);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        gridLayout_3->addWidget(saveButton, 1, 1, 1, 1);

        cancelButton = new QPushButton(SettingsDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout_3->addWidget(cancelButton, 1, 2, 1, 1);


        retranslateUi(SettingsDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Settings", 0));
        groupBox_2->setTitle(QApplication::translate("SettingsDialog", "Appearance", 0));
        dialWinOnTop->setText(QApplication::translate("SettingsDialog", "Call windows stay on top", 0));
        mainWinOnTop->setText(QApplication::translate("SettingsDialog", "All windows stay on top", 0));
        devicesBox->setTitle(QApplication::translate("SettingsDialog", "Devices", 0));
        inputDeviceIcon->setText(QString());
        label->setText(QApplication::translate("SettingsDialog", "Input device", 0));
        outputDeviceIcon->setText(QString());
        label_2->setText(QApplication::translate("SettingsDialog", "Output device", 0));
        ringDeviceIcon->setText(QString());
        label_3->setText(QApplication::translate("SettingsDialog", "Ring device", 0));
        networkGroupBox->setTitle(QApplication::translate("SettingsDialog", "Network", 0));
        networkAutoCheckBox->setText(QApplication::translate("SettingsDialog", "Auto", 0));
        codecLabel->setText(QApplication::translate("SettingsDialog", "Voice quality", 0));
        codecComboBox->clear();
        codecComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "ULAW", 0)
         << QApplication::translate("SettingsDialog", "GSM", 0)
         << QApplication::translate("SettingsDialog", "SPEEX", 0)
         << QApplication::translate("SettingsDialog", "iLBC", 0)
        );
        versionInfoCheckBox->setText(QApplication::translate("SettingsDialog", "Request Version Info (anonymous)", 0));
        startupCheckBoxes->setTitle(QApplication::translate("SettingsDialog", "Start up", 0));
        autoCheckBox->setText(QApplication::translate("SettingsDialog", "Auto", 0));
        onStartupCheckBox->setText(QApplication::translate("SettingsDialog", "Connect on start up", 0));
        hideCheckBox->setText(QApplication::translate("SettingsDialog", "Hide window", 0));
        tabWidget->setTabText(tabWidget->indexOf(general), QApplication::translate("SettingsDialog", "General", 0));
        groupBox->setTitle(QApplication::translate("SettingsDialog", "System", 0));
        startupLabel->setText(QApplication::translate("SettingsDialog", "Startup", 0));
        startupComboBox->clear();
        startupComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "Run on OS start", 0)
         << QApplication::translate("SettingsDialog", "Do not run on OS start", 0)
        );
        routingLabel->setText(QApplication::translate("SettingsDialog", "Routing", 0));
        routingComboBox->clear();
        routingComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "Softphone", 0)
         << QApplication::translate("SettingsDialog", "Server", 0)
        );
        tabWidget->setTabText(tabWidget->indexOf(advanced), QApplication::translate("SettingsDialog", "Advanced", 0));
        saveButton->setText(QApplication::translate("SettingsDialog", "Save", 0));
        cancelButton->setText(QApplication::translate("SettingsDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
