#include "Kiax2CallDialog.h"
#include "PConstants.h"
#include <QDesktopWidget>
#include "Kiax2MainWindow.h"
#include "Kiax2CallDialogView.h"

inline double fround(double n, unsigned d)
{
return floor(n * pow(10., d) + .5) / pow(10., d);
}

Kiax2CallDialog::Kiax2CallDialog(QWidget* parent, APIBinder* binder, Contact* contactToCall, int cNum):
					Kiax2CallAppearance(parent, binder, contactToCall, cNum)

{

	appearance = new Kiax2CallDialogView(NULL);
	dialogAppearance = (QDialog*)appearance;
	ui.setupUi(dialogAppearance);
	ui.contactNameLabel->setText(QString(contact->contactName.data()));
	if (contact->contactName.compare(contact->extension.data())!=0)
	{
#ifndef USE_DOTTEL	
		ui.contactNumberLabel->setText(QString(contact->extension));	
#else
		ui.contactNumberLabel->setText(decodeExtension(QString::fromStdString(contact->extension), false, true));	
#endif
	}
	ui.muteButton->setEnabled(false);

	ui.contactNameLabel->setText(QString(contact->contactName.data()));
	connect( ui.dtmfButton, SIGNAL (toggled(bool)), this, SLOT(hideDTMF(bool)));
	connect( ui.callButton, SIGNAL (clicked()), this, SLOT(callRequest()));
	connect( ui.muteButton, SIGNAL (toggled(bool)), this, SLOT(muteRequest(bool)));
	connect( ui.dtmf0Button, SIGNAL (clicked()), this, SLOT(dtmf0()));
	connect( ui.dtmf1Button, SIGNAL (clicked()), this, SLOT(dtmf1()));
	connect( ui.dtmf2Button, SIGNAL (clicked()), this, SLOT(dtmf2()));
	connect( ui.dtmf3Button, SIGNAL (clicked()), this, SLOT(dtmf3()));
	connect( ui.dtmf4Button, SIGNAL (clicked()), this, SLOT(dtmf4()));
	connect( ui.dtmf5Button, SIGNAL (clicked()), this, SLOT(dtmf5()));
	connect( ui.dtmf6Button, SIGNAL (clicked()), this, SLOT(dtmf6()));
	connect( ui.dtmf7Button, SIGNAL (clicked()), this, SLOT(dtmf7()));
	connect( ui.dtmf8Button, SIGNAL (clicked()), this, SLOT(dtmf8()));
	connect( ui.dtmf9Button, SIGNAL (clicked()), this, SLOT(dtmf9()));
	connect( ui.dtmfStarButton, SIGNAL (clicked()), this, SLOT(dtmfStar()));
	connect( ui.dtmfHashButton, SIGNAL (clicked()), this, SLOT(dtmfHash()));
	connect(ui.micSlider, SIGNAL(valueChanged(int)), this, SLOT(setMicVolume(int)));
	connect(ui.audioSlider, SIGNAL(valueChanged(int)), this, SLOT(setAudioVolume(int)));
	
	connect(this, SIGNAL (callDurationUpdated(QString)), this, SLOT (callDurationUpdatedSlot(QString)));
	connect(this, SIGNAL (callStateNone()), this, SLOT (callStateNoneSlot()));
	connect(this, SIGNAL (callStateComplete()), this, SLOT (callStateCompleteSlot()));
	connect(this, SIGNAL (callStateInactive()), this, SLOT (callStateInactiveSlot()));
	connect(this, SIGNAL (callDirectionIncoming()), this, SLOT (callDirectionIncomingSlot()));
	connect(this, SIGNAL (callDirectionOutgoing()), this, SLOT (callDirectionOutgoingSlot()));
	connect(this, SIGNAL (guiHungUp()), this, SLOT (guiHungUpSlot()));
	connect(this, SIGNAL (guiDialed()), this, SLOT (guiDialedSlot()));
	connect(this, SIGNAL (guiMuted()), this, SLOT (guiMutedSlot()));
	connect(this, SIGNAL (guiUnmuted()), this, SLOT (guiUnmutedSlot()));
	
	connect(appearance, SIGNAL( dialogShown() ), this, SLOT( dialogShownSlot() ));
	connect(appearance, SIGNAL( dialogClosed() ), this, SLOT( dialogClosedSlot() ));
	connect(appearance, SIGNAL( dialogChanged(bool) ), this, SLOT( dialogChangedSlot(bool) ));
	
	ui.micSlider->setValue((int)(iaxc_input_level_get()*100));
	ui.audioSlider->setValue((int)(iaxc_output_level_get()*100));

#if !defined(IAXCLIENT_HOLD_HACK)
	ui.muteButton->setVisible(false);
#endif
	flags = dialogAppearance->windowFlags();
	QDesktopWidget* desktop = QApplication::desktop();

	int x= desktop->screenGeometry().width()/2 - width()/2;
	int y = desktop->screenGeometry().height()/2 - height()/2 + 100;
	
	dialogAppearance->move(x, y); 
	
	Logger::log(Logger::DEBUG, "dialog for call number %d\n", callNumber);
	
	initialized = false;
	
}

