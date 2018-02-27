#ifndef Account_h
#define Account_h

#include "Serializable.h"

class Account : public Serializable {

 public:
	 
	Account();
	~Account();
	std::string getCreateTypeQuery();
	std::string getCreateQuery();
	std::string getStoreQuery();
	std::string getDeleteQuery();
	std::string getLoadQuery(int objectId);
	
 public:
    std::string serviceName;
    std::string username;
    std::string password;
    std::string host1;
    std::string host2;
    std::string host3;

};

#endif
