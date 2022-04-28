#ifndef TRAINTICKET_CPP
#define TRAINTICKET_CPP

#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<fstream>
#include "Exceptions.hpp"
#include "Trainticket.h"
#include<vector>//最后用手写的vector换掉
#include<algorithm>
using namespace std;

static int string_to_int(const string &str){
	int x=0;
	for(int i=0;i<(int)str.size();i++)x=x*10+str[i]-'0';
	return x;
}
static int time_to_minute(const string &str){
	return ((str[0]-'0')*10+str[1]-'0')*60+((str[3]-'0')*10+str[4]-'0');
}
static string minute_to_time(const int x){
	int Hour=x/60,Minute=x%60;
	string ans="";
	ans+=('0'+Hour/10),ans+=('0'+Hour%10);
	ans+=':';
	ans+=('0'+Minute/10),ans+=('0'+Minute%10);
	return ans;
}
static const int SumDay[13]={0,31,59,90,120,151,181,212,243,273,304,334,365};
static int date_to_day(const string &str){
	return SumDay[(str[0]-'0')*10+str[1]-'0' -1]+((str[3]-'0')*10+str[4]-'0');
}
static string day_to_date(const int x){
	string ans="";
	for(int i=11;i>=0;i--){
		if(x>SumDay[i]){
			int Month=i+1,Day=x-SumDay[i];
			ans+=('0'+Month/10),ans+=('0'+Month%10);
			ans+='-';
			ans+=('0'+Day/10),ans+=('0'+Day%10);
			break;
		}
	}
	return ans;
}
static void Output(Train_System::Train G){
	cout<<"~~~~ "<<G.trainID<<" ~~~~"<<endl;
	cout<<"stationNum="<<G.stationNum<<endl;
	for(int i=1;i<=G.stationNum;i++)cout<<G.stations[i]<<" ";
	cout<<endl;
	// cout<<"seatNum"<<endl;
	// for(int i=1;i<=G.saleDateR-G.saleDateL+1;i++){
	// 	for(int j=1;j<=G.stationNum;j++)cout<<G.seatNum[i][j]<<" ";
	// 	cout<<endl;
	// }
	cout<<"prices"<<endl;
	for(int i=1;i<=G.stationNum-1;i++)cout<<G.prices[i]<<" ";
	cout<<endl;
	cout<<"startTime="<<minute_to_time(G.startTime)<<endl;
	cout<<"travelTimes"<<endl;
	for(int i=1;i<=G.stationNum-1;i++)cout<<G.travelTimes[i]<<" ";
	cout<<endl;
	cout<<"stopoverTimes"<<endl;
	cout<<0<<" ";
	for(int i=2;i<=G.stationNum-1;i++)cout<<G.stopoverTimes[i]<<" ";
	cout<<0<<" ";
	cout<<endl;
	cout<<"saleDate= "<<day_to_date(G.saleDateL)<<","<<day_to_date(G.saleDateR)<<endl;
	cout<<"type="<<G.type<<endl;
}

extern string d_order[30];
extern int dcnt;

Train_System::Train Train_System::GetTrainFromData(const string &trainID){
	vector<int> G=TrainIndex.FindAll(trainID);
	Train tmp_train;
	TrainData.read(tmp_train,G[0]);
	return tmp_train;
}
void Train_System::queueUpdate(const string &trainID){

}

