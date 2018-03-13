#ifndef CONTACTSMODULEIMPL_H_
#define CONTACTSMODULEIMPL_H_

#include "ContactsModule.h"
#include "StorageService.h"

class ContactsModuleImpl : public ContactsModule {

 public:
	
	static AbstractModule* instance();
    
	virtual void addContact(Contact* contact);

    virtual void removeContact(Contact* contact);

    virtual std::vector<Contact*> getContacts();

    virtual void addContact(Contact* contact, Account* account);
    
    virtual std::vector<Contact*> getContacts(std::string filter);
    
    virtual void init();
    
    virtual void dispose();
    
 private:
	 std::vector<Serializable*> contacts;
	 ContactsModuleImpl();
};


#endif /*CONTACTSMODULEIMPL_H_*/
