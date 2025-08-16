#include "CDR.h"

CDR::CDR() : Serializable(){
	objectType = "CDR";
	columnsNumber = 8;
	columnsNames = new string[columnsNumber];
	columnsNames[0] = "cdrTime";
	columnsNames[1] = "cdrInfo";
	columnsNames[2] = "srcExt";
	columnsNames[3] = "dstExt";
	columnsNames[4] = "callState";
	columnsNames[5] = "direction";
	columnsNames[6] = "duration";
	columnsNames[7] = "dstName";
}

CDR::~CDR() {
	columnsNames = 0;
}

std::string CDR::getCreateTypeQuery()
{
	char* statement = "create table if not exists %s (cdrTime varchar, cdrInfo varchar, srcExt varchar, dstExt varchar, callState varchar, direction varchar, duration varchar, dstName varchar)";
	char query[1024];
	sprintf(query, statement, objectType.data());
	std::string result(query);
	return result;
}

std::string CDR::getCreateQuery()
{
	char* statement = "insert into %s (cdrTime, cdrInfo, srcExt, dstExt, callState, direction, duration, dstName) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
								cdrTime.data(), 
								cdrInfo.data(),
								srcExt.data(),
								dstExt.data(),
								callState.data(),
								direction.data(),
								duration.data(),
								dstName.data());
	std::string result(query);
	return result;
}

std::string CDR::getStoreQuery()
{
	char* statement = "update %s set cdrTime='%s', cdrInfo='%s', srcExt='%s', dstExt='%s', callState='%s', direction='%s', duration='%s', dstName='%s' where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							cdrTime.data(),
							cdrInfo.data(), 
							srcExt.data(),
							dstExt.data(),
							callState.data(),
							direction.data(),
							duration.data(),
							dstName.data(),
							objectId);
	std::string result(query);
	return result;
}

std::string CDR::getDeleteQuery()
{
	char* statement = "delete from %s where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							objectId);
	std::string result(query);
	return result;
}

std::string CDR::getLoadQuery(int objectId)
{
	char* statement = "select rowid, cdrTime, cdrInfo, srcExt, dstExt, callState, direction, duration, dstName from %s where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							objectId);
	std::string result(query);
	return result;
}