Train_System::Train_System():TrainData("TrainData"),OrderData("OrderData"),TrainIndex("TrainIndex"),StationIndex("StationIndex"),OrderIndex("OrderIndex"),QueueIndex("QueueIndex"){}
static void Analysis(string s,string *tmp,int &tmplen){
	tmp[tmplen=1]="";
	for(int i=0;i<(int)s.size();i++){
		if(s[i]=='|')tmp[++tmplen]="";
		else tmp[tmplen]+=s[i];
	}
	if(tmp[tmplen]=="")tmplen--;
	if(tmp[tmplen]=="_")tmplen--;
}
void Train_System::add_train(){
	string tmp[105];
	int tmplen;
	Train NewTrain;
	int seatNum;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-i")strcpy(NewTrain.trainID,d_order[i+1].c_str());
		if(d_order[i]=="-n")NewTrain.stationNum=string_to_int(d_order[i+1]);
		if(d_order[i]=="-m")seatNum=string_to_int(d_order[i+1]);
		if(d_order[i]=="-s"){
			Analysis(d_order[i+1],tmp,tmplen);
			for(int j=1;j<=tmplen;j++)strcpy(NewTrain.stations[j],tmp[j].c_str());
		}
		if(d_order[i]=="-p"){
			Analysis(d_order[i+1],tmp,tmplen);
			for(int j=1;j<=tmplen;j++)NewTrain.prices[j]=string_to_int(tmp[j]);
		}
		if(d_order[i]=="-x")NewTrain.startTime=time_to_minute(d_order[i+1]);
		if(d_order[i]=="-t"){
			Analysis(d_order[i+1],tmp,tmplen);
			for(int j=1;j<=tmplen;j++)NewTrain.travelTimes[j]=string_to_int(tmp[j]);
		}
		if(d_order[i]=="-o"){
			Analysis(d_order[i+1],tmp,tmplen);
			for(int j=1;j<=tmplen;j++)NewTrain.stopoverTimes[j+1]=string_to_int(tmp[j]);
		}
		if(d_order[i]=="-d"){
			Analysis(d_order[i+1],tmp,tmplen);
			NewTrain.saleDateL=date_to_day(tmp[1]),NewTrain.saleDateR=date_to_day(tmp[2]);
			cout<<NewTrain.saleDateL<<" "<<NewTrain.saleDateR<<endl;
		}
		if(d_order[i]=="-y")NewTrain.type=d_order[i+1][0];
	}
	NewTrain.isRelease=0;
	for(int i=1;i<=NewTrain.saleDateR-NewTrain.saleDateL+1;i++)for(int j=1;j<=NewTrain.stationNum;j++)NewTrain.seatNum[i][j]=seatNum;
	Output(NewTrain);
	if(TrainIndex.Find(NewTrain.trainID)){printf("-1\n");return;}
	int pos=TrainData.write(NewTrain);
	TrainIndex.insert(NewTrain.trainID,pos);
	for(int i=1;i<=NewTrain.stationNum;i++)StationIndex.insert(NewTrain.stations[i],pos);
	printf("0\n");
}
void Train_System::release_train(){
	string trainID;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-i")trainID=d_order[i+1];
	}
	if(!TrainIndex.Find(trainID)){printf("-1\n");return;}
	Train train=GetTrainFromData(trainID);
	if(train.isRelease){printf("-1\n");return;}
	train.isRelease=1;
	int pos=TrainIndex.FindAll(trainID)[0];
	TrainData.update(train,pos);
	queueUpdate(trainID);
	printf("0\n");
}
void Train_System::query_train(){
	string trainID;
	int day;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-i")trainID=d_order[i+1];
		if(d_order[i]=="-d")day=date_to_day(d_order[i+1]);
	}
	if(!TrainIndex.Find(trainID)){printf("-1\n");return;}
	Train train=GetTrainFromData(trainID);
	if(day<train.saleDateL||day>train.saleDateR){printf("-1\n");return;}
	cout<<train.trainID<<" "<<train.type<<endl;
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=day,LeavingMinute=train.startTime;
	int SumPrice=0;
	for(int i=1;i<=train.stationNum;i++){
		cout<<train.stations[i]<<" ";
		if(i==1)cout<<"xx-xx xx:xx";
		else cout<<day_to_date(ArrivingDay)<<" "<<minute_to_time(ArrivingMinute)<<" ";
		cout<<"-> ";
		if(i==train.stationNum)cout<<"xx-xx xx:xx";
		else cout<<day_to_date(LeavingDay)<<" "<<minute_to_time(LeavingMinute)<<" ";
		cout<<SumPrice<<" "<<train.seatNum[day-train.saleDateL+1][i]<<endl;
		ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i];
		if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
		if(i<=train.stationNum-2){
			LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i+1];
			if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
		}
		SumPrice+=train.prices[i];
	}
}
static bool Is_exist(const Train_System::Train &train,const string &startStation,const string &endStation){
	int sumTime=0,Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(train.stations[i]==startStation)Count=1;
		if(train.stations[i]==endStation)return Count;
	}
	return 0;
}
static int GetTime(const Train_System::Train &train,const string &startStation,const string &endStation){
	int sumTime=0,Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(train.stations[i]==startStation)Count=1;
		if(train.stations[i]==endStation)break;
		if(Count){
			sumTime+=train.travelTimes[i];
			if(train.stations[i]!=startStation)sumTime+=train.stopoverTimes[i];
		}
	}
	return sumTime;
}
static int GetCost(const Train_System::Train &train,const string &startStation,const string &endStation){
	int sumCost=0,Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(train.stations[i]==startStation)Count=1;
		if(train.stations[i]==endStation)break;
		if(Count)sumCost+=train.prices[i];
	}
	return sumCost;
}
static int GetMaxSeatNum(const Train_System::Train &train,const string &startStation,const string &endStation,const int &day){
	int MaxSeatNum=0,Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(train.stations[i]==startStation)Count=1;
		if(train.stations[i]==endStation)break;
		if(Count)MaxSeatNum=min(MaxSeatNum,train.seatNum[day-train.saleDateL+1][i]);
	}
	return MaxSeatNum;
}
static string GetLeavingTime(const Train_System::Train &train,const string &Station,const int &day){
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=day,LeavingMinute=train.startTime;
	for(int i=1;i<=train.stationNum;i++){
		ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i];
		if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
		if(i<=train.stationNum-2){
			LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i+1];
			if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
		}
		if(train.stations[i]==Station)break;
	}
	string ans=day_to_date(LeavingDay)+" "+minute_to_time(LeavingMinute);
	return ans;
}
static string GetArrivingTime(const Train_System::Train &train,const string &Station,const int &day){
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=day,LeavingMinute=train.startTime;
	for(int i=1;i<=train.stationNum;i++){
		ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i];
		if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
		if(i<=train.stationNum-2){
			LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i+1];
			if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
		}
		if(train.stations[i]==Station)break;
	}
	string ans=day_to_date(ArrivingDay)+" "+minute_to_time(ArrivingMinute);
	return ans;
}
static vector<Train_System::Train> tmp1;
static vector<int> tmp2;
static void SortTime(vector<Train_System::Train> &Alltrain,vector<int> &Time,int l,int r){
	if(l>=r)return;
	int mid=(l+r)>>1;
	SortTime(Alltrain,Time,l,mid),SortTime(Alltrain,Time,mid+1,r);
	int g1=l,g2=mid+1,g=l;
	while(g1<=mid&&g2<=r){
		if(Time[g1]<Time[g2]||(Time[g1]==Time[g2]&&Alltrain[g1].trainID<Alltrain[g2].trainID))tmp1[g]=Alltrain[g1],tmp2[g]=Time[g1],g++,g1++;
		else tmp1[g]=Alltrain[g2],tmp2[g]=Time[g2],g++,g2++;
	}
	while(g1<=mid)tmp1[g]=Alltrain[g1],tmp2[g]=Time[g1],g++,g1++;
	while(g2<=r)tmp1[g]=Alltrain[g2],tmp2[g]=Time[g2],g++,g2++;
	for(int i=l;i<=r;i++)Alltrain[i]=tmp1[i],Time[i]=tmp2[i];
	return;
}
static void SortCost(vector<Train_System::Train> &Alltrain,vector<int> &Cost,int l,int r){
	if(l>=r)return;
	int mid=(l+r)>>1;
	SortTime(Alltrain,Cost,l,mid),SortTime(Alltrain,Cost,mid+1,r);
	int g1=l,g2=mid+1,g=l;
	while(g1<=mid&&g2<=r){
		if(Cost[g1]<Cost[g2]||(Cost[g1]==Cost[g2]&&Alltrain[g1].trainID<Alltrain[g2].trainID))tmp1[g]=Alltrain[g1],tmp2[g]=Cost[g1],g++,g1++;
		else tmp1[g]=Alltrain[g2],tmp2[g]=Cost[g2],g++,g2++;
	}
	while(g1<=mid)tmp1[g]=Alltrain[g1],tmp2[g]=Cost[g1],g++,g1++;
	while(g2<=r)tmp1[g]=Alltrain[g2],tmp2[g]=Cost[g2],g++,g2++;
	for(int i=l;i<=r;i++)Alltrain[i]=tmp1[i],Cost[i]=tmp2[i];
	return;
}

