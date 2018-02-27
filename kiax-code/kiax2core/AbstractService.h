#ifndef AbstractService_h
#define AbstractService_h

#include <string>

class AbstractService {

 public:

	AbstractService(); 
	virtual ~AbstractService();
	 
    virtual bool start() = 0;
    virtual bool stop() = 0;
    int getErrorCode();
    bool isStarted();

 protected:
	int serviceErrCode;
	bool started;
    std::string serviceName;
    std::string serviceType;
};

#endif
