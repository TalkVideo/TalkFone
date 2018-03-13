#ifndef ConfigurationModule_h
#define ConfigurationModule_h

#include "AbstractModule.h"
#include "StorageAPI.h"
#include "ConfigurationAPI.h"


class ConfigurationModule : public AbstractModule, public ConfigurationAPI {

 public:
	
	static ConfigurationModule* instance();
    
 public:
	 
    StorageAPI* storageService;

 protected:
	 
	static ConfigurationModule* singleton;
    
};

#endif
