#include "Kiax2CallAppearance.h"
#include "PConstants.h"
#include <QDesktopWidget>
#include "Kiax2MainWindow.h"

inline double fround(double n, unsigned d)
{
return floor(n * pow(10., d) + .5) / pow(10., d);
}

Kiax2CallAppearance::Kiax2CallAppearance(QWidget* parent, APIBinder* binder, Contact* contactToCall, int cNum)
{
	appearance = NULL;
	//setAttribute(Qt::WA_DeleteOnClose, true);
	currentCDR = new CDR();
	mainWindow = parent;
	callNumber = cNum;
	callAnswered = false;
	api = binder;
	callDurationTimer = new QTimer();
	closeTimer = new QTimer();
	contact = contactToCall;
	setCallState(CALL_NONE);
	connect(callDurationTimer, SIGNAL(timeout()), this, SLOT(callDuration()));
	connect(closeTimer, SIGNAL(timeout()), this, SLOT(closeAppearance()));
	
	flags = windowFlags();
	QDesktopWidget* desktop = QApplication::desktop();

	initialized = false;
	
}

void Kiax2CallAppearance::closeEvent(QCloseEvent* event)
{
	closeAppearance();
	event->accept();
}

Kiax2CallAppearance::~Kiax2CallAppearance()
{
	Logger::log(Logger::DEBUG, "destoroying call appearance\n");
	delete callDurationTimer;
	delete closeTimer;
	delete contact;
	if (appearance!=NULL)
		delete appearance;
	Logger::log(Logger::DEBUG, "destoroyed call appearance\n");
}

void Kiax2CallAppearance::callDuration()
{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    uint currentTime = QDateTime::currentDateTime().toTime_t();
    uint interval = currentTime - callStartTime;
    div_t hours_calc = div(interval, 3600);
    hours = hours_calc.quot;
    div_t minutes_calc = div(hours_calc.rem, 60);
    minutes = minutes_calc.quot;
    seconds = minutes_calc.rem;
	QString hoursStr = QString::number(hours);
    if (hours<10)
        hoursStr = "0" + hoursStr;
    QString minutesStr = QString::number(minutes);
    if (minutes<10)
        minutesStr = "0" + minutesStr;
    QString secondsStr = QString::number(seconds);
    if (seconds<10)
        secondsStr = "0" + secondsStr;
	callDurationStr = hoursStr + ":" + minutesStr + ":" + secondsStr;
	
	emit callDurationUpdated(callDurationStr);

}

void Kiax2CallAppearance::initiateCall(std::string callDirection)
{
	currentCDR = new CDR();
	QString callInitiationTimeStr = QDateTime::currentDateTime().toString();
	currentCDR->callState = CALL_STATE_NONE;
	currentCDR->direction = callDirection;
	currentCDR->dstExt = contact->extension;
	currentCDR->dstName = contact->contactName;
	currentCDR->cdrTime = callInitiationTimeStr.toStdString();
	
}

void Kiax2CallAppearance::startCallTimer()
{
	callStartTime = QDateTime::currentDateTime().toTime_t();
	callDurationTimer->start(1000);
}

void Kiax2CallAppearance::stopCallTimer()
{
	callDurationTimer->stop();
	callDurationStr = "";
}

void Kiax2CallAppearance::closeAppearance()
{
	closeTimer->stop();
	if (callNumber>-1)
	{	
		api->setActiveCall(callNumber);	
		if (((Kiax2MainWindow*)mainWindow)->callMap[callNumber]!=NULL)
		{
			if (!callAnswered)
				api->reject(callNumber);
			else
				api->hangup(callNumber);
		}
	}
	callNumber = -1;		
	appearance->close();
	//close();
#ifdef REMOTE_CDR	
	((Kiax2MainWindow*)mainWindow)->populateCallRecords("");
#endif	
}

void Kiax2CallAppearance::setCallNumber(int cNum)
{
	callNumber = cNum;
	Logger::log(Logger::INFO, "setting callNumber %d\n",callNumber);
}

void Kiax2CallAppearance::setContact(Contact* contactToCall)
{
	contact = contactToCall;
}

int Kiax2CallAppearance::getCallNumber()
{
	return callNumber;
}

int Kiax2CallAppearance::getCallState()
{
	return callState;
}

Contact* Kiax2CallAppearance::getContact()
{
	return contact;
}

void Kiax2CallAppearance::setCallState(int state)
{
	callState = state;
	switch (callState) {
		case CALL_NONE :
		{
			emit callStateNone();
			callState = state;
			callAnswered = false;
			break;
		}
		case CALL_COMPLETE :
		{
			emit callStateComplete();
			callState = state;
			currentCDR->callState = CALL_STATE_ANSWERED;
			callAnswered = true;
			if (!callDurationTimer->isActive())
				startCallTimer();
			break;
		}
		case CALL_INACTIVE :
		{
			emit callStateInactive();
			callState = state;
			QString duration = callDurationStr;
			stopCallTimer();
#ifndef REMOTE_CDR
			if (callNumber>-1)
				{
					if (currentCDR->callState.compare(CALL_STATE_NONE)==0)
					{
						currentCDR->callState = CALL_STATE_UNANSWERED;
						currentCDR->duration = "00:00:00";
					}
					currentCDR->duration = duration.toStdString();
					api->createObject(currentCDR);
					((Kiax2MainWindow*)mainWindow)->addCDR(currentCDR);
				}
			
#endif
			closeTimer->start(1000);
			//callNumber = -1;
			callAnswered = false;
			break;
		}
	}
}

void Kiax2CallAppearance::setCallDirection(int direction)
{
	callDirection = direction;
	switch (callDirection) {
		case CALL_INCOMING :
		{
			emit callDirectionIncoming();
			callDirection = direction;
			break;
		}
		case CALL_OUTGOING :
		{
			emit callDirectionOutgoing();
			callDirection = direction;
			break;
		}
	}
}

int Kiax2CallAppearance::getCallDirection()
{
	return callDirection;
}

void Kiax2CallAppearance::callRequest()
{
	switch (callState) {

		case CALL_NONE :
		{
			api->hangup(callNumber);
			break;
		}

		case CALL_COMPLETE :
		{
			if (((Kiax2MainWindow*)mainWindow)->callMap[callNumber]!=NULL)
				api->hangup(callNumber);
			emit guiHungUp();
			break;
		}
	
		case CALL_INACTIVE :
		{
			setCallState(CALL_NONE);
			setCallDirection(CALL_NONE);
			emit guiDialed();
			break;
		}
	}
	
	switch (callDirection) {
		case CALL_INCOMING :
		{
			if (callState == CALL_RINGING)
			{
				Logger::log(Logger::DEBUG, "answering call %d\n", callNumber);
				api->answer(callNumber);
				api->setActiveCall(callNumber);
				callAnswered = true;
			}
			break;
		}
		case CALL_OUTGOING :
		{
			if (((Kiax2MainWindow*)mainWindow)->callMap[callNumber]!=NULL)
				api->hangup(callNumber);
			callDirection = CALL_NONE;
			emit guiHungUp();
			break;
		}
	}
}

void Kiax2CallAppearance::muteRequest(bool state)
{
	if (state)
	{
		api->muteCall(callNumber);
		emit guiMuted();
	}
	else
	{
		api->unmuteCall(callNumber);
		emit guiUnmuted();
	}
}