QString Kiax2CallDialog::decodeExtension(QString number, bool decodeHost, bool decodeExtension)
{
               QString telNumber = number;
		QString destination = number;
		QString host = number;
		int hashPos = number.indexOf("##");
		host.truncate(hashPos);
		host += ".tel";
		if (decodeHost)
		{
			Logger::log(Logger::DEBUG, "decodeExtension: returning host-%s\n", host.toStdString().data());
			return host;
		}		
		number.remove(0, hashPos + 2);
		QString telnumber = number;
		hashPos = -1;
		hashPos = telnumber.indexOf("##");
		if (hashPos>-1)
			telnumber.truncate(hashPos);
		if (decodeExtension)
		{
			Logger::log(Logger::DEBUG, "decodeExtension: returning extension-%s\n", telnumber.toStdString().data());
			return telnumber;
		}	
		QString telname= host + " - " + telnumber;
		return telname;
}

void Kiax2CallDialog::setAlwaysOnTop(bool always)
{
	if (always)
	{
		Qt::WindowFlags winFlags;
		winFlags |= Qt::WindowStaysOnTopHint;
		winFlags |= Qt::Dialog;
		dialogAppearance->setWindowFlags(winFlags);
	
	}
	else
	{
		Qt::WindowFlags winFlags;
		winFlags |= Qt::Dialog;
		dialogAppearance->setWindowFlags(winFlags);
	}
}

void Kiax2CallDialog::dialogShownSlot()
{
	if (!initialized)
	{
		ui.dialogLayout->removeWidget(ui.dtmfFrame);
		ui.dialogLayout->setEnabled(false);
		ui.dtmfFrame->setVisible(false);
		dialogAppearance->resize(minimumSize());
		initialized = true;
		dialogAppearance->setFocus(Qt::OtherFocusReason);
	}
}

void Kiax2CallDialog::dialogClosedSlot()
{
	Logger::log(Logger::DEBUG, "dialogClosedSlot(). Closing call %d\n", callNumber);
	closeAppearance();
}

Kiax2CallDialog::~Kiax2CallDialog()
{
}

void Kiax2CallDialog::callStateNoneSlot() 
{
	ui.callButton->setText(tr("Cancel"));
	ui.callButton->setEnabled(true);
	ui.statusLabel->setText(tr("Calling"));	
}

void Kiax2CallDialog::callStateCompleteSlot()
{		
	ui.callButton->setText(tr("Hangup"));
	ui.callButton->setEnabled(true);
#if defined(IAXCLIENT_HOLD_HACK)			
	ui.muteButton->setEnabled(true);
#endif
	ui.statusLabel->setText(tr("Call active"));
}

void Kiax2CallDialog::callStateInactiveSlot()
{		
	ui.callButton->setText(tr("Call"));
	ui.callButton->setEnabled(false);
	ui.muteButton->setText(tr("Hold"));
	ui.muteButton->setEnabled(false);
	ui.statusLabel->setText(tr("Call ended"));			
}

void Kiax2CallDialog::callDirectionIncomingSlot()
{
	if (callState== CALL_RINGING)
		ui.callButton->setText(tr("Answer"));
	ui.callButton->setEnabled(true);
	ui.statusLabel->setText(tr("Incoming call"));
}

void Kiax2CallDialog::callDirectionOutgoingSlot()
{
	ui.callButton->setText(tr("Hangup"));
	ui.callButton->setEnabled(true);
	ui.statusLabel->setText(tr("Calling.."));

}

void Kiax2CallDialog::guiHungUpSlot()
{
	ui.statusLabel->setText(tr("Hung up"));
	ui.callButton->setEnabled(false);
	ui.muteButton->setEnabled(false);
}

void Kiax2CallDialog::guiDialedSlot()
{
	((Kiax2MainWindow*)mainWindow)->dialWithCallAppearance(this);
}
	
void Kiax2CallDialog::guiMutedSlot()
{
	ui.muteButton->setText(tr("Unhold"));
}

void Kiax2CallDialog::guiUnmutedSlot()
{
	ui.muteButton->setText(tr("Hold"));
}

