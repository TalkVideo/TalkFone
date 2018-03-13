#ifndef ContactsModule_h
#define ContactsModule_h

#include "AbstractModule.h"
#include "StorageAPI.h"
#include "ContactsAPI.h"


class ContactsModule : public AbstractModule, public ContactsAPI {

 public:
	
	static ContactsModule* instance();
    
 public:
	 
    Contact* myContact;
    StorageAPI* storageService;

 protected:
	 
	static ContactsModule* singleton;
    
};

#endif
