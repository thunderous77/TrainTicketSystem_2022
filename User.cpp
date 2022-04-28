#ifndef USER_CPP
#define USER_CPP

#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<fstream>
#include "Exceptions.hpp"
#include "User.h"
#include<map>//最后用手写的map换掉
using namespace std;

static int string_to_int(const string &str){
	int x=0;
	for(int i=0;i<(int)str.size();i++)x=x*10+str[i]-'0';
	return x;
}
static void Output(User_System::User G){
	cout<<"!!! "<<G.username<<" "<<G.password<<" "<<G.name<<" "<<G.mailAddr<<" "<<G.privilege<<endl;
}

extern string d_order[30];
extern int dcnt;
map<string,int>Is_login;

User_System::User User_System::GetUserFromData(const string &username){
	vector<int> G=UserIndex.FindAll(username);
	User tmp_user;
	UserData.read(tmp_user,G[0]);
	return tmp_user;
}
User_System::User_System():UserData("UserData"),UserIndex("UserIndex"){}
void User_System::add_user(){
	//将输入转成User类型
	User NewUser;
	string cur_username="";
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-c")cur_username=d_order[i+1];
		if(d_order[i]=="-u")strcpy(NewUser.username,d_order[i+1].c_str());
		if(d_order[i]=="-p")strcpy(NewUser.password,d_order[i+1].c_str());
		if(d_order[i]=="-n")strcpy(NewUser.name,d_order[i+1].c_str());
		if(d_order[i]=="-m")strcpy(NewUser.mailAddr,d_order[i+1].c_str());
		if(d_order[i]=="-g")NewUser.privilege=string_to_int(d_order[i+1]);
	}
	if(UserIndex.Find(NewUser.username)){printf("-1\n");return;}
	if(cur_username==""|| (Is_login[cur_username]&&UserIndex.Find(cur_username)&&GetUserFromData(cur_username).privilege>NewUser.privilege) ){
		if(cur_username=="")NewUser.privilege=10;
		int pos=UserData.write(NewUser);
		// Output(NewUser);
		// cout<<"!!!"<<NewUser.username<<" "<<pos<<endl;
		User cur_user;
		UserData.read(cur_user,pos);
		UserIndex.insert(NewUser.username,pos);
		printf("0\n");return;
	}
	printf("-1\n");
}
void User_System::login(){
	string username,password;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
		if(d_order[i]=="-p")password=d_order[i+1];
	}
	if(Is_login[username]){printf("-1\n");return;}
	// cout<<"Is_Find: "<<username<<" "<<UserIndex.Find(username)<<endl;
	if(!UserIndex.Find(username)){printf("-1\n");return;}
	// cout<<GetUserFromData(username).password<<" "<<password<<endl;
	if(GetUserFromData(username).password==password){
		Is_login[username]=1;
		printf("0\n");return;
	}
	else printf("-1\n");
}
void User_System::logout(){
	string username;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
	}
	if(!Is_login[username]){printf("-1\n");return;}
	Is_login[username]=0;
	printf("0\n");
}
void User_System::query_profile(){
	string cur_username,username;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-c")cur_username=d_order[i+1];
		if(d_order[i]=="-u")username=d_order[i+1];
	}
	if(!Is_login[cur_username]){printf("-1\n");return;}
	if(!UserIndex.Find(username)){printf("-1\n");return;}
	User cur_user=GetUserFromData(cur_username),user=GetUserFromData(username);
	if(cur_username!=username&&cur_user.privilege<=user.privilege){printf("-1\n");return;}
	cout<<user.username<<" "<<user.name<<" "<<user.mailAddr<<" "<<user.privilege<<endl;
}
void User_System::modify_profile(){
	string cur_username,username;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-c")cur_username=d_order[i+1];
		if(d_order[i]=="-u")username=d_order[i+1];
	}
	if(!Is_login[cur_username]){printf("-1\n");return;}
	if(!UserIndex.Find(username)){printf("-1\n");return;}
	User cur_user=GetUserFromData(cur_username),user=GetUserFromData(username);
	if(cur_username!=username&&cur_user.privilege<=user.privilege){printf("-1\n");return;}
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-p")strcpy(user.password,d_order[i+1].c_str());
		if(d_order[i]=="-n")strcpy(user.name,d_order[i+1].c_str());
		if(d_order[i]=="-m")strcpy(user.mailAddr,d_order[i+1].c_str());
		if(d_order[i]=="-g")user.privilege=string_to_int(d_order[i+1]);
	}
	if(cur_username!=username&&user.privilege>=cur_user.privilege){printf("-1\n");return;}
	if(cur_username==username&&user.privilege>cur_user.privilege){printf("-1\n");return;}
	int pos=UserIndex.FindAll(username)[0];
	UserData.update(user,pos);
	cout<<user.username<<" "<<user.name<<" "<<user.mailAddr<<" "<<user.privilege<<endl;
}
void User_System::clean(){
	UserData.clean();
	UserIndex.clean();
}
void User_System::rollback(){
}
#endif