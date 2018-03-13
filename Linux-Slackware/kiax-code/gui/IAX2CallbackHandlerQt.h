#ifndef IAX2CALLBACKHANDLERQT_H_
#define IAX2CALLBACKHANDLERQT_H_

#include "IAX2CallbackHandler.h"
#include "Logger.h"
#include <qobject.h>
#include <qevent.h>

class IAX2CallbackHandlerQt : public QObject, public IAX2CallbackHandler
{

Q_OBJECT

public:

	IAX2CallbackHandlerQt(QObject *parent = 0, const char *name = 0);
	
	static IAX2CallbackHandler* instance();
	
	virtual ~IAX2CallbackHandlerQt();
	
	virtual void handleIAXEvent(iaxc_event event);

	virtual void event_level(float in, float out);
	
	virtual void event_state(int callNo, int state, char *remote, char *remote_name,
							char *local, char *local_context);
	
	virtual void event_text(int type, char *message);
	
	virtual void event_registration(int id, int reply, int msgcount);
	
	void customEvent(QEvent * qevent);	

	virtual void handleIaxCEvent(iaxc_event event);

};

#endif /*IAX2CALLBACKHANDLERDEFAULT_H_*/
