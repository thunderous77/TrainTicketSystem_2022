#ifndef USER_H
#define USER_H

using namespace std;
class User_System{
	// Data_
public:
	void add_user();
	void login();
	void logout();
	void query_profile();
	void modify_profile();
	void clean();
	void rollback();
};

#endif