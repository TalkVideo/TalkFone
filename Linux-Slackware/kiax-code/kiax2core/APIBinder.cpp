#include "APIBinder.h"


AccountsAPI* APIBinder::getAccountsAPI()
{
	return boundAccountsModule;
}

ContactsAPI* APIBinder::getContactsAPI()
{
	return boundContactsModule;
}

CDRAPI* APIBinder::getCDRAPI()
{
	return boundCDRModule;
}

SignalingAPI* APIBinder::getSignalingAPI()
{
	return boundSignalingModule;
}

StorageAPI* APIBinder::getStorageAPI()
{
	return boundStorageService;
}

ConfigurationAPI* APIBinder::getConfigurationAPI()
{
	return boundConfigurationModule;
}

void APIBinder::logCDR(CDR* cdr)
{
	boundCDRModule->logCDR(cdr);
}

std::vector<CDR*> APIBinder::getCDRs()
{
	return boundCDRModule->getCDRs();
}

std::vector<CDR*> APIBinder::getCDRs(std::string filter)
{
	return boundCDRModule->getCDRs(filter);
}

void APIBinder::addContact(Contact* contact)
{
	boundContactsModule->addContact(contact);
}

void APIBinder::removeContact(Contact* contact)
{
	boundContactsModule->removeContact(contact);
}

std::vector<Contact*> APIBinder::getContacts()
{
	return boundContactsModule->getContacts();
}

void APIBinder::addContact(Contact* contact, Account* account)
{
	boundContactsModule->addContact(contact, account);
}

std::vector<Contact*> APIBinder::getContacts(std::string filter)
{
	return boundContactsModule->getContacts(filter);
}

void APIBinder::addAccount(Account* account)
{
	boundAccountsModule->addAccount(account);
}

void APIBinder::removeAccount(Account* account)
{
	boundAccountsModule->removeAccount(account);
}

void APIBinder::setDefaultAccount(Account* account)
{
	boundAccountsModule->setDefaultAccount(account);
}

std::vector<Account*> APIBinder::getAccounts()
{
	return boundAccountsModule->getAccounts();
}

void APIBinder::registerAccount(Account* account)
{
	boundSignalingModule->registerAccount(account);
}

void APIBinder::unregisterAccount(Account* account)
{
	boundSignalingModule->unregisterAccount(account);
}

int APIBinder::dial(std::string extension, Account* account)
{
	return boundSignalingModule->dial(extension, account);
}

void APIBinder::hangup(int callNumber)
{
	boundSignalingModule->hangup(callNumber);
}

void APIBinder::reject(int callNumber)
{
	boundSignalingModule->reject(callNumber);
}

void APIBinder::answer(int callNumber)
{
	boundSignalingModule->answer(callNumber);
}

void APIBinder::sendMessage(std::string message)
{
	boundSignalingModule->sendMessage(message);
}

void APIBinder::transferCall(std::string extension, int callNumber)
{
	boundSignalingModule->transferCall(extension, callNumber);
}

void APIBinder::setActiveCall(int activeCallNumber)
{
	boundSignalingModule->setActiveCall(activeCallNumber);
}

int APIBinder::getActiveCallNumber()
{
	return boundSignalingModule->getActiveCallNumber();
}

void APIBinder::addSignalHandler(SignalingCallback* handler)
{
	boundSignalingModule->addSignalHandler(handler);
}

void APIBinder::removeSignalHandler(int handlerNumber)
{
	boundSignalingModule->removeSignalHandler(handlerNumber);
}

void APIBinder::sendDTMF(char tone, int callNumber)
{
	boundSignalingModule->sendDTMF(tone, callNumber);
}

void APIBinder::muteCall(int callNumber)
{
	boundSignalingModule->muteCall(callNumber);
}

void APIBinder::unmuteCall(int callNumber)
{
	boundSignalingModule->unmuteCall(callNumber);
}

bool APIBinder::createObject(Serializable* proto)
{
	return boundStorageService->createObject(proto);
}

bool APIBinder::updateObject(Serializable* proto)
{
	return boundStorageService->updateObject(proto);
}

std::vector<Serializable*> APIBinder::findObject(std::string& objectType, std::string& criteria)
{
	return boundStorageService->findObject(objectType, criteria);
}

bool APIBinder::deleteObject(Serializable* object)
{
	return boundStorageService->deleteObject(object);
}

void APIBinder::addParameter(ConfigurationParameter* param)
{
	boundConfigurationModule->addParameter(param);
}

void APIBinder::removeParameter(ConfigurationParameter* param)
{
	boundConfigurationModule->removeParameter(param);
}

std::vector<ConfigurationParameter*> APIBinder::getParameters()
{
	return boundConfigurationModule->getParameters();
}

std::vector<ConfigurationParameter*> APIBinder::getParameters(std::string filter)
{
	return boundConfigurationModule->getParameters(filter);
}
