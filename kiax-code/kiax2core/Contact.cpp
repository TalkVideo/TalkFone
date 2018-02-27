#include "Contact.h"

Contact::Contact() : Serializable(){
    objectType = "Contact";
    columnsNumber = 2;
    columnsNames = new string[columnsNumber];
    columnsNames[0] = string("contactName");
    columnsNames[1] = string("extension"); 
    account = NULL;
}

Contact::~Contact() {
	columnsNames = 0;
}

std::string Contact::getCreateTypeQuery()
{
	char* statement = "create table if not exists %s (contactName varchar, extension varchar)";
	char query[1024];
	sprintf(query, statement, objectType.data());
	std::string result(query);
	return result;
}

std::string Contact::getCreateQuery()
{
	char* statement = "insert into %s (contactName, extension) values ('%s', '%s')";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							contactName.data(), 
							extension.data());
	std::string result(query);
	return result;
}

std::string Contact::getStoreQuery()
{
	char* statement = "update %s set contactName='%s', extension='%s' where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							contactName.data(),
							extension.data(), 
							objectId);
	std::string result(query);
	return result;
}

std::string Contact::getDeleteQuery()
{
	char* statement = "delete from %s where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							objectId);
	std::string result(query);
	return result;
}

std::string Contact::getLoadQuery(int objectId)
{
	char* statement = "select rowid, contactName, extension from %s where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							objectId);
	std::string result(query);
	return result;
}
