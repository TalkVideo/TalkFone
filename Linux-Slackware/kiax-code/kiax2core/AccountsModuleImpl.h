#ifndef ACCOUNTSMODULEIMPL_H_
#define ACCOUNTSMODULEIMPL_H_


#include <vector>

#include "Account.h"
#include "AbstractModule.h"
#include "AbstractService.h"
#include "AccountsModule.h"

class AccountsModuleImpl : public AccountsModule {

 public:

	static AccountsModule* instance();
    
	virtual void addAccount(Account* account);

    virtual void removeAccount(Account* account);

    virtual void setDefaultAccount(Account* account);

    virtual std::vector<Account*> getAccounts();
    
    virtual void init();
    
    virtual void dispose();

 public:
	 
    Account* defaultAccount;
    
 protected:

	AbstractService* storageService;
	
 private:
	std::vector<Serializable*> accounts;
	AccountsModuleImpl();
	
};

#endif /*ACCOUNTSMODULEIMPL_H_*/
