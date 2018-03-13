#ifndef Configuration_h

#define Configuration_h

#include <vector>

#include "ConfigurationParameter.h"
#include "Serializable.h"


class Configuration : public Serializable {

 public:
	std::string	getCreateTypeQuery();
	std::string getStoreQuery();
	std::string	getCreateQuery();
	std::string getDeleteQuery();
	std::string getLoadQuery(int objectId);

 public:
    std::string name;

 public:

    /**
     * @element-type ConfigurationParameter
     */
    std::vector< ConfigurationParameter* > entries;

};

#endif

