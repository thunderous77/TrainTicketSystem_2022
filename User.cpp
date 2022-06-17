#ifndef USER_CPP
#define USER_CPP

#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<fstream>
#include "Exceptions.hpp"
#include "User.h"
#include "vector.hpp"
using namespace std;
using namespace sjtu;

extern long long Clock7;

extern string OutputData;
extern string d_order[30];
extern int dcnt;
linked_hashmap<string,int>Is_login;
linked_hashmap<string,User_System::User>User_cache;

void Usercache_Get(User_System::User &user){
	if(User_cache.find(user.username)!=User_cache.end()){
		if(User_cache.size()==10000)User_cache.erase(User_cache.begin());
		User_cache[user.username]=user;
	}
}

static int string_to_int(const string &str){
	int x=0;
	for(int i=0;i<(int)str.size();i++)x=x*10+str[i]-'0';
	return x;
}
static int string_to_int2(const string &str){
	int x=0;
	for(int i=0;i<(int)str.size();i++)if('0'<=str[i]&&str[i]<='9')x=x*10+str[i]-'0';
	return x;
}
static string int_to_string(int x){
	int s[12],cnt=0;
	string str="";
	while(x)s[++cnt]=x%10,x/=10;
	while(cnt)str+=char(s[cnt]+'0'),cnt--;
	return str;
}
static void Output(User_System::User G){
	cout<<"!!! "<<G.username<<" "<<G.password<<" "<<G.name<<" "<<G.mailAddr<<" "<<G.privilege<<endl;
}