void Kiax2CallDialog::callDurationUpdatedSlot(QString callDurationString)
{
	ui.statusLabel->setText(tr("Duration") + " " + callDurationString);
}

void Kiax2CallDialog::dialogChangedSlot(bool active)
{
	// Logger::log(Logger::DEBUG, "WINDOW STATE CHANGE = %d\n", event->type());
	
		if (active)
		{
//			if ((callState==CALL_COMPLETE)||((callState!=CALL_COMPLETE)&&(callDirection!=CALL_INCOMING)))
			if (((Kiax2MainWindow*)mainWindow)->callMap[callNumber]!=NULL)
			{
				if ((callState==CALL_COMPLETE))
				{
						api->setActiveCall(callNumber);
						Logger::log(Logger::DEBUG, "SET ACTIVE CALL = %d\n", callNumber);
				}
			}
		}
}

void Kiax2CallDialog::hideDTMF(bool hideDtmf)
{
	if (!hideDtmf)
	{
		ui.dtmfFrame->setVisible(false);
		ui.dtmfButtonsFrame->setVisible(false);
		ui.dtmfFrameLayout->removeWidget(ui.dtmfButtonsFrame);
		ui.dialogLayout->removeWidget(ui.dtmfFrame);
		ui.dtmfFrameLayout->setEnabled(false);
		ui.dialogLayout->setEnabled(false);
		dialogAppearance->resize(minimumSize());
	}
	else
	{
		ui.dtmfButtonsFrame->setVisible(true);
		ui.dtmfFrame->setVisible(true);
		dialogAppearance->resize(maximumSize());
		ui.dtmfFrameLayout->addWidget(ui.dtmfButtonsFrame, 0, 0, 1, 1);
		ui.dialogLayout->addWidget(ui.dtmfFrame, 3, 0, 1, 1);
		ui.dtmfFrameLayout->setEnabled(true);
		ui.dialogLayout->setEnabled(true);
	}
	
}

void Kiax2CallDialog::setInputOutputLevels(float inputLevel, float outputLevel)
{
	Logger::log(Logger::DEBUG, "CallWindow %d - setting io levels %d, %d\n", callNumber, inputLevel, outputLevel);
	ui.micLevelProgressBar->setValue((int)(100-(-inputLevel)));
	if (callState==CALL_COMPLETE)

	{
		ui.audioLevelProgressBar->setValue((int)(100-(-outputLevel)));

	}
}

void Kiax2CallDialog::setMicVolume(int level)
{
	iaxc_input_level_set((float) fround(level,2)/100);
	Logger::log(Logger::DEBUG, "input level changed to %d\n", level);
}

void Kiax2CallDialog::setAudioVolume(int level)
{
	iaxc_output_level_set((float)fround(level,2)/100);
	Logger::log(Logger::DEBUG, "output level changed to %d\n", level);	
}

int Kiax2CallDialog::getMicSliderValue()
{
	return ui.micSlider->value();
}

int Kiax2CallDialog::getAudioSliderValue()
{
	return ui.audioSlider->value();
}

void Kiax2CallDialog::setMicSliderValue(int value)
{
	if (ui.micSlider->isEnabled()&&value<0)
		ui.micSlider->setEnabled(false);
	ui.micSlider->setValue(value);
}

void Kiax2CallDialog::setAudioSliderValue(int value)
{
	if (ui.audioSlider->isEnabled()&&value<0)
		ui.audioSlider->setEnabled(false);
	ui.audioSlider->setValue(value);
}

void Kiax2CallDialog::dtmf0()
{
	api->sendDTMF('0', callNumber);
}

void Kiax2CallDialog::dtmf1()
{
	api->sendDTMF('1', callNumber);
}

void Kiax2CallDialog::dtmf2()
{
	api->sendDTMF('2', callNumber);
}

void Kiax2CallDialog::dtmf3()
{
	api->sendDTMF('3', callNumber);
}

void Kiax2CallDialog::dtmf4()
{
	api->sendDTMF('4', callNumber);
}

void Kiax2CallDialog::dtmf5()
{
	api->sendDTMF('5', callNumber);
}

void Kiax2CallDialog::dtmf6()
{
	api->sendDTMF('6', callNumber);
}

void Kiax2CallDialog::dtmf7()
{
	api->sendDTMF('7', callNumber);
}

void Kiax2CallDialog::dtmf8()
{	
	api->sendDTMF('8', callNumber);
}

void Kiax2CallDialog::dtmf9()
{
	api->sendDTMF('9', callNumber);
}

void Kiax2CallDialog::dtmfStar()
{
	api->sendDTMF('*', callNumber);
}

void Kiax2CallDialog::dtmfHash()
{
	api->sendDTMF('#', callNumber);
}
