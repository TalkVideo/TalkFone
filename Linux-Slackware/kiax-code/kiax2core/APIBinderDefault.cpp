#include "APIBinderDefault.h"
#include "Core.h"

APIBinder* APIBinder::singleton;

APIBinder* APIBinderDefault::instance()
{
	if (singleton==NULL)
	{
		singleton = new APIBinderDefault();
	}
	return singleton;
}

APIBinderDefault::APIBinderDefault()
{
	NamingService* naming = dynamic_cast<NamingService*>(Core::instance()->getService(NAMING_SERVICE_NAME));
	boundCDRModule = dynamic_cast<CDRAPI*> (naming->findModuleInterface(DEFAULT_CDR_MODULE_NAME));
	boundContactsModule = dynamic_cast<ContactsAPI*> (naming->findModuleInterface(DEFAULT_CONTACTS_MODULE_NAME));
	boundAccountsModule = dynamic_cast<AccountsAPI*> (naming->findModuleInterface(DEFAULT_ACCOUNTS_MODULE_NAME));
	boundSignalingModule = dynamic_cast<SignalingAPI*> (naming->findModuleInterface(DEFAULT_SIGNALING_MODULE_NAME));
	boundConfigurationModule = dynamic_cast<ConfigurationAPI*> (naming->findModuleInterface(DEFAULT_CONFIGURATION_MODULE_NAME));
	boundStorageService = dynamic_cast<StorageAPI*>(Core::instance()->getService(STORAGE_SERVICE_NAME));
}

