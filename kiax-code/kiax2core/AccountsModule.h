#ifndef AccountsModule_h
#define AccountsModule_h

#include <vector>

#include "Account.h"
#include "AbstractModule.h"
#include "AbstractService.h"
#include "AccountsAPI.h"

class AccountsModule : public AbstractModule, public AccountsAPI {

 public:

	static AccountsModule* instance();
    
 public:
	 
    Account* defaultAccount;
    std::vector< Account* > accounts;
    
 protected:

	static AccountsModule* singleton;
	AbstractService* storageService;
	
};
#endif
