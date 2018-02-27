#ifndef ConfigurationParameter_h

#define ConfigurationParameter_h

#include "Serializable.h"


class ConfigurationParameter : public Serializable {

 public:
	ConfigurationParameter();
	~ConfigurationParameter();
	std::string getCreateTypeQuery();
	std::string getStoreQuery();
	std::string getCreateQuery();
	std::string getDeleteQuery();
	std::string getLoadQuery(int objectId);
	ConfigurationParameter* clone();

 public:
    std::string value;
    std::string name;
    std::string paramType;
};

#endif
