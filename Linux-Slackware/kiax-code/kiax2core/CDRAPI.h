#ifndef CDRAPI_H_
#define CDRAPI_H_

#include <vector>
#include "CDR.h"

class CDRAPI {

 public:

    virtual void logCDR(CDR* cdr) = 0;
    virtual std::vector<CDR*> getCDRs() = 0;

    virtual std::vector<CDR*> getCDRs(std::string filter) = 0;

    virtual ~CDRAPI() {}

};

#endif /*CDRAPI_H_*/
