#ifndef Core_h
#define Core_h

#include <vector>
#include "PConstants.h"
#include "PConfiguration.h"
#include "APIBinder.h"
#include "AbstractModule.h"
#include "AbstractService.h"
#include "AccountsModule.h"
#include "CDRModule.h"
#include "Configuration.h"
#include "ContactsModule.h"
#include "NamingService.h"
#include "SignalingModule.h"
#include "StorageService.h"
#include "CDRModuleImpl.h"
#ifdef REMOTE_CDR
#include "CDRModuleRemXml.h"
#endif
#include "ContactsModuleImpl.h"
#include "AccountsModuleImpl.h"
#include "SignalingModuleIAX2.h"
#include "ConfigurationModuleImpl.h"

class Core {

 private:
	
	Core();
	static Core* coreInstance; //singleton
	
 public:

	static Core* instance();
	static Core* instance(std::vector<string> moduleNames);
	static void destroy();
	
	virtual ~Core();
	 
    void loadModules();

    void unloadModules();

    static AbstractService* getService(std::string serviceName, bool warning = true);
    
    void startService(std::string serviceName);

    void stopService(std::string serviceName);

 public:

    std::vector< AbstractModule* > modules;
    
    NamingService* namingService;
    
    StorageService* storageService;

    Configuration* coreConfiguration;
	
	static std::map<string, string> configuration;

};

#endif
