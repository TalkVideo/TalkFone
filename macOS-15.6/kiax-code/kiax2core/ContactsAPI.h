#ifndef CONTACTSAPI_H_
#define CONTACTSAPI_H_

#include "Contact.h"
#include "Account.h"

class ContactsAPI {

 public:
    
	virtual void addContact(Contact* contact) = 0;

    virtual void removeContact(Contact* contact) = 0;

    virtual std::vector<Contact*> getContacts() = 0;

    virtual void addContact(Contact* contact, Account* account) = 0;

    virtual std::vector<Contact*> getContacts(std::string filter) = 0;

    virtual ~ContactsAPI() {}
    
};
#endif /*CONTACTSAPI_H_*/
