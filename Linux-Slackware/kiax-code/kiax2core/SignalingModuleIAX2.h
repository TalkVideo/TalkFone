#ifndef SIGNALINGMODULEIAX2_H_
#define SIGNALINGMODULEIAX2_H_

#include "SignalingModule.h"
#include "iaxclient.h"
#include "IAX2CallbackHandler.h"
#include <map>

class SignalingModuleIAX2 : public SignalingModule {

 public:

	static IAX2CallbackHandler* handler;
	
	static SignalingModule* instance();
	
	virtual void init();
	
	virtual void dispose();
	
	virtual void registerAccount(Account* account);

	virtual void unregisterAccount(Account* account);
	
    virtual int dial(std::string extension, Account* account);

    virtual void hangup(int callNumber);

    virtual void reject(int callNumber);

    virtual void answer(int callNumber);

    virtual void sendMessage(std::string message);

    virtual void transferCall(std::string extension, int callNumber);

    virtual void setActiveCall(int activeCallNumber);
    
    virtual void addSignalHandler(SignalingCallback* handler);
    
    virtual void removeSignalHandler(int handlerNumber);
    
    virtual void sendDTMF(char tone, int callNumber);
    
    virtual void muteCall(int callNumber);

    virtual void unmuteCall(int callNumber);
	
	virtual void setIAX2CallbackHandler(IAX2CallbackHandler* callbackHandler);
	
	virtual int getActiveCallNumber();

 private:
	 
	SignalingModuleIAX2();

	std::map<Account*, int> registrations;
 public:
	Account* findAccountByRegistrationKey(int regId);
 
 public:
	 
};


#endif /*SIGNALINGMODULEIAX2_H_*/
