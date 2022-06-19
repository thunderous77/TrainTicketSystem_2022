#ifndef USER_H
#define USER_H

#include "MemoryRiver.hpp"
// #include "BlockList.hpp"
#include "CBPlusTree.hpp"
#include "../lib/linked_hashmap.hpp"
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
template<int Len>
class MyStr{
public:
	char s[Len];
	MyStr(){for(int i=0;i<Len;i++)s[i]='\0';}
	MyStr(const string &G){strcpy(s,G.c_str());}
	MyStr(char *G){strcpy(s,G);}
	friend bool operator <(const MyStr &A,const MyStr &B){
		return strcmp(A.s,B.s)<0;
	}
	friend bool operator >(const MyStr &A,const MyStr &B){
		return strcmp(A.s,B.s)>0;
	}
	friend bool operator ==(const MyStr &A,const MyStr &B){
		return strcmp(A.s,B.s)==0;
	}
	friend bool operator !=(const MyStr &A,const MyStr &B){return !(A==B);}
};

class User_System{
public:
	class User{
	public:
		char username[21];
		char password[31];
		char name[16];
		char mailAddr[31];
		short privilege;
	};
private:
	User GetUserFromData(const string &username);
public:
	MemoryRiver<User> UserData;
	// CBPlusTree<int,21,20> UserIndex;
	CBPlusTree<int,int,21,75,75> UserIndex;
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