#ifndef APIBinder_h
#define APIBinder_h

#include <vector>
#include "SignalingAPI.h"
#include "AccountsAPI.h"
#include "ContactsAPI.h"
#include "CDRAPI.h"
#include "StorageAPI.h"
#include "ConfigurationAPI.h"


class APIBinder : public CDRAPI, public SignalingAPI, public ContactsAPI, public AccountsAPI, public StorageAPI, public ConfigurationAPI {

 public:

	static APIBinder* instance();

    virtual CDRAPI* getCDRAPI();

    virtual AccountsAPI* getAccountsAPI();

    virtual ContactsAPI* getContactsAPI();

    virtual SignalingAPI* getSignalingAPI();

    virtual StorageAPI* getStorageAPI();

    virtual ConfigurationAPI* getConfigurationAPI();
    
    virtual void logCDR(CDR* cdr);
    
    virtual std::vector<CDR*> getCDRs();

    virtual std::vector<CDR*> getCDRs(std::string filter) ;    

	virtual void addContact(Contact* contact);

    virtual void removeContact(Contact* contact);

    virtual std::vector<Contact*> getContacts();

    virtual void addContact(Contact* contact, Account* account);
    
    virtual std::vector<Contact*> getContacts(std::string filter);
    
    virtual void addAccount(Account* account);

    virtual void removeAccount(Account* account);

    virtual void setDefaultAccount(Account* account);

    virtual std::vector<Account*> getAccounts();

	virtual void registerAccount(Account* account);
	
	virtual void unregisterAccount(Account* account);
	
	virtual int dial(std::string extension, Account* account);

    virtual void hangup(int callNumber);

    virtual void reject(int callNumber);

    virtual void answer(int callNumber);

    virtual void sendMessage(std::string message);

    virtual void transferCall(std::string extension, int callNumber);

    virtual void setActiveCall(int activeCallNumber);

    virtual void addSignalHandler(SignalingCallback* handler);
    
    virtual void removeSignalHandler(int handlerNumber);

    virtual void sendDTMF(char tone, int callNumber);
    
    virtual void muteCall(int callNumber);

    virtual void unmuteCall(int callNumber);
	
	virtual int getActiveCallNumber();

	virtual void addParameter(ConfigurationParameter* param);

    virtual void removeParameter(ConfigurationParameter* param);

    virtual std::vector<ConfigurationParameter*> getParameters();
    
    virtual std::vector<ConfigurationParameter*> getParameters(std::string filter);
	
	virtual bool createObject(Serializable* proto);

	virtual bool updateObject(Serializable* proto);

    virtual std::vector<Serializable*> findObject(std::string& objectType, std::string& criteria);

    virtual bool deleteObject(Serializable* object);    

    virtual ~APIBinder() {}
    
 public:
	std::string binderType;
	std::string binderName;
	
 protected:
	
	static APIBinder* singleton;
	CDRAPI* boundCDRModule;
	ContactsAPI* boundContactsModule;
	AccountsAPI* boundAccountsModule;
	SignalingAPI* boundSignalingModule;
	StorageAPI* boundStorageService;
	ConfigurationAPI* boundConfigurationModule;

};

#endif
