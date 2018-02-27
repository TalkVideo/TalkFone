#include "ContactsModuleImpl.h"

#include "Core.h"

ContactsModule* ContactsModule::singleton;

AbstractModule* ContactsModuleImpl::instance()
{
	if (singleton==NULL)
	{
		singleton = new ContactsModuleImpl();
	}
	return singleton;
}

ContactsModuleImpl::ContactsModuleImpl()
{
	moduleName = "ContactsModuleImpl";
	moduleType = "ContactsModule";
}

void ContactsModuleImpl::init()
{
	Core* core = Core::instance();
	storageService = core->storageService;
	Logger::log(Logger::INFO, "Module %s initialized.\n", moduleName.data());
}

void ContactsModuleImpl::dispose()
{
    Logger::log(Logger::INFO, "Module %s disposed.\n", moduleName.data());
}

void ContactsModuleImpl::addContact(Contact* contact)
{
	bool result = storageService->createObject(contact);
	if (result)
		Logger::log(Logger::DEBUG, "successuflly created contact object\n");
	else
		Logger::log(Logger::SEVERE, "could not create contact.\n");
}

void ContactsModuleImpl::addContact(Contact* contact, Account* account)
{
	addContact(contact, account);
}

std::vector<Contact*> ContactsModuleImpl::getContacts()
{
	Logger::log(Logger::DEBUG, "getContacts() IN\n");
	std::string objType = "Contact";
	std::vector<Contact*> contacts_;
	std::string filter = "rowid>=0";
	std::vector<Serializable*> contacts = storageService->findObject(objType, filter);

	for (unsigned int i = 0; i<contacts.size(); i++) {
		Contact* contact = dynamic_cast<Contact*> (contacts.at(i));
		contacts_.push_back(contact);
		Logger::log(Logger::DEBUG, "Found object Contact objectId=%d\n",
				contact->getObjectId());
	}
	Logger::log(Logger::DEBUG, "getContacts() OUT\n");
	return contacts_;	
}

void ContactsModuleImpl::removeContact(Contact* contact)
{
	bool result = storageService->deleteObject(contact);
	if (result)
		Logger::log(Logger::DEBUG, "successuflly deleted contact object\n");
	else
		Logger::log(Logger::SEVERE, "could not delete contact.\n");	
}

std::vector<Contact*> ContactsModuleImpl::getContacts(string filter)
{
	Logger::log(Logger::DEBUG, "getContacts(%s) IN\n", filter.data());
	std::string objType = "Contact";
	std::string criteria = "extension like '%" + filter + "%' OR contactName like '%" + filter + "%'";
	std::vector<Contact*> contacts_;
	for (unsigned int i = 0; i<contacts.size(); i++) {
		Contact* contact = dynamic_cast<Contact*> (contacts.at(i));
		if (contact!=NULL)
		{
			delete contact;
			contact = NULL;
		}
	}
	contacts.clear();
	contacts = storageService->findObject(objType, criteria);
	 
	for (unsigned int i = 0; i<contacts.size(); i++) {
		Contact* contact = dynamic_cast<Contact*> (contacts.at(i));
		contacts_.push_back(contact);
		Logger::log(Logger::DEBUG, "Found object Contact objectId=%d\n",
				contact->getObjectId());
	}
	Logger::log(Logger::DEBUG, "getContacts(%s) OUT\n", filter.data());
	return contacts_;
}
