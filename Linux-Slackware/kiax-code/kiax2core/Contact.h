#ifndef Contact_h

#define Contact_h

#include "Serializable.h"
#include "Account.h"

class Contact : public Serializable {

public:
	
	Contact();
	virtual ~Contact();
	std::string getCreateTypeQuery();
	std::string getCreateQuery();
	std::string getStoreQuery();
	std::string getDeleteQuery();
	std::string getLoadQuery(int objectId);
	
 public:
    std::string contactName;
    std::string extension;
    Account* account;
};

#endif
