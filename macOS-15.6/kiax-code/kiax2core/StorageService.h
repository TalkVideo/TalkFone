#ifndef StorageService_h
#define StorageService_h

#include <vector>
#include <sqlite3.h>
#include "PConfiguration.h"
#include "AbstractService.h"
#include "Serializable.h"
#include "Contact.h"
#include "CDR.h"
#include "Configuration.h"
#include "ConfigurationParameter.h"
#include "Account.h"
#include "Logger.h"
#include "StorageAPI.h"
#ifdef WIN32DEP
#include <cstdlib.>
#endif
#ifdef LINUXDEP
#include <sys/stat.h>
#include <sys/types.h>
#endif

class StorageService : public AbstractService, public StorageAPI {

 public:

	virtual ~StorageService();
	 
	virtual bool start();
	
	virtual bool stop();
	
    virtual bool createObject(Serializable* proto);

    virtual bool updateObject(Serializable* proto);

    virtual std::vector<Serializable*> findObject(std::string& objectType, std::string& criteria);

    virtual bool deleteObject(Serializable* object);

    static std::string touchProfileDir(bool homeDir = true); // if homeDir data is stored in user home, else on current working dir

    static int touchDir(const char* dirName);

 public:
	 
	static StorageService* instance(); //returns singleton instance
	vector<std::string> rowids;
	vector<Serializable*> objectsPool;
    
 private:
	 
	static StorageService* singleton;
	StorageService(std::string& name);
 
 protected:
	 string dbName;
	 sqlite3 *db;
	 sqlite3_mutex* mutex;
	 
};

#endif
