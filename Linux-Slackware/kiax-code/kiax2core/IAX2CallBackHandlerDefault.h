#ifndef IAX2CALLBACKHANDLERDEFAULT_H_
#define IAX2CALLBACKHANDLERDEFAULT_H_

#include "IAX2CallbackHandler.h"
#include "Logger.h"

class IAX2CallbackHandlerDefault : public IAX2CallbackHandler
{
public:
	
	static IAX2CallbackHandler* instance();
	
	virtual ~IAX2CallbackHandlerDefault();
	
	virtual void handleIAXEvent(iaxc_event event);

	virtual void event_level(double in, double out);
	
	virtual void event_state(int callNo, int state, char *remote, char *remote_name,
							char *local, char *local_context);
	
	virtual void event_text(int type, char *message);
	
	virtual void event_registration(int id, int reply, int msgcount);
	
private:
	IAX2CallbackHandlerDefault();
};

#endif /*IAX2CALLBACKHANDLERDEFAULT_H_*/
