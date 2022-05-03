#ifndef USER_H
#define USER_H

#include "MemoryRiver.hpp"
#include "BlockList.hpp"
#include<map>//最后用手写的map换掉
using namespace std;
class User_System{
public:
	class User{
	public:
		char username[22];
		char password[32];
		char name[12];
		char mailAddr[32];
		int privilege;
	};
private:
	User GetUserFromData(const string &username);
public:
	MemoryRiver<User> UserData;
	Key_value_database<int> UserIndex;
	User_System();
	void add_user();
	void login();
	void logout();
	void query_profile();
	void modify_profile();
	void clean();
	void rollback();
};

#endif