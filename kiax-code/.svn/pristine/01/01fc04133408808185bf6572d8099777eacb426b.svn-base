#ifndef VOIPMIXLOGGER
#define VOIPMIXLOGGER

#include <stdarg.h>
#include <string.h>
#include <iostream>

#include "PConstants.h"
#include "PConfiguration.h"

using namespace std;

class Logger {

public:

	static const int INFO 	= 	LOG_LEVEL_INFO;
	static const int DEBUG 	= 	LOG_LEVEL_DEBUG;
	static const int WARNING= 	LOG_LEVEL_WARNING;
	static const int SEVERE	= 	LOG_LEVEL_ERROR;

	static int debugLevel;

	static void log(int level, const char* message, ...);
	static void setDebugLevel(int dlevel);
	static void init(int debugLevel);
	
}; // end class Logger


#endif
