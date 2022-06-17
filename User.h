#ifndef USER_H
#define USER_H

#include "MemoryRiver.hpp"
// #include "BlockList.hpp"
#include "CBPlusTree.hpp"
#include "linked_hashmap.hpp"
using namespace std;
template<class T>class for_rollback{
public:
	int timestamp,type;// type=1,insert(key,data);type=-1,Delete(key,data)
	char key[42];
	T val;
	for_rollback(){}
	for_rollback(int _timestamp,int _type,string _key,T _val){
		timestamp=_timestamp;
		type=_type;
		strcpy(key,_key.c_str());
		val=_val;
	}
};

class User_System{
public:
	class User{
	public:
		char username[21];
		char password[31];
		char name[17];
		char mailAddr[31];
		int privilege;
	};
private:
	User GetUserFromData(const string &username);
public:
	MemoryRiver<User> UserData;
	CBPlusTree<int,21,20> UserIndex;
	// Key_value_database<int> UserIndex;
	MemoryRiver< for_rollback<int> > UserIndex_rollback;
	MemoryRiver<int> UserData_rollback;
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