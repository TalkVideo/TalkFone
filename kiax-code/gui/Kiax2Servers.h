#ifndef KIAX2_SERVERS
#define KIAX2_SERVERS

#include "Configuration.h"
#include <string>
#include <vector>

class Kiax2Servers
{

public:
	Kiax2Servers();
	~Kiax2Servers() {}
	void addServer(std::string server);
	std::vector<std::string> getServers();
	bool isSupernodeUser();
	bool isValidUser();
	void setRealName(std::string realname);
	void setUsername(std::string uname);
	void setSupernodeUser(bool flag);
	void setValidUser(bool flag);
	std::string getRealName();
	std::string getUsername();
private:
	std::vector<std::string> servers;
	std::string realName;
	std::string username;
	bool validUser;
	bool supernodeUser;
};
#endif

