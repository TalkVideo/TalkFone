#ifndef NamingService_h
#define NamingService_h

#include <vector>
#include <map>
#include "PConstants.h"
#include "AbstractModule.h"
#include "AbstractService.h"

class NamingService : public AbstractService {

 public:
	
	static NamingService* instance(); //returns singleton instance
    
	virtual AbstractModule* findModuleInterface(std::string moduleName);

    virtual void bindModule(std::string  moduleName, AbstractModule* instance);

    virtual void unbindModule(std::string moduleName);

    virtual std::vector<AbstractModule*> findAllModules();
    
    virtual bool start();
    virtual bool stop();

 private:
	 
	static NamingService* singleton;
	NamingService();

	map<std::string, AbstractModule*> modules;
};
#endif
