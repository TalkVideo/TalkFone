#include "Logger.h"

int Logger::debugLevel;

void Logger::init(int dlevel)
{
	debugLevel = dlevel;
}

void Logger::log(int level, const char* message, ...) {
	va_list ap;
	if ((level <= debugLevel)||(level == LOG_LEVEL_INFO)) 
	{
		char* finalLog = new char[2048]; // max 2K bytes per log entry
		finalLog = strcpy(finalLog, DEFAULT_SERVICE_NAME);
		switch (level) {
			case LOG_LEVEL_INFO : {
				finalLog = strcat(finalLog, " -INFO- ");
				break;
			}
			case LOG_LEVEL_DEBUG : {
				finalLog = strcat(finalLog, " -DEBUG- ");
				break;
			}
			case LOG_LEVEL_WARNING : {
				finalLog = strcat(finalLog, " -WARNING- ");
				break;
			}
			case LOG_LEVEL_ERROR : {
				finalLog = strcat(finalLog, " -ERROR- ");
				break;
			}

		}
		finalLog = strncat(finalLog, message, 2046);
		va_start(ap, message);
			vfprintf(stderr, finalLog, ap);
			fflush(stderr);
		va_end(ap);
		delete finalLog;
	}
}

void Logger::setDebugLevel(int dlevel)
{
	debugLevel = dlevel;
}

