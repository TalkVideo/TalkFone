#ifndef CONFIGURATION_MODULE_IMPL_H
#define CONFIGURATION_MODULE_IMPL_H

#include "ConfigurationModule.h"
#include "StorageService.h"

class ConfigurationModuleImpl : public ConfigurationModule {

 public:
	
	static AbstractModule* instance();
    
	virtual void addParameter(ConfigurationParameter* param);

    virtual void removeParameter(ConfigurationParameter* param);

    virtual std::vector<ConfigurationParameter*> getParameters();

    virtual std::vector<ConfigurationParameter*> getParameters(std::string filter);
    
    virtual void init();
    
    virtual void dispose();
    
 private:
	 std::vector<Serializable*> parameters;
	 ConfigurationModuleImpl();
};


#endif /*CONTACTSMODULEIMPL_H_*/
