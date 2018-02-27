#ifndef STORAGEAPI_H_
#define STORAGEAPI_H_

#include "Serializable.h"

class StorageAPI {

 public:

	virtual bool createObject(Serializable* proto) = 0;
	
	virtual bool updateObject(Serializable* proto) = 0;

    virtual std::vector<Serializable*> findObject(std::string& objectType, std::string& criteria) = 0;

    virtual bool deleteObject(Serializable* object) = 0;
    
    virtual ~StorageAPI() {}

};
#endif /*STORAGEAPI_H_*/
