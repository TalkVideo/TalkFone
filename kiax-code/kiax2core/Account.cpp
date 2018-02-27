#include "Account.h"

Account::Account() : Serializable(){
    objectType = "Account";
    columnsNumber = 6;
    columnsNames = new string[columnsNumber];
    columnsNames[0] = string("serviceName");
    columnsNames[1] = string("acc_username"); 
    columnsNames[2] = string("acc_password"); 
    columnsNames[3] = string("acc_host1"); 
    columnsNames[4] = string("acc_host2"); 
    columnsNames[5] = string("acc_host3"); 
}

Account::~Account() {
	columnsNames = 0;
}

std::string Account::getCreateTypeQuery()
{
	char* statement = "create table if not exists %s (serviceName varchar, acc_username varchar, acc_password varchar, acc_host1 varchar, acc_host2 varchar, acc_host3 varchar)";
	char query[1024];
	sprintf(query, statement, objectType.data());
	std::string result(query);
	return result;
}

std::string Account::getCreateQuery()
{
	char* statement = "insert into %s (serviceName, acc_username, acc_password, acc_host1, acc_host2, acc_host3) values ('%s', '%s', '%s', '%s', '%s', '%s')";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							serviceName.data(), 
							username.data(),
							password.data(),
							host1.data(),
							host2.data(),
							host3.data());
	std::string result(query);
	return result;
}

std::string Account::getStoreQuery()
{
	char* statement = "update %s set serviceName='%s', acc_username='%s', acc_password='%s', acc_host1='%s', acc_host2='%s', acc_host3='%s' where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							serviceName.data(),
							username.data(), 
							password.data(),
							host1.data(),
							host2.data(),
							host3.data(),
							objectId);
	std::string result(query);
	return result;
}

std::string Account::getDeleteQuery()
{
	char* statement = "delete from %s where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							objectId);
	std::string result(query);
	return result;
}

std::string Account::getLoadQuery(int objectId)
{
	char* statement = "select rowid, serviceName, acc_username, acc_password, acc_host1, acc_host2, acc_host3 from %s where rowid=%d";
	char query[1024];
	sprintf(query, statement, objectType.data(), 
							objectId);
	std::string result(query);
	return result;
}
