#ifndef CDRModule_h
#define CDRModule_h

#include <string>
#include <vector>
#include "AbstractModule.h"
#include "AbstractService.h"
#include "CDR.h"
#include "CDRAPI.h"

class CDRModule : public AbstractModule, public CDRAPI {

 public:

	static CDRModule* instance();
		
 protected:
	
	AbstractService* storageService;

 protected:
	 
    static CDRModule* singleton;
};

#endif

