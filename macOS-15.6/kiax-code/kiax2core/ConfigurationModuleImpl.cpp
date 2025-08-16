#include "ConfigurationModuleImpl.h"
#include "Core.h"

ConfigurationModule* ConfigurationModule::singleton;

AbstractModule* ConfigurationModuleImpl::instance()
{
	if (singleton==NULL)
	{
		singleton = new ConfigurationModuleImpl();
	}
	return singleton;
}

ConfigurationModuleImpl::ConfigurationModuleImpl()
{
	moduleName = "ConfigurationModuleImpl";
	moduleType = "ConfigurationModule";
}

void ConfigurationModuleImpl::init()
{
	Core* core = Core::instance();
	storageService = core->storageService;
	Logger::log(Logger::INFO, "Module %s initialized.\n", moduleName.data());
}

void ConfigurationModuleImpl::dispose()
{
    Logger::log(Logger::INFO, "Module %s disposed.\n", moduleName.data());
}

void ConfigurationModuleImpl::addParameter(ConfigurationParameter* param)
{
	bool result = storageService->createObject(param);
	if (result)
		Logger::log(Logger::DEBUG, "successuflly created ConfigurationParameter object\n");
	else
		Logger::log(Logger::SEVERE, "could not create Configuration Parameter.\n");
}

void ConfigurationModuleImpl::removeParameter(ConfigurationParameter* param)
{
	bool result = storageService->deleteObject(param);
	if (result)
		Logger::log(Logger::DEBUG, "successuflly deleted ConfigurationParameter object\n");
	else
		Logger::log(Logger::SEVERE, "could not delete ConfigurationParameter.\n");	
}

std::vector<ConfigurationParameter*> ConfigurationModuleImpl::getParameters()
{
	Logger::log(Logger::DEBUG, "getParameters() IN\n");
	std::string objType = "ConfigurationParameter";
	std::vector<ConfigurationParameter*> params_;
	std::string filter = "rowid>=0";
	std::vector<Serializable*> params = storageService->findObject(objType, filter);

	for (unsigned int i = 0; i<params.size(); i++) {
		ConfigurationParameter* param = dynamic_cast<ConfigurationParameter*> (params.at(i));
		params_.push_back(param);
		Logger::log(Logger::DEBUG, "Found object ConfigurationParameter objectId=%d\n",
				param->getObjectId());
	}
	Logger::log(Logger::DEBUG, "getParameters() OUT\n");
	return params_;	
}

std::vector<ConfigurationParameter*> ConfigurationModuleImpl::getParameters(std::string filter)
{
	Logger::log(Logger::DEBUG, "getParameters(%s) IN\n", filter.data());
	std::string objType = "ConfigurationParameter";
	std::string criteria = "paramName='" + filter + "'";
	std::vector<ConfigurationParameter*> params_;
	for (unsigned int i = 0; i<parameters.size(); i++) {
		ConfigurationParameter* param = dynamic_cast<ConfigurationParameter*> (parameters.at(i));
		if (param!=NULL)
		{
			delete param;
			param = NULL;
		}
	}
	parameters.clear();
	parameters = storageService->findObject(objType, criteria);
	 
	for (unsigned int i = 0; i<parameters.size(); i++) {
		ConfigurationParameter* param = dynamic_cast<ConfigurationParameter*> (parameters.at(i));
		params_.push_back(param);
		Logger::log(Logger::DEBUG, "Found object ConfigurationParameter objectId=%d\n",
				param->getObjectId());
	}
	Logger::log(Logger::DEBUG, "getParameters(%s) OUT\n", filter.data());
	return params_;
}
