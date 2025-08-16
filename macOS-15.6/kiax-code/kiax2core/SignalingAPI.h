#ifndef SIGNALINGAPI_H_
#define SIGNALINGAPI_H_

#include "Account.h"
#include "Contact.h"
#include "SignalingCallback.h"

class SignalingAPI {

 public:

	virtual void registerAccount(Account* account) = 0;
	
	virtual void unregisterAccount(Account* account) = 0;
	
	virtual int dial(std::string extension, Account* account) = 0;

    virtual void hangup(int callNumber) = 0;

    virtual void reject(int callNumber) = 0;

    virtual void answer(int callNumber) = 0;

    virtual void sendMessage(std::string message) = 0;

    virtual void transferCall(std::string extension, int callNumber) = 0;

    virtual void setActiveCall(int activeCallNumber) = 0;
	
	virtual int getActiveCallNumber() = 0;

    virtual void addSignalHandler(SignalingCallback* handler) = 0;
    
    virtual void removeSignalHandler(int handlerNumber) = 0;

    virtual void sendDTMF(char tone, int callNumber) = 0;
    
    virtual void muteCall(int callNumber) = 0;

    virtual void unmuteCall(int callNumber) = 0;

    virtual ~SignalingAPI() {}

};

#endif /*SIGNALINGAPI_H_*/
