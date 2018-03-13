#include "IAX2CallbackHandlerDefault.h"
#include "SignalingModuleIAX2.h"

IAX2CallbackHandler* IAX2CallbackHandler::singleton;

IAX2CallbackHandler* IAX2CallbackHandlerDefault::instance()
{
	if (singleton==NULL)
	{
		singleton = new IAX2CallbackHandlerDefault();
	}
	return singleton;
}

IAX2CallbackHandlerDefault::IAX2CallbackHandlerDefault()
{

}

IAX2CallbackHandlerDefault::~IAX2CallbackHandlerDefault()
{

}

void IAX2CallbackHandlerDefault::handleIAXEvent(iaxc_event e)
{
	switch (e.type)
	{
	case IAXC_EVENT_LEVELS:
		event_level(e.ev.levels.input, e.ev.levels.output);
		break;
	case IAXC_EVENT_TEXT:
		event_text(e.ev.text.type, e.ev.text.message);
		break;
	case IAXC_EVENT_STATE:
		event_state(e.ev.call.callNo, e.ev.call.state, e.ev.call.remote, e.ev.call.remote_name,
				e.ev.call.local, e.ev.call.local_context);
		break;
	case IAXC_EVENT_REGISTRATION:
		event_registration(e.ev.reg.id, e.ev.reg.reply, e.ev.reg.msgcount);
		break;
	default:
		Logger::log(Logger::WARNING,
				"Received unknown event. Leaving it not handled.\n");
		break;
	}
}

void IAX2CallbackHandlerDefault::event_registration(int id, int reply,
		int msgcount)
{
	Logger::log(Logger::DEBUG, " REGISTRATION EVENT\n");
	std::vector<SignalingCallback*>::iterator sigIter;
	for (sigIter = signalHandlers.begin(); sigIter != signalHandlers.end(); sigIter++)
	{
		SignalingCallback* callback = *sigIter;
		Account* account = dynamic_cast<SignalingModuleIAX2*> (SignalingModuleIAX2::instance())->findAccountByRegistrationKey(id);
		if (account!=NULL)
			switch (reply)
			{
			case IAXC_REGISTRATION_REPLY_ACK:
			{
				callback->registrationAccepted(account);
				break;
			}
			case IAXC_REGISTRATION_REPLY_REJ:
			{
				callback->registrationRejected(account);
				break;
			}
			case IAXC_REGISTRATION_REPLY_TIMEOUT:
			{
				callback->registrationTimedout(account);
				break;
			}
			}
	}
}

void IAX2CallbackHandlerDefault::event_level(double in, double out)
{
	std::vector<SignalingCallback*>::iterator sigIter;
	for (sigIter = signalHandlers.begin(); sigIter != signalHandlers.end(); sigIter++)
	{
		SignalingCallback* callback = *sigIter;
		callback->inputOutputLevels(in, out);
	}
}

void IAX2CallbackHandlerDefault::event_state(int callNo, int state, char *remote,
		char *remote_name, char *local, char *local_context)
{

	/* Call state masks */
	bool active = state & IAXC_CALL_STATE_ACTIVE;
	bool outgoing = state & IAXC_CALL_STATE_OUTGOING;
	bool ringing = state & IAXC_CALL_STATE_RINGING;
	bool complete = state & IAXC_CALL_STATE_COMPLETE;
	bool selected = state & IAXC_CALL_STATE_SELECTED;

	if (active)
	{ // there is a call progress
		//ougoing calls
		if ((outgoing) && ringing)
		{
			Logger::log(Logger::DEBUG, " ACTIVE_OUTGOING_RINGING : %s\n",
					(const char *) remote);
			std::vector<SignalingCallback*>::iterator sigIter;
			for (sigIter = signalHandlers.begin(); sigIter
					!= signalHandlers.end(); sigIter++)
			{
				SignalingCallback* callback = *sigIter;
				callback->outgoingCall(callNo);
				callback->ringing(callNo);
			}
		}
		// incoming calls
		if ((!outgoing)&& ringing)
		{
			Logger::log(Logger::DEBUG, " ACTIVE_INCOMING_RINGING: %s\n",
					(const char *) remote);
			std::vector<SignalingCallback*>::iterator sigIter;
			for (sigIter = signalHandlers.begin(); sigIter
					!= signalHandlers.end(); sigIter++)
			{
				SignalingCallback* callback = *sigIter;
				callback->incomingCall(callNo);
				callback->ringing(callNo);
			}
		}
		// incoming and outgoing, but active
		if (complete && &selected)
		{
			Logger::log(Logger::DEBUG, " ACTIVE_CALL_ESTABLISHED: %s\n",
					(const char *) remote);
			std::vector<SignalingCallback*>::iterator sigIter;
			for (sigIter = signalHandlers.begin(); sigIter
					!= signalHandlers.end(); sigIter++)
			{
				SignalingCallback* callback = *sigIter;
				callback->activeCallChanged(callNo);
				callback->callComplete(callNo);
			}
		}
	}
	else
	{ // there is no call progress, hangup 
		std::vector<SignalingCallback*>::iterator sigIter;
		for (sigIter = signalHandlers.begin(); sigIter != signalHandlers.end(); sigIter++)
		{
			SignalingCallback* callback = *sigIter;
			callback->callInactive(callNo);
		}
	}
}

void IAX2CallbackHandlerDefault::event_text(int type, char *message)
{

}
