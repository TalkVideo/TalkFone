#include "NamingService.h"
#include "Logger.h"

NamingService* NamingService::singleton;

NamingService* NamingService::instance()
{
	if (NamingService::singleton==NULL)
	{
		NamingService::singleton = new NamingService();
	}
	return NamingService::singleton;
}

NamingService::NamingService() : AbstractService() 
{
	serviceType = SERVICE_TYPE_NAMING;
}

bool NamingService::start()
{
	started = true;
	return true;
}

bool NamingService::stop()
{
	modules.clear();
	started = false;
	return true;
}

AbstractModule* NamingService::findModuleInterface(std::string moduleName)
{

	map<string,AbstractModule*>::iterator iter;
	iter = modules.find(moduleName);
	if (iter!=modules.end()) return iter->second;
	
	return NULL;
}
 
void NamingService::bindModule(std::string moduleName, AbstractModule* instance)
{
    modules[moduleName] = instance;
}

void NamingService::unbindModule(std::string moduleName)
{
    modules.erase(moduleName);
}

std::vector<AbstractModule*> NamingService::findAllModules()
{
	std::vector<AbstractModule*> modulesVector;
	map<string,AbstractModule*>::iterator iter;   
	for( iter = modules.begin(); iter != modules.end(); iter++ ) {
	    modulesVector.push_back(iter->second);
		//Logger::log(Logger::INFO, "found module %s\n", ((AbstractModule*)iter->second)->moduleName.data());
	}
	return modulesVector;
}

