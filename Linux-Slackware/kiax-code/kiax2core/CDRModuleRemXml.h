#ifndef CDRMODULEREMXML_H_
#define CDRMODULEREMXML_H_
#include "CDRModule.h"
#include "CDR.h"

class CDRModuleRemXml : public CDRModule {
 public:
	static AbstractModule* instance();
    virtual void logCDR(CDR* cdr);
    virtual std::vector<CDR*> getCDRs();
    virtual std::vector<CDR*> getCDRs(std::string filter);
    virtual void init();
    virtual void dispose();
 private:
	std::vector<Serializable*> cdrs;
	CDRModuleRemXml();
};
#endif /*CDRMODULEREMXML_H_*/

