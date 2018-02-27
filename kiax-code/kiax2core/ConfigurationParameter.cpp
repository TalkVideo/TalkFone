#include "ConfigurationParameter.h"
#include "Logger.h"

ConfigurationParameter::ConfigurationParameter() : Serializable(){
    objectType = "ConfigurationParameter";
    columnsNumber = 3;
    columnsNames = new string[columnsNumber];
    columnsNames[0] = string("paramName");
    columnsNames[1] = string("paramType"); 
    columnsNames[2] = string("paramValue"); 
}

ConfigurationParameter::~ConfigurationParameter() {
	Logger::log(Logger::DEBUG, "ConfigurationParameter DESTRUCT IN\n");
	//delete columnsNames;
	columnsNames = 0;
	Logger::log(Logger::DEBUG, "ConfigurationParameter DESTRUCT OUT\n");
}

std::string ConfigurationParameter::getCreateTypeQuery()
{
	char* statement = "create table if not exists %s (paramName varchar, paramType varchar, paramValue varchar)";
	char query[1024];
	sprintf(query, statement, objectType.data());
	std::string result(query);
	return result;
}

std::string ConfigurationParameter::getCreateQuery()
{
	char* statement = "insert into %s (paramName, paramType, paramValue) values ('%s', '%s', '%s')";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							name.data(), 
							paramType.data(),
							value.data());
	std::string result(query);
	return result;
}

std::string ConfigurationParameter::getStoreQuery()
{
	char* statement = "update %s set paramName='%s', paramType='%s', paramValue='%s' where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							name.data(),
							paramType.data(), 
							value.data(), 
							objectId);
	std::string result(query);
	return result;
}

std::string ConfigurationParameter::getDeleteQuery()
{
	char* statement = "delete from %s where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							objectId);
	std::string result(query); 
	return result;
}

std::string ConfigurationParameter::getLoadQuery(int objectId)
{
	char* statement = "select rowid, paramName, paramType, paramValue from %s where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							objectId);
	std::string result(query);
	return result;
}

ConfigurationParameter* ConfigurationParameter::clone()
{
	ConfigurationParameter* cloning = new ConfigurationParameter();
	cloning->setObjectId(objectId);
	cloning->paramType = paramType;
	cloning->name = name;
	cloning->value = value;
	return cloning;
}
