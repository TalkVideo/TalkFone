#include "AbstractService.h"

AbstractService::AbstractService()
{
	started = false;
}

AbstractService::~AbstractService()
{
	started = false;
}

int AbstractService::getErrorCode()
{
	return serviceErrCode;
}

bool AbstractService::isStarted()
{
	return started;
}
