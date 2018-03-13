#ifndef IAX2CALLBACKHANDLER_H_
#define IAX2CALLBACKHANDLER_H_


#include <vector>
#include "SignalingCallback.h"
#include "iaxclient.h"

class IAX2CallbackHandler 
{

public:
	std::vector<SignalingCallback*> signalHandlers;	
	
	IAX2CallbackHandler() {}
	
	virtual ~IAX2CallbackHandler() {}
	
	virtual void handleIAXEvent(iaxc_event event) = 0;

	virtual void event_level(float in, float out) = 0;
	
	virtual void event_state(int callNo, int state, char *remote, char *remote_name,
							char *local, char *local_context) = 0;
	
	virtual void event_text(int type, char *message) = 0;
	
	virtual void event_registration(int id, int reply, int msgcount) = 0;

	

protected:
	static IAX2CallbackHandler* singleton;

private:


};

#endif /*IAX2CALLBACKHANDLER_H_*/
