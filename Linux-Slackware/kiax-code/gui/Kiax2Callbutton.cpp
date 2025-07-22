#include "Kiax2CallButton.h"
#include "PConstants.h"
#include "Logger.h"
// #include "Kiax2MainWindow.h"

Kiax2CallButton::Kiax2CallButton(QWidget* parent, APIBinder* binder, Contact* contactToCall, int cNum):
					Kiax2CallAppearance(parent, binder, contactToCall, cNum)

{
	firstRowText = tr("Calling..");
	if (contact->contactName.compare("")!=0)
		secondRowText = QString::fromStdString(contact->contactName);
	else 
	{
		secondRowText = QString::fromStdString(contact->extension);
	}	
	appearance = new QPushButton(getButtonText());

	buttonAppearance = (QPushButton*)appearance;
	buttonAppearance->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #dadbde, stop: 1 #f6f7fa);");
	buttonAppearance->setCheckable(true);
	buttonAppearance->setIcon(QIcon("icons/contactlarge.png"));

	connect( buttonAppearance, SIGNAL (toggled(bool)), this, SLOT(toggledSlot(bool)));
	
	connect(this, SIGNAL (callDurationUpdated(QString)), this, SLOT (callDurationUpdatedSlot(QString)));
	connect(this, SIGNAL (callStateNone()), this, SLOT (callStateNoneSlot()));
	connect(this, SIGNAL (callStateComplete()), this, SLOT (callStateCompleteSlot()));
	connect(this, SIGNAL (callStateInactive()), this, SLOT (callStateInactiveSlot()));
	connect(this, SIGNAL (callDirectionIncoming()), this, SLOT (callDirectionIncomingSlot()));
	connect(this, SIGNAL (callDirectionOutgoing()), this, SLOT (callDirectionOutgoingSlot()));
	connect(this, SIGNAL (guiHungUp()), this, SLOT (guiHungUpSlot()));
	connect(this, SIGNAL (guiMuted()), this, SLOT (guiMutedSlot()));
	connect(this, SIGNAL (guiUnmuted()), this, SLOT (guiUnmutedSlot()));
	
	initialized = false;
	
}

Kiax2CallButton::~Kiax2CallButton()
{
	delete appearance;
	appearance = NULL;
}

void Kiax2CallButton::callStateNoneSlot() 
{
	firstRowText = tr("Calling..");
	secondRowText = QString::fromStdString(contact->contactName);
	buttonAppearance->setText(getButtonText());
}

void Kiax2CallButton::callStateCompleteSlot()
{		
	firstRowText = tr("Call active");
	buttonAppearance->setText(getButtonText());	
}

void Kiax2CallButton::callStateInactiveSlot()
{		
	firstRowText = tr("Call ended");			
	buttonAppearance->setText(getButtonText());
}

void Kiax2CallButton::callDirectionIncomingSlot()
{
	secondRowText = tr("Incoming call");
	secondRowText = QString::fromStdString(contact->contactName);
	buttonAppearance->setText(getButtonText());
}

void Kiax2CallButton::callDirectionOutgoingSlot()
{
	firstRowText = tr("Calling..");
	secondRowText = QString::fromStdString(contact->contactName);
	buttonAppearance->setText(getButtonText());
}

void Kiax2CallButton::guiHungUpSlot()
{
	firstRowText = tr("Hung up");
	buttonAppearance->setText(getButtonText());
}
	
void Kiax2CallButton::guiMutedSlot()
{
	firstRowText = tr("Muted");
	buttonAppearance->setText(getButtonText());
}

void Kiax2CallButton::guiUnmutedSlot()
{
	firstRowText = tr("Unmuted");
	buttonAppearance->setText(getButtonText());
}

void Kiax2CallButton::callDurationUpdatedSlot(QString callDurationString)
{
	firstRowText = callDurationString;
	buttonAppearance->setText(getButtonText());
}

QString Kiax2CallButton::getButtonText()
{
	return firstRowText + "\n" + secondRowText;
}

void Kiax2CallButton::toggledSlot(bool toggled)
{
	if (toggled)
	{
		Logger::log(Logger::DEBUG, "Selecting call %d\n", callNumber);
		api->setActiveCall(callNumber);
	}
		
}
