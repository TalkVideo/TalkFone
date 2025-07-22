#include "AccountsModuleImpl.h"
#include "Core.h"

AccountsModule* AccountsModule::singleton;

AccountsModule* AccountsModuleImpl::instance()
{
	if (singleton==NULL)
	{
		singleton = new AccountsModuleImpl();
	}
	return singleton;	
}

AccountsModuleImpl::AccountsModuleImpl()
{
	moduleName = "AccountsModuleImpl";
	moduleType = "AccountsModule";
}

void AccountsModuleImpl::init()
{
	Core* core = Core::instance();
	storageService = core->storageService;
	Logger::log(Logger::INFO, "Module %s initialized.\n", moduleName.data());
}

void AccountsModuleImpl::dispose()
{
    Logger::log(Logger::INFO, "Module %s disposed.\n", moduleName.data());
}

void AccountsModuleImpl::addAccount(Account* account)
{
	Logger::log(Logger::DEBUG, "addAccount() IN\n");
	bool createResult = (dynamic_cast<StorageService*>(storageService))->createObject(account);
	if (createResult) 
	{
		Logger::log(Logger::DEBUG, "Account created successfully.\n");
	}
	else
	{
		Logger::log(Logger::SEVERE, "Account creation failed\n");
	}
	Logger::log(Logger::DEBUG, "addAccount() OUT\n");
}

std::vector<Account*> AccountsModuleImpl::getAccounts()
{
	Logger::log(Logger::DEBUG, "getAccountss() IN\n");
	StorageService* storage = dynamic_cast<StorageService*> (storageService);
	std::string objType = "Account";
	std::vector<Account*> accounts_;
	std::string filter = "rowid>=0";
	for (unsigned int i = 0; i<accounts.size(); i++) {
		Account* account = dynamic_cast<Account*> (accounts.at(i));
		if (account!=NULL)
		{
			delete account;
			account = NULL;
		}
	}
	accounts.clear();
	accounts = storage->findObject(objType, filter);
	for (unsigned int i = 0; i<accounts.size(); i++) {
		Account* account = dynamic_cast<Account*> (accounts.at(i));
		accounts_.push_back(account);
		Logger::log(Logger::DEBUG, "Found object Contact objectId=%d\n",
				account->getObjectId());
	}
	Logger::log(Logger::DEBUG, "getAccountss() OUT\n");
	return accounts_;
}

void AccountsModuleImpl::removeAccount(Account* account)
{
	bool deleteResult = (dynamic_cast<StorageService*>(storageService))->deleteObject(account);
	if (deleteResult)
		Logger::log(Logger::DEBUG, "Account deleted successfully.\n");
	else
		Logger::log(Logger::SEVERE, "Account deletion failed\n");	
}

void AccountsModuleImpl::setDefaultAccount(Account* account)
{
	defaultAccount = account;
}