void Train_System::query_ticket(){
	string startStation,endStation;
	int day;
	string sortType="time";
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-s")startStation=d_order[i+1];
		if(d_order[i]=="-t")endStation=d_order[i+1];
		if(d_order[i]=="-d")day=date_to_day(d_order[i+1]);
		if(d_order[i]=="-p")sortType=d_order[i+1];
	}
	vector<int> G=StationIndex.FindAll(startStation);
	Train tmp_train;
	vector<Train> Alltrain;
	for(int i=0;i<(int)G.size();i++){
		TrainData.read(tmp_train,G[i]);
		Alltrain.push_back(tmp_train);
	}
	vector<Train> OKtrain;
	vector<int> Time,Cost;
	for(int i=0;i<(int)Alltrain.size();i++){
		if(day<Alltrain[i].saleDateL||day>Alltrain[i].saleDateR)continue;
		if(!Is_exist(Alltrain[i],startStation,endStation))continue;
		OKtrain.push_back(Alltrain[i]);
		Time.push_back(GetTime(Alltrain[i],startStation,endStation));
		Cost.push_back(GetCost(Alltrain[i],startStation,endStation));
	}
	int Num=(int)OKtrain.size();
	if(sortType=="time")SortTime(OKtrain,Time,0,Num-1);
	if(sortType=="cost")SortCost(OKtrain,Cost,0,Num-1);
	cout<<Num<<endl;
	for(int i=0;i<Num;i++){
		cout<<OKtrain[i].trainID<<" ";
		cout<<startStation<<" ";
		cout<<GetLeavingTime(OKtrain[i],startStation,day)<<" ";
		cout<<"-> ";
		cout<<endStation<<" ";
		cout<<GetArrivingTime(OKtrain[i],endStation,day)<<" ";
		cout<<Time[i]<<" ";
		cout<<GetMaxSeatNum(OKtrain[i],startStation,endStation,day)<<endl;
	}
}
void Train_System::query_transfer(){
}
void Train_System::buy_ticket(){
}
void Train_System::query_order(){
}
void Train_System::refund_ticket(){
}
void Train_System::clean(){
	TrainData.clean();
	OrderData.clean();
	TrainIndex.clean();
	StationIndex.clean();
	OrderIndex.clean();
	QueueIndex.clean();
}
void Train_System::rollback(){
}

#endif