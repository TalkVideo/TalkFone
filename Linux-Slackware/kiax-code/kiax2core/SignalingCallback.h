#ifndef SignalingCallback_h
#define SignalingCallback_h

#include <string>
#include "Account.h"

class SignalingCallback {

 public:

	virtual void registrationAccepted(Account* account) = 0;

	virtual void registrationTimedout(Account* account) = 0;
	
	virtual void registrationRejected(Account* account) = 0;
	
	virtual void incomingCall(int callNumber, std::string callerId)  = 0;

    virtual void ringing(int callNumber)  = 0;

    virtual void outgoingCall(int callNumber)  = 0;

    virtual void callInactive(int callNumber)  = 0;

    virtual void callTransferred(int callNumber) = 0;

    virtual void messageReceived(std::string message) = 0;
    
    virtual void messageSent(std::string message) = 0;
    
    virtual void activeCallChanged(int callNumber) = 0;

    virtual void callComplete(int callNumber) = 0;
    
    virtual void inputOutputLevels(float inputLevel, float outputLevel) = 0;

    virtual ~SignalingCallback() {}

};
#endif
