#ifndef CONFIGURATION_API_H_
#define CONFIGURATION_API_H_

#include <vector>
#include "ConfigurationParameter.h"

class ConfigurationAPI {

 public:

	virtual void addParameter(ConfigurationParameter* param) = 0;

    virtual void removeParameter(ConfigurationParameter* param) = 0;

    virtual std::vector<ConfigurationParameter*> getParameters() = 0;

    virtual std::vector<ConfigurationParameter*> getParameters(std::string filter) = 0;

    virtual ~ConfigurationAPI() {}

};

#endif /*CONFIGURATION_API_H_*/
