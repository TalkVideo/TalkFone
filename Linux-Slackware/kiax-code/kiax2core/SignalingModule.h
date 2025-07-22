#ifndef SignalingModule_h
#define SignalingModule_h

#include <vector>

#include "AbstractModule.h"
#include "Account.h"
#include "SignalingCallback.h"
#include "SignalingAPI.h"


class SignalingModule : public AbstractModule, public SignalingAPI {

 public:
	
	static SignalingModule* instance();
    
 public:
    int signalingProtocol;
    int maxNumberOfCalls;
    
 protected:

    static SignalingModule* singleton;

};

#endif
