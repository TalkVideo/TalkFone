#ifndef APIBINDERDEFAULT_H_
#define APIBINDERDEFAULT_H_

#include "APIBinder.h"

class APIBinderDefault : public APIBinder {

 public:

	static APIBinder* instance();

 private:
	 
	APIBinderDefault();

};

#endif /*APIBINDERDEFAULT_H_*/
