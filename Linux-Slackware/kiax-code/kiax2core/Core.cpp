#include "Core.h"

Core* Core::coreInstance;
map<string, string> Core::configuration;
Core* Core::instance(std::vector<string> moduleNames)
{
	if (coreInstance==NULL)
	{
		coreInstance = new Core();
		coreInstance->startService(NAMING_SERVICE_NAME);
		coreInstance->startService(STORAGE_SERVICE_NAME);
		// add hardcoded modules
		NamingService* naming = dynamic_cast<NamingService*>(Core::getService(NAMING_SERVICE_NAME));

		for (unsigned int i=0; i<moduleNames.size();i++)
		{
			if (moduleNames[i].compare("account")==0) 
			{
				AccountsModule* accountsModule = dynamic_cast<AccountsModule*> (HARDCODED_ACCOUNTS_MODULE::instance());
				accountsModule->init();
				naming->bindModule(accountsModule->moduleName, accountsModule);
			} 
			if (moduleNames[i].compare("configuration")==0)
			{
				ConfigurationModule* configurationModule = dynamic_cast<ConfigurationModule*> (HARDCODED_CONFIGURATION_MODULE::instance());
				configurationModule->init();
				naming->bindModule(configurationModule->moduleName, configurationModule);
			}
			if (moduleNames[i].compare("contact")==0)
			{
				ContactsModule* contactsModule = dynamic_cast<ContactsModule*> (HARDCODED_CONTACTS_MODULE::instance());
				contactsModule->init();
				naming->bindModule(contactsModule->moduleName, contactsModule);
			}
			if (moduleNames[i].compare("signaling")==0)
			{
				SignalingModule* signalingModule = dynamic_cast<SignalingModule*> (HARDCODED_SIGNALING_MODULE::instance());
				signalingModule->init();
				naming->bindModule(signalingModule->moduleName, signalingModule);
			}
			if (moduleNames[i].compare("cdr")==0)
			{			
				CDRModule* cdrModule = dynamic_cast<CDRModule*> (HARDCODED_CDR_MODULE::instance());
				cdrModule->init();
				naming->bindModule(cdrModule->moduleName, cdrModule);
			}
		}

	}
	return coreInstance;
}

Core* Core::instance()
{
	if (coreInstance==NULL)
	{
		coreInstance = new Core();
		coreInstance->startService(NAMING_SERVICE_NAME);
		coreInstance->startService(STORAGE_SERVICE_NAME);
		coreInstance->loadModules();
	}
	return coreInstance;
}

void Core::destroy()
{
	if (coreInstance!=NULL)
	{
		coreInstance->unloadModules();
		coreInstance->stopService(STORAGE_SERVICE_NAME);
		coreInstance->stopService(NAMING_SERVICE_NAME);
		coreInstance=NULL;
	}
}

Core::Core()
{
	// set any services, modules need them
	namingService = dynamic_cast<NamingService*>(getService(NAMING_SERVICE_NAME, false));
	storageService = dynamic_cast<StorageService*>(getService(STORAGE_SERVICE_NAME, false));
}

Core::~Core()
{

}

void Core::loadModules()
{
	// add hardcoded modules
	NamingService* naming = dynamic_cast<NamingService*>(getService(NAMING_SERVICE_NAME));
	CDRModule* cdrModule = dynamic_cast<CDRModule*> (HARDCODED_CDR_MODULE::instance());
	cdrModule->init();
	ContactsModule* contactsModule = dynamic_cast<ContactsModule*> (HARDCODED_CONTACTS_MODULE::instance());
	contactsModule->init();
	AccountsModule* accountsModule = dynamic_cast<AccountsModule*> (HARDCODED_ACCOUNTS_MODULE::instance());
	accountsModule->init();
	SignalingModule* signalingModule = dynamic_cast<SignalingModule*> (HARDCODED_SIGNALING_MODULE::instance());
	signalingModule->init();
	ConfigurationModule* configurationModule = dynamic_cast<ConfigurationModule*> (HARDCODED_CONFIGURATION_MODULE::instance());
	configurationModule->init();
	naming->bindModule(contactsModule->moduleName, contactsModule);
	naming->bindModule(cdrModule->moduleName, cdrModule);
	naming->bindModule(accountsModule->moduleName, accountsModule);
	naming->bindModule(signalingModule->moduleName, signalingModule);
	naming->bindModule(configurationModule->moduleName, configurationModule);

}

void Core::unloadModules()
{
	NamingService* naming = dynamic_cast<NamingService*>(getService(NAMING_SERVICE_NAME));

	std::vector<AbstractModule*> modules = naming->findAllModules();
	int modSize = modules.size();
	for (int i=0;i<modSize;i++)
	{
		AbstractModule* module = modules[i];
		string moduleName = module->moduleName;
		naming->unbindModule(moduleName);
		module->dispose();
		delete module;
	}
}

// lazy implemented service name binding
// returns always the hardcoded naming and storage services
AbstractService* Core::getService(std::string serviceName, bool warning)
{
	AbstractService* service = NULL;
	if (serviceName.compare(NAMING_SERVICE_NAME)==0)
	{
		service = HARDCODED_NAMING_SERVICE::instance();
	} 
	else if (serviceName.compare(STORAGE_SERVICE_NAME)==0)
	{
		service = HARDCODED_STORAGE_SERVICE::instance();
	}
	else
	{
		//Logger::log(Logger::ERROR, "Cannot get instance of service %s\n", serviceName.data());
	}
	if (service!=NULL)
		if ((!service->isStarted())&&warning)
		{
			Logger::log(Logger::WARNING, "Service %s not started. Please start service.\n", serviceName.data());		
		}
	return service;
	
}

void Core::startService(std::string serviceName)
{
    AbstractService* service = getService(serviceName, false);
    if (service!=NULL)
    {
    	bool startResult = service->start();
    	if (startResult)
    	{
    		Logger::log(Logger::INFO, "Service %s started successfully.\n", serviceName.data());
    	}
    	else
    	{
    		//Logger::log(Logger::ERROR, "Service %s could not be started. Service error code is %d.\n", serviceName.data(), service->getErrorCode());
    	}
    }
    else
    {
    	//Logger::log(Logger::ERROR, "Cannot start service %s. Service does not exist.\n", serviceName.data());
    }
}

void Core::stopService(std::string serviceName)
{
    AbstractService* service = getService(serviceName);
    if (service!=NULL)
    {
    	bool stopResult = service->stop();
    	if (stopResult)
    	{
    		Logger::log(Logger::INFO, "Service %s stopped successfully.\n", serviceName.data());
    	}
    	else
    	{
    		//Logger::log(Logger::ERROR, "Service %s could not be stopped. Service error code is %d.\n", serviceName.data(), service->getErrorCode());
    	}
    }
    else
    {
    	//Logger::log(Logger::ERROR, "Cannot stop service %s. Service does not exist.\n", serviceName.data());
    }
    
}
