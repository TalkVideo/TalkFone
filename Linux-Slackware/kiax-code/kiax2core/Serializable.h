#ifndef Serializable_h
#define Serializable_h

#include <string>

using namespace std;

class Serializable {

 public:

	Serializable();
	
	virtual ~Serializable();
	
	virtual std::string getCreateTypeQuery() = 0;

	virtual std::string getCreateQuery() = 0;
	 
    virtual std::string getStoreQuery() = 0;

    virtual std::string getDeleteQuery() = 0;

    virtual std::string getLoadQuery(int id) = 0;
    
    virtual std::string getObjectType();
    
    virtual string* getColumnsNames();
    
    virtual int getColumnsNumber();
    
    virtual void setObjectId(long int id);
    virtual long int getObjectId();
    
 protected:
	 
	long int objectId;
    string objectType;
    string* columnsNames;
    int columnsNumber;

};

#endif
