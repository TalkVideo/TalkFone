#include "Kiax2Servers.h"

Kiax2Servers::Kiax2Servers()
{
	validUser = false;
	supernodeUser = false;
}

void Kiax2Servers::addServer(std::string server)
{
	servers.push_back(server);
}

std::vector<std::string> Kiax2Servers::getServers()
{
	return servers;
}

bool Kiax2Servers::isValidUser()
{
	return validUser;
}

bool Kiax2Servers::isSupernodeUser()
{
	return supernodeUser;
}

std::string Kiax2Servers::getRealName()
{
	return realName;
}

std::string Kiax2Servers::getUsername()
{
	return username;
}

void Kiax2Servers::setValidUser(bool flag)
{
	validUser = flag;
}

void Kiax2Servers::setSupernodeUser(bool flag)
{
	supernodeUser = flag;
}

void Kiax2Servers::setUsername(std::string uname)
{
	username = uname;
}

void Kiax2Servers::setRealName(std::string realname)
{
	realName = realname;
}
