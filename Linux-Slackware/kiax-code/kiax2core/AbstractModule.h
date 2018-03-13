#ifndef AbstractModule_h
#define AbstractModule_h


#include "Configuration.h"

class AbstractModule {

 public:
 
    virtual void init()  = 0;

    virtual void dispose()  = 0;
    
	virtual ~AbstractModule();

 public:
	 
    std::string moduleName;
    std::string moduleType;
	
    Configuration* moduleConfiguration;

	
	
};
#endif
