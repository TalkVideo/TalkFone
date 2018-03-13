#ifndef CDR_h
#define CDR_h

#include <stdio.h>
#include "Serializable.h"


class CDR : public Serializable {

 public:
	CDR();
	virtual ~CDR();
	std::string getCreateTypeQuery();
	std::string getCreateQuery();
	std::string getStoreQuery();
	std::string getDeleteQuery();
	std::string getLoadQuery(int objectId);

 public:
    std::string cdrTime;
    std::string cdrInfo;
    std::string srcExt;
    std::string dstExt;
    std::string callState;
    std::string direction;
    std::string duration;
    std::string dstName;
};

#endif
