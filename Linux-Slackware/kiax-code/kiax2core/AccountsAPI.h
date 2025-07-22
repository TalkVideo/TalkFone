#ifndef ACCOUNTSAPI_H_
#define ACCOUNTSAPI_H_

class AccountsAPI {

 public:
  
	virtual void addAccount(Account* account) = 0;

    virtual void removeAccount(Account* account) = 0;

    virtual void setDefaultAccount(Account* account) = 0;

    virtual std::vector<Account*> getAccounts() = 0;

    virtual ~AccountsAPI() {}
	
};

#endif /*ACCOUNTSAPI_H_*/
