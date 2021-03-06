#ifndef ORDER_CPP
#define ORDER_CPP

#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<fstream>
#include "Exceptions.hpp"
#include "Order.h"
#include "User.h"
#include "Trainticket.h"
using namespace std;
long long Clock1=0,Clock2=0,Clock3=0,Clock4=0,Clock5=0,Clock6=0,Clock7=0;

string d_order[30];
int dcnt;
void Order_System::DivideOrder(string order){
	d_order[dcnt=1]="";
	int len=order.size();
	for(int i=0;i<len;i++){
		if(order[i]==' '){
			d_order[++dcnt]="";
			while(i+1<len&&order[i+1]==' ')i++;
		}
		else d_order[dcnt]+=order[i];
	}
	if(d_order[dcnt]=="")dcnt--;
}
void Order_System::AnalysisOrder(string order){
	/*按空格划分order*/
	DivideOrder(order);
	// cout<<"order"<<endl;
	// for(int i=1;i<=dcnt;i++)cout<<d_order[i]<<" ";
	// cout<<endl;
	/*得到时间轴*/
	int Time=0;
	for(int i=1;i<=(int)d_order[1].size()-2;i++)Time=Time*10+d_order[1][i]-'0';
	// cout<<"Time="<<Time<<" ";
	/*判断为哪种指令*/
	if(d_order[2]=="add_user")UserSystem.add_user();
	if(d_order[2]=="login")UserSystem.login();
	if(d_order[2]=="logout")UserSystem.logout();
	if(d_order[2]=="query_profile")UserSystem.query_profile();
	if(d_order[2]=="modify_profile")UserSystem.modify_profile();
	if(d_order[2]=="add_train")TrainSystem.add_train();
	if(d_order[2]=="release_train")TrainSystem.release_train();
	if(d_order[2]=="query_train")TrainSystem.query_train();
	if(d_order[2]=="delete_train")TrainSystem.delete_train();
	if(d_order[2]=="query_ticket")TrainSystem.query_ticket();
	if(d_order[2]=="query_transfer")TrainSystem.query_transfer();
	if(d_order[2]=="buy_ticket")TrainSystem.buy_ticket();
	if(d_order[2]=="query_order")TrainSystem.query_order();
	if(d_order[2]=="refund_ticket")TrainSystem.refund_ticket();
	if(d_order[2]=="rollback"){UserSystem.rollback(),TrainSystem.rollback();cout<<d_order[1]<<" ";cout<<0<<endl;}
	if(d_order[2]=="clean"){UserSystem.clean(),TrainSystem.clean();cout<<d_order[1]<<" ";cout<<0<<endl;}
	if(d_order[2]=="exit"){
		cout<<d_order[1]<<" ";cout<<"bye"<<endl;
		// cerr<<"query_profile time:"<<Clock7/(double)(1e6)<<endl;
		// cerr<<"query_ticket time:"<<Clock1/(double)(1e6)<<endl;
		// cerr<<"query_transfer time:"<<Clock2/(double)(1e6)<<endl;
		// cerr<<"query_order time:"<<Clock3/(double)(1e6)<<endl;
		// cerr<<"refund_ticket time:"<<Clock4/(double)(1e6)<<endl;
		// cerr<<"buy_ticket time:"<<Clock6/(double)(1e6)<<endl;
		// cerr<<"read_write time:"<<Clock5/(double)(1e6)<<endl;
		exit(0);
	}
}
#endif