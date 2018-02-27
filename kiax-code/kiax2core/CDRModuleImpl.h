#ifndef CDRMODULEIMPL_H_
#define CDRMODULEIMPL_H_

#include "CDRModule.h"
#include "CDR.h"

class CDRModuleImpl : public CDRModule {

 public:

	static AbstractModule* instance();
    
    virtual void logCDR(CDR* cdr);

    virtual std::vector<CDR*> getCDRs();

    virtual std::vector<CDR*> getCDRs(std::string filter);

    virtual void init();
    
    virtual void dispose();

 private:
	std::vector<Serializable*> cdrs;
	CDRModuleImpl();
};

#endif /*CDRMODULEIMPL_H_*/
