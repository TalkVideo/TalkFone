#ifndef KIAX2CALLAPPEARANCE_H
#define KIAX2CALLAPPEARANCE_H

#include "APIBinder.h"
#include <QTimer>
#include <QDateTime>
#include <QWidget>

class Kiax2CallAppearance : public QWidget
{

  Q_OBJECT

public:
  Kiax2CallAppearance(QWidget* parent, APIBinder* binder, Contact* contactToCall = 0, int cNum = -1);
  virtual ~Kiax2CallAppearance();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/

  virtual void callRequest();
  virtual void muteRequest(bool state);
  virtual void callDuration();
  virtual void closeAppearance();
	
protected slots:


signals:

	void callDurationUpdated(QString durationString);
	void callStateNone();
	void callStateComplete();
	void callStateInactive();
	void callDirectionIncoming();
	void callDirectionOutgoing();
	void guiHungUp();
	void guiDialed();
	void guiMuted();
	void guiUnmuted();
  
public:
	virtual void setCallState(int state);
	virtual void setCallDirection(int direction);
    virtual void setCallNumber(int cNum);
	virtual void setContact(Contact* contact);
	virtual int getCallState();
	virtual int getCallNumber();
	virtual int getCallDirection();
	virtual Contact* getContact();
	virtual void startCallTimer();
	virtual void stopCallTimer();
	virtual void initiateCall(std::string callDirection);
	QWidget* appearance;
	bool	callAnswered;

protected:
	void closeEvent(QCloseEvent* event);

    Contact* contact;
	APIBinder* api;
	int callNumber;
	int callState;
	int callDirection;
	QWidget* mainWindow;
	QTimer* callDurationTimer;
	QTimer* closeTimer;
	int callStartTime;
	CDR* currentCDR;
	bool initialized;
	QString callDurationStr;
	Qt::WindowFlags flags;

};

#endif