User_System::User User_System::GetUserFromData(const string &username){
	Vector<int> G=UserIndex.FindAll(username);
	User tmp_user;
	UserData.read(tmp_user,G[0]);
	return tmp_user;
}
User_System::User_System():UserData("UserData",true,true),UserIndex("UserIndex")
							,UserIndex_rollback("UserIndex_rollback",false,false),UserData_rollback("UserData_rollback",false,false)
							{}
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
	if(UserIndex.Find(NewUser.username))throw Username_Is_Exist();
	int Usernum;
	UserData.read_info(Usernum,1);
	// cout<<Usernum<<endl;
	if(Usernum&&!Is_login[cur_username])throw User_Not_Login();
	if(Usernum&&GetUserFromData(cur_username).privilege<=NewUser.privilege)throw Invalid_Privilege();
	if(!Usernum)NewUser.privilege=10;
	int pos=UserData.write(NewUser);
	// Output(NewUser);
	// cout<<"!!!"<<NewUser.username<<" "<<pos<<endl;

	Usercache_Get(NewUser);

	int tmp_rollback2=string_to_int2(d_order[1]);
	UserData_rollback.write(tmp_rollback2);

	
	UserIndex.insert(NewUser.username,pos);
	
	for_rollback<int> tmp_rollback(string_to_int2(d_order[1]),1,NewUser.username,pos);
	UserIndex_rollback.write(tmp_rollback);
	
	cout<<d_order[1]<<" ";
	printf("0\n");OutputData+="添加成功<br>";return;
}
void User_System::login(){
	string username,password;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
		if(d_order[i]=="-p")password=d_order[i+1];
	}
	if(Is_login[username])throw Already_Login();
	// cout<<"Is_Find: "<<username<<" "<<UserIndex.Find(username)<<endl;
	if(!UserIndex.Find(username))throw Wrong_User_Or_Password();
	// cout<<GetUserFromData(username).password<<" "<<password<<endl;
	User user=GetUserFromData(username);
	if(user.password==password){
		Is_login[username]=1;
		cout<<d_order[1]<<" ";
		printf("0\n");OutputData+="登录成功<br>";return;

		Usercache_Get(user);
	}
	else throw Wrong_User_Or_Password();

	
}
void User_System::logout(){
	string username;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
	}
	if(!Is_login[username]){throw User_Not_Login();}
	Is_login[username]=0;
	cout<<d_order[1]<<" ";
	printf("0\n");OutputData+="登出成功<br>";
}
void User_System::query_profile(){
	Clock7-=clock();
	string cur_username,username;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-c")cur_username=d_order[i+1];
		if(d_order[i]=="-u")username=d_order[i+1];
	}
	if(!Is_login[cur_username]){Clock7+=clock();throw User_Not_Login();}

	User user;
	if(User_cache.find(username)!=User_cache.end())user=User_cache[username];
	else {
		Vector<int> G=UserIndex.FindAll(username);
		if(!G.size()){Clock7+=clock();throw Username_Not_Exist();}
		// if(!UserIndex.Find(username))throw Username_Not_Exist();
		UserData.read(user,G[0]);
	}
	User cur_user=GetUserFromData(cur_username);
	if(cur_username!=username&&cur_user.privilege<=user.privilege){Clock7+=clock();throw Invalid_Privilege();}
	cout<<d_order[1]<<" ";
	cout<<user.username<<" "<<user.name<<" "<<user.mailAddr<<" "<<user.privilege<<endl;
	OutputData+="查询成功<br>";
	OutputData+="用户名："+string(user.username)+"<br>";
	OutputData+="真实姓名："+string(user.name)+"<br>";
	OutputData+="邮箱："+string(user.mailAddr)+"<br>";
	OutputData+="权限："+int_to_string(user.privilege)+"<br>";

	Usercache_Get(user);
	Usercache_Get(cur_user);
	
	Clock7+=clock();
}
void User_System::modify_profile(){
	string cur_username,username;
	int privilege=-1;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-c")cur_username=d_order[i+1];
		if(d_order[i]=="-u")username=d_order[i+1];
	}
	if(!Is_login[cur_username])throw User_Not_Login();
	User user;
	Vector<int> G=UserIndex.FindAll(username);
	if(!G.size())throw Username_Not_Exist();
	// if(!UserIndex.Find(username))throw Username_Not_Exist();
	UserData.read(user,G[0]);
	User cur_user;
	if(User_cache.find(cur_username)!=User_cache.end())cur_user=User_cache[cur_username];
	else cur_user=GetUserFromData(cur_username);
	if(cur_username!=username&&cur_user.privilege<=user.privilege)throw Invalid_Privilege();
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-p")strcpy(user.password,d_order[i+1].c_str());
		if(d_order[i]=="-n")strcpy(user.name,d_order[i+1].c_str());
		if(d_order[i]=="-m")strcpy(user.mailAddr,d_order[i+1].c_str());
		if(d_order[i]=="-g")privilege=string_to_int(d_order[i+1]);
	}
	if(privilege!=-1&&privilege>=cur_user.privilege)throw Invalid_Privilege();
	if(privilege!=-1)user.privilege=privilege;
	int pos=G[0];
	UserData.update(user,pos);
	int tmp_rollback2=string_to_int2(d_order[1]);
	UserData_rollback.write(tmp_rollback2);

	cout<<d_order[1]<<" ";
	cout<<user.username<<" "<<user.name<<" "<<user.mailAddr<<" "<<user.privilege<<endl;
	OutputData+="更改成功<br>";
	OutputData+="用户名："+string(user.username)+"<br>";
	OutputData+="真实姓名："+string(user.name)+"<br>";
	OutputData+="邮箱："+string(user.mailAddr)+"<br>";
	OutputData+="权限："+int_to_string(user.privilege)+"<br>";

	Usercache_Get(user);
	Usercache_Get(cur_user);
}
void User_System::clean(){
	UserData.clean();
	UserIndex.clean();
	UserIndex_rollback.clean();
	UserData_rollback.clean();
	Is_login.clear();
	User_cache.clear();
	OutputData+="用户系统数据清空成功<br>";
}
void User_System::rollback(){
	User_cache.clear();

	int Backtimestmp=string_to_int(d_order[4]);
	int timestamp=string_to_int2(d_order[1]);
	if(Backtimestmp>timestamp)throw Rollback_Timestamp_Error();
	// cout<<"!!!!!"<<Backtimestmp<<endl;
	for_rollback<int> tmp;
	int tmp2;

	//回滚UserIndex
	int pos=UserIndex_rollback.Maxpos();
	while(pos!=-1){
		UserIndex_rollback.read(tmp,pos);
		if(tmp.timestamp<Backtimestmp)break;
		// cout<<(tmp.type==1?"insert":"Delete")<<" "<<tmp.key<<" "<<tmp.val<<endl;
		if(tmp.type==1)UserIndex.Delete(tmp.key,tmp.val);//在原来该时间戳insert(key,val)
		if(tmp.type==-1)UserIndex.insert(tmp.key,tmp.val);//在原来该时间戳Delete(key,val)
		UserIndex_rollback.Delete(pos,1);
		pos=UserIndex_rollback.Maxpos();
	}

	//回滚UserData
	pos=UserData_rollback.Maxpos();
	while(pos!=-1){
		UserData_rollback.read(tmp2,pos);
		if(tmp2<Backtimestmp)break;
		// cout<<"@@@ UserData "<<tmp2<<endl;
		UserData.rollback();
		UserData_rollback.Delete(pos,1);
		pos=UserData_rollback.Maxpos();
	}


	Is_login.clear();
}
#endif