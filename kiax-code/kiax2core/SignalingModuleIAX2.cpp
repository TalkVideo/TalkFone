#include "Core.h"

SignalingModule* SignalingModule::singleton;
IAX2CallbackHandler* SignalingModuleIAX2::handler;

int iaxc_callback(iaxc_event e) {
	if (SignalingModuleIAX2::handler!=NULL)
		SignalingModuleIAX2::handler->handleIAXEvent(e);
    return 1;
}

Account* SignalingModuleIAX2::findAccountByRegistrationKey(int regId)
{
	for( map<Account*, int>::iterator iter = registrations.begin(); iter != registrations.end(); iter++ ) {
		if (regId == (*iter).second)
			return (*iter).first;
	}
	return NULL; // not found
}

SignalingModule* SignalingModuleIAX2::instance()
{
	if (singleton==NULL)
	{
		singleton = new SignalingModuleIAX2();
	}
	return singleton;
}

void SignalingModuleIAX2::setIAX2CallbackHandler(IAX2CallbackHandler* callbackHandler)
{
	handler = callbackHandler;
}

SignalingModuleIAX2::SignalingModuleIAX2()
{
	moduleName = "SignalingModuleIAX2";
	moduleType = "SignalingModule";
	signalingProtocol = PROTOCOL_IAX2;
	maxNumberOfCalls = DEFAULT_CALLS_NUMBER;
}

void SignalingModuleIAX2::init()
{
    if (iaxc_initialize(maxNumberOfCalls)) 
    {
		Logger::log(Logger::DEBUG, "cannot initialize iaxclient!\n");
    } 
    iaxc_set_silence_threshold(-99.0); /* the default */
    iaxc_set_audio_output(0);	/* the default */ 
    iaxc_set_event_callback(iaxc_callback); 
    iaxc_set_filters(IAX2_FILTERS) ;
 	iaxc_set_formats(IAXC_FORMAT_ULAW,IAX2_CAPABILITIES_SPEEX);
    iaxc_start_processing_thread(); 
    Logger::log(Logger::INFO, "Module %s initialized.\n", moduleName.data());
}

void SignalingModuleIAX2::dispose()
{
    registrations.clear();
    iaxc_dump_all_calls();
    iaxc_stop_processing_thread();
    Logger::log(Logger::INFO, "Module %s disposed.\n", moduleName.data());
}

void SignalingModuleIAX2::registerAccount(Account* account)
{
	int sessionId = iaxc_register((const char*)account->username.data(), 
										(const char*)account->password.data(),
										(const char*)account->host1.data());
	registrations[account] = sessionId;
}

void SignalingModuleIAX2::unregisterAccount(Account* account)
{
	int sessionId = registrations[account];
	iaxc_unregister(sessionId);
	registrations.erase(account);
}

int SignalingModuleIAX2::dial(std::string extension, Account* account)
{
	iaxc_set_callerid("TalkVideo Network","2025550000");
	std::string dialURL = account->username + ":" + account->password + "@" + account->host1 + "/" + extension;
	Logger::log(Logger::DEBUG, "IAX2 dial URL = %s\n", dialURL.data());
	int callNumber = iaxc_call(dialURL.data());
	Logger::log(Logger::DEBUG, "NEW CALL NUMBER IS %d\n", callNumber);
	return callNumber;
}

void SignalingModuleIAX2::hangup(int callNumber)
{
	iaxc_dump_call();
}

void SignalingModuleIAX2::reject(int callNumber)
{
	iaxc_reject_call_number(callNumber);
}

void SignalingModuleIAX2::answer(int callNumber)
{
	iaxc_answer_call(callNumber);
}

void SignalingModuleIAX2::sendMessage(std::string message)
{
	iaxc_send_text((const char*) message.data());
}

void SignalingModuleIAX2::transferCall(std::string extension, int callNumber)
{
	iaxc_blind_transfer_call(callNumber, extension.data());
}

void SignalingModuleIAX2::setActiveCall(int activeCallNumber)
{
	iaxc_select_call(activeCallNumber);	
}

void SignalingModuleIAX2::sendDTMF(char tone, int callNumber)
{
	iaxc_send_dtmf(tone);
}

void SignalingModuleIAX2::muteCall(int callNumber)
{
#if defined(IAXCLIENT_HOLD_HACK)
	iaxc_hold(callNumber);
#endif
}

void SignalingModuleIAX2::unmuteCall(int callNumber)
{
#if defined(IAXCLIENT_HOLD_HACK)
	iaxc_unhold(callNumber); 
#endif
}

void SignalingModuleIAX2::addSignalHandler(SignalingCallback* sighandler)
{
	handler->signalHandlers.push_back(sighandler);
}

void SignalingModuleIAX2::removeSignalHandler(int handlerNumber)
{
	handler->signalHandlers.erase(handler->signalHandlers.begin() + handlerNumber);
}

int SignalingModuleIAX2::getActiveCallNumber()
{
	return iaxc_selected_call();
}

