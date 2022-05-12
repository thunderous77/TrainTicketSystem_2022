#ifndef TRAINTICKET_CPP
#define TRAINTICKET_CPP

#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<fstream>
#include "Exceptions.hpp"
#include "User.h"
#include "Trainticket.h"
#include<vector>//最后用手写的vector换掉
#include<algorithm>
using namespace std;

extern string d_order[30];
extern int dcnt;
extern map<string,int>Is_login;
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
static int time_to_minute(const string &str){
	return ((str[0]-'0')*10+str[1]-'0')*60+((str[3]-'0')*10+str[4]-'0');
}
static string minute_to_time(const int &x){
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
static int DateTime_to_minute(const string &str){
	string str1="",str2="";
	str1+=str[0],str1+=str[1],str1+=str[2],str1+=str[3],str1+=str[4];
	str2+=str[6],str2+=str[7],str2+=str[8],str2+=str[9],str2+=str[10];
	return date_to_day(str1)*1440+time_to_minute(str2);
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
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
}
static void Analysis(string s,string *tmp,int &tmplen){
	tmp[tmplen=1]="";
	for(int i=0;i<(int)s.size();i++){
		if(s[i]=='|')tmp[++tmplen]="";
		else tmp[tmplen]+=s[i];
	}
	if(tmp[tmplen]=="")tmplen--;
	if(tmp[tmplen]=="_")tmplen--;
}

Train_System::Train Train_System::GetTrainFromData(const string &trainID){
	vector<int> G=TrainIndex.FindAll(trainID);
	Train tmptrain;
	TrainData.read(tmptrain,G[0]);
	return tmptrain;
}
static bool Is_exist(const Train_System::Train &train,const string &startStation,const string &endStation,const int &startday=-1){
	// cout<<"!!!"<<train.trainID<<" "<<startStation<<" "<<endStation<<" "<<startday<<endl;
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=0,LeavingMinute=train.startTime;
	int Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(i!=1){
			ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i-1];
			if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
			if(i!=train.stationNum){
				LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i];
				if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
			}
		}
		if(string(train.stations[i])==startStation){
			// cout<<"!!!"<<LeavingDay<<" "<<train.saleDateL<<" "<<train.saleDateR<<" "<<startday<<endl;
			if( startday!=-1 && !(train.saleDateL<=startday-LeavingDay&&startday-LeavingDay<=train.saleDateR) )return 0;
			Count=1;
			continue;
		}
		if(string(train.stations[i])==endStation)return Count;
	}
	// cout<<"@@"<<train.trainID<<" "<<startStation<<" "<<endStation<<endl;
	return 0;
}
static int GetTime(const Train_System::Train &train,const string &startStation,const string &endStation){
	int sumTime=0,Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(string(train.stations[i])==startStation)Count=1;
		if(string(train.stations[i])==endStation)break;
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
		if(string(train.stations[i])==startStation)Count=1;
		if(string(train.stations[i])==endStation)break;
		if(Count)sumCost+=train.prices[i];
	}
	return sumCost;
}
static int GetMaxSeatNum(const Train_System::Train &train,const string &startStation,const string &endStation,const int &firday){
	int MaxSeatNum=1e9,Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(string(train.stations[i])==startStation)Count=1;
		if(string(train.stations[i])==endStation)break;
		if(Count)MaxSeatNum=min(MaxSeatNum,train.seatNum[firday-train.saleDateL+1][i]);
	}
	return MaxSeatNum;
}
void updateSeatNum(Train_System::Train &train,const string &startStation,const string &endStation,const int &num,const int &firday){
	int Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(string(train.stations[i])==startStation)Count=1;
		if(string(train.stations[i])==endStation)break;
		if(Count)train.seatNum[firday-train.saleDateL+1][i]+=num;
	}
}
static string GetLeavingTime(const Train_System::Train &train,const string &Station,const int &firday){
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=firday,LeavingMinute=train.startTime;
	for(int i=1;i<=train.stationNum;i++){
		if(i!=1){
			ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i-1];
			if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
			if(i!=train.stationNum){
				LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i];
				if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
			}
		}
		if(string(train.stations[i])==Station)break;
	}
	string ans=day_to_date(LeavingDay)+" "+minute_to_time(LeavingMinute);
	return ans;
}
static string GetArrivingTime(const Train_System::Train &train,const string &Station,const int &firday){
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=firday,LeavingMinute=train.startTime;
	for(int i=1;i<=train.stationNum;i++){
		if(i!=1){
			ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i-1];
			if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
			if(i!=train.stationNum){
				LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i];
				if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
			}
		}
		if(string(train.stations[i])==Station)break;
	}
	string ans=day_to_date(ArrivingDay)+" "+minute_to_time(ArrivingMinute);
	return ans;
}
static int GetTrainStartDay(const Train_System::Train &train,const string &startStation,const int &startday){
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=0,LeavingMinute=train.startTime;
	for(int i=1;i<=train.stationNum;i++){
		if(i!=1){
			ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i-1];
			if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
			if(i!=train.stationNum){
				LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i];
				if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
			}
		}
		if(string(train.stations[i])==startStation)return startday-LeavingDay;
	}
	return -2333;
}
static int GetTransferStartDay(const Train_System::Train &train,const string &Station,const string &ArrivingDateTime){
	int totMinute=DateTime_to_minute(ArrivingDateTime);
	int L_ArrivingDay=totMinute/1440,L_ArrivingMinute=totMinute%1440;
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=0,LeavingMinute=train.startTime;
	for(int i=1;i<=train.stationNum;i++){
		if(i!=1){
			ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i-1];
			if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
			if(i!=train.stationNum){
				LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i];
				if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
			}
		}
		if(string(train.stations[i])==Station){
			int startday=L_ArrivingDay-LeavingDay;
			if(L_ArrivingMinute>=LeavingMinute)startday++;
			if(startday>train.saleDateR)return -1;
			if(startday<train.saleDateL)return train.saleDateL;
			return startday;
		}
	}
	return -1;
}
static vector<Train_System::Train> tmp1;
static vector<int> tmp2;
static void SortTrainTime(vector<Train_System::Train> &Alltrain,vector<int> &Time,int l,int r){
	if(l>=r)return;
	int mid=(l+r)>>1;
	SortTrainTime(Alltrain,Time,l,mid),SortTrainTime(Alltrain,Time,mid+1,r);
	int g1=l,g2=mid+1,g=l;
	while(g1<=mid&&g2<=r){
		if(Time[g1]<Time[g2]||(Time[g1]==Time[g2]&&string(Alltrain[g1].trainID)<string(Alltrain[g2].trainID)))tmp1[g]=Alltrain[g1],tmp2[g]=Time[g1],g++,g1++;
		else tmp1[g]=Alltrain[g2],tmp2[g]=Time[g2],g++,g2++;
	}
	while(g1<=mid)tmp1[g]=Alltrain[g1],tmp2[g]=Time[g1],g++,g1++;
	while(g2<=r)tmp1[g]=Alltrain[g2],tmp2[g]=Time[g2],g++,g2++;
	for(int i=l;i<=r;i++)Alltrain[i]=tmp1[i],Time[i]=tmp2[i];
	return;
}
static void SortTrainCost(vector<Train_System::Train> &Alltrain,vector<int> &Cost,int l,int r){
	if(l>=r)return;
	int mid=(l+r)>>1;
	SortTrainCost(Alltrain,Cost,l,mid),SortTrainCost(Alltrain,Cost,mid+1,r);
	int g1=l,g2=mid+1,g=l;
	while(g1<=mid&&g2<=r){
		if(Cost[g1]<Cost[g2]||(Cost[g1]==Cost[g2]&&string(Alltrain[g1].trainID)<string(Alltrain[g2].trainID)))tmp1[g]=Alltrain[g1],tmp2[g]=Cost[g1],g++,g1++;
		else tmp1[g]=Alltrain[g2],tmp2[g]=Cost[g2],g++,g2++;
	}
	while(g1<=mid)tmp1[g]=Alltrain[g1],tmp2[g]=Cost[g1],g++,g1++;
	while(g2<=r)tmp1[g]=Alltrain[g2],tmp2[g]=Cost[g2],g++,g2++;
	for(int i=l;i<=r;i++)Alltrain[i]=tmp1[i],Cost[i]=tmp2[i];
	return;
}
static vector<Train_System::Order> tmp3;
static vector<int> tmp4;
static void SortOrderTime(vector<Train_System::Order> &AllOrder,vector<int> &Pos,int l,int r,int tp){//tp=0 timestamp从小到大 tp=1 timestamp从大到小
	if(l>=r)return;
	int mid=(l+r)>>1;
	SortOrderTime(AllOrder,Pos,l,mid,tp),SortOrderTime(AllOrder,Pos,mid+1,r,tp);
	int g1=l,g2=mid+1,g=l;
	while(g1<=mid&&g2<=r){
		if((tp==0&&AllOrder[g1].timestamp<AllOrder[g2].timestamp)||(tp==1&&AllOrder[g1].timestamp>AllOrder[g2].timestamp))tmp3[g]=AllOrder[g1],tmp4[g]=Pos[g1],g++,g1++;
		else tmp3[g]=AllOrder[g2],tmp4[g]=Pos[g2],g++,g2++;
	}
	while(g1<=mid)tmp3[g]=AllOrder[g1],tmp4[g]=Pos[g1],g++,g1++;
	while(g2<=r)tmp3[g]=AllOrder[g2],tmp4[g]=Pos[g2],g++,g2++;
	for(int i=l;i<=r;i++)AllOrder[i]=tmp3[i],Pos[i]=tmp4[i];
	return;
}
void Train_System::queueUpdate(const string &trainID){
	//读取数据并排序
	vector<int> G=QueueIndex.FindAll(trainID);
	vector<Order> AllOrder;
	Order tmpOrder;
	int Num=(int)G.size();
	for(int i=0;i<Num;i++){
		OrderData.read(tmpOrder,G[i]);
		AllOrder.push_back(tmpOrder);
	}
	tmp3=AllOrder,tmp4=G,SortOrderTime(AllOrder,G,0,Num-1,0);
	for(int i=0;i<Num;i++){
		Train train=GetTrainFromData(AllOrder[i].trainID);
		// cout<<"!!!!!!!"<<AllOrder[i].timestamp<<" "<<AllOrder[i].seatNum<<" "<<GetMaxSeatNum(train,AllOrder[i].startStation,AllOrder[i].endStation,AllOrder[i].firday)<<endl;
		if(GetMaxSeatNum(train,AllOrder[i].startStation,AllOrder[i].endStation,AllOrder[i].firday)>=AllOrder[i].seatNum){
			AllOrder[i].status=1;
			//更新OrderData
			OrderData.update(AllOrder[i],G[i]);
			//更新TrainData
			updateSeatNum(train,AllOrder[i].startStation,AllOrder[i].endStation,-AllOrder[i].seatNum,AllOrder[i].firday);
			int pos=TrainIndex.FindAll(train.trainID)[0];
			TrainData.update(train,pos);
			//在QueueIndex中删除
			QueueIndex.Delete(AllOrder[i].trainID,G[i]);
		}
	}
}

Train_System::Train_System():TrainData("TrainData"),OrderData("OrderData"),TrainIndex("TrainIndex"),StationIndex("StationIndex"),OrderIndex("OrderIndex"),QueueIndex("QueueIndex"){}
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
		}
		if(d_order[i]=="-y")NewTrain.type=d_order[i+1][0];
	}
	NewTrain.isRelease=0;
	for(int i=1;i<=NewTrain.saleDateR-NewTrain.saleDateL+1;i++)for(int j=1;j<NewTrain.stationNum;j++)NewTrain.seatNum[i][j]=seatNum;
	// Output(NewTrain);
	//已经存在该动车 不合法
	if(TrainIndex.Find(NewTrain.trainID)){printf("-1\n");return;}
	//添加train
	int pos=TrainData.write(NewTrain);
	// cout<<NewTrain.trainID<<" pos="<<pos<<endl;
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
	//动车已经发布 不合法
	if(train.isRelease){printf("-1\n");return;}
	//更新TrainData
	train.isRelease=1;
	int pos=TrainIndex.FindAll(trainID)[0];
	TrainData.update(train,pos);
	//queueUpdate
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
	//未找到该动车 不合法
	if(!TrainIndex.Find(trainID)){printf("-1\n");return;}
	//读取数据
	Train train=GetTrainFromData(trainID);
	if(day<train.saleDateL||day>train.saleDateR){printf("-1\n");return;}
	//输出动车信息
	cout<<train.trainID<<" "<<train.type<<endl;
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=day,LeavingMinute=train.startTime;
	int SumPrice=0;
	for(int i=1;i<=train.stationNum;i++){
		cout<<train.stations[i]<<" ";
		if(i==1)cout<<"xx-xx xx:xx ";
		else cout<<day_to_date(ArrivingDay)<<" "<<minute_to_time(ArrivingMinute)<<" ";
		cout<<"-> ";
		if(i==train.stationNum)cout<<"xx-xx xx:xx ";
		else cout<<day_to_date(LeavingDay)<<" "<<minute_to_time(LeavingMinute)<<" ";
		cout<<SumPrice<<" ";
		if(i==train.stationNum)cout<<"x"<<endl;
		else cout<<train.seatNum[day-train.saleDateL+1][i]<<endl;
		ArrivingDay=LeavingDay,ArrivingMinute=LeavingMinute+train.travelTimes[i];
		if(ArrivingMinute>=1440)ArrivingDay+=ArrivingMinute/1440,ArrivingMinute%=1440;
		if(i<=train.stationNum-2){
			LeavingDay=ArrivingDay,LeavingMinute=ArrivingMinute+train.stopoverTimes[i+1];
			if(LeavingMinute>=1440)LeavingDay+=LeavingMinute/1440,LeavingMinute%=1440;
		}
		SumPrice+=train.prices[i];
	}
}
void Train_System::delete_train(){
	string trainID;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-i")trainID=d_order[i+1];
	}
	//未找到该动车 不合法
	if(!TrainIndex.Find(trainID)){printf("-1\n");return;}
	Train train=GetTrainFromData(trainID);
	//动车已经发布，不合法
	if(train.isRelease){printf("-1\n");return;}
	//在TrainData中删除
	int pos=TrainIndex.FindAll(trainID)[0];
	// cout<<"Del "<<trainID<<" "<<pos<<endl;
	TrainData.Delete(pos);
	//在TrainIndex中删除
	TrainIndex.Delete(trainID,pos);
	//在StationIndex中删除
	for(int i=1;i<=train.stationNum;i++)StationIndex.Delete(train.stations[i],pos);
	printf("0\n");
}
void Train_System::query_ticket(){
	string startStation,endStation;
	int startday;
	string sortType="time";
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-s")startStation=d_order[i+1];
		if(d_order[i]=="-t")endStation=d_order[i+1];
		if(d_order[i]=="-d")startday=date_to_day(d_order[i+1]);
		if(d_order[i]=="-p")sortType=d_order[i+1];
	}
	//读取数据
	vector<int> G=StationIndex.FindAll(startStation);
	vector<Train> Alltrain;
	Train tmptrain;
	for(int i=0;i<(int)G.size();i++){
		TrainData.read(tmptrain,G[i]);
		Alltrain.push_back(tmptrain);
	}
	//选出合法数据并按照要求排序
	vector<Train> OKtrain;
	vector<int> Time,Cost;
	for(int i=0;i<(int)Alltrain.size();i++){
		// cout<<Alltrain[i].trainID<<endl;
		if(!Is_exist(Alltrain[i],startStation,endStation,startday))continue;
		if(!Alltrain[i].isRelease)continue;
		OKtrain.push_back(Alltrain[i]);
		Time.push_back(GetTime(Alltrain[i],startStation,endStation));
		Cost.push_back(GetCost(Alltrain[i],startStation,endStation));
	}
	int Num=(int)OKtrain.size();
	if(sortType=="time")tmp1=OKtrain,tmp2=Time,SortTrainTime(OKtrain,Time,0,Num-1);
	if(sortType=="cost")tmp1=OKtrain,tmp2=Cost,SortTrainCost(OKtrain,Cost,0,Num-1);
	//输出动车信息
	cout<<Num<<endl;
	for(int i=0;i<Num;i++){
		// if(sortType=="time")cout<<"time="<<GetTime(OKtrain[i],startStation,endStation)<<" ";
		// if(sortType=="cost")cout<<"cost="<<GetCost(OKtrain[i],startStation,endStation)<<" ";
		int firday=GetTrainStartDay(OKtrain[i],startStation,startday);
		cout<<OKtrain[i].trainID<<" ";
		cout<<startStation<<" ";
		cout<<GetLeavingTime(OKtrain[i],startStation,firday)<<" ";
		cout<<"-> ";
		cout<<endStation<<" ";
		cout<<GetArrivingTime(OKtrain[i],endStation,firday)<<" ";
		cout<<GetCost(OKtrain[i],startStation,endStation)<<" ";
		cout<<GetMaxSeatNum(OKtrain[i],startStation,endStation,firday)<<endl;
	}
}
void Train_System::query_transfer(){
	string startStation,endStation;
	int startday;
	string sortType="time";
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-s")startStation=d_order[i+1];
		if(d_order[i]=="-t")endStation=d_order[i+1];
		if(d_order[i]=="-d")startday=date_to_day(d_order[i+1]);
		if(d_order[i]=="-p")sortType=d_order[i+1];
	}
	//读取数据
	vector<int> G;
	Train tmptrain;
	vector<Train> Alltrain1,Alltrain2;
	G=StationIndex.FindAll(startStation);
	for(int i=0;i<(int)G.size();i++){
		TrainData.read(tmptrain,G[i]);
		Alltrain1.push_back(tmptrain);
		// cout<<"!!!"<<tmptrain.trainID<<endl;
	}
	G=StationIndex.FindAll(endStation);
	for(int i=0;i<(int)G.size();i++){
		TrainData.read(tmptrain,G[i]);
		Alltrain2.push_back(tmptrain);
		// cout<<"@@@"<<tmptrain.trainID<<endl;
	}
	//寻找最优解
	Train ans1,ans2;
	string ansTransferStation="";
	int ansTime,ansCost,ansTime1;
	int ansFirday1,ansFirday2;
	for(int i=0;i<(int)Alltrain1.size();i++){
		if(!Alltrain1[i].isRelease)continue;
		int count=0;
		for(int k=1;k<=Alltrain1[i].stationNum;k++){
			if(string(Alltrain1[i].stations[k])==startStation){
				if(!Is_exist(Alltrain1[i],startStation,Alltrain1[i].stations[Alltrain1[i].stationNum],startday))break;
				count=1;
				continue;
			}
			if(!count)continue;
			string transferStation=Alltrain1[i].stations[k];
			int firday1=GetTrainStartDay(Alltrain1[i],startStation,startday);
			string ArrivingDateTime=GetArrivingTime(Alltrain1[i],transferStation,firday1);
			int MaxSeatNum1=GetMaxSeatNum(Alltrain1[i],startStation,transferStation,firday1);
			if(!MaxSeatNum1)continue;
			// cout<<"!!!"<<i<<" "<<k<<endl;
			for(int j=0;j<(int)Alltrain2.size();j++){
				if(string(Alltrain1[i].trainID)==string(Alltrain2[j].trainID))continue;
				if(!Is_exist(Alltrain2[j],transferStation,endStation))continue;
				if(!Alltrain2[j].isRelease)continue;
				int firday2=GetTransferStartDay(Alltrain2[j],transferStation,ArrivingDateTime);
				if(firday2==-1)continue;
				int MaxSeatNum2=GetMaxSeatNum(Alltrain2[j],transferStation,endStation,firday2);
				while(!MaxSeatNum2){
					firday2++;
					if(firday2>Alltrain2[j].saleDateR)break;
					MaxSeatNum2=GetMaxSeatNum(Alltrain2[j],transferStation,endStation,firday2);
				}
				if(firday2>Alltrain2[j].saleDateR)continue;
				if(sortType=="time"){
					int Time1=GetTime(Alltrain1[i],startStation,transferStation);
					string DateTimeBegin=GetLeavingTime(Alltrain1[i],startStation,firday1);
					string DateTimeEnd=GetArrivingTime(Alltrain2[j],endStation,firday2);
					int Time=DateTime_to_minute(DateTimeEnd)-DateTime_to_minute(DateTimeBegin);
					if(ansTransferStation==""||Time<ansTime||(Time==ansTime&&Time1<ansTime1)){
						ans1=Alltrain1[i],ans2=Alltrain2[j];
						ansTransferStation=transferStation;
						ansTime=Time,ansTime1=Time1;
						ansFirday1=firday1,ansFirday2=firday2;
					}
				}
				if(sortType=="cost"){
					int Cost1=GetCost(Alltrain1[i],startStation,transferStation);
					int Cost2=GetCost(Alltrain2[j],transferStation,endStation);
					int Cost=Cost1+Cost2;
					int Time1=GetTime(Alltrain1[i],startStation,transferStation);
					// cout<<"!!!!"<<endl;
					// cout<<Alltrain1[i].trainID<<" "<<transferStation<<" "<<Alltrain2[j].trainID<<" "<<Cost<<" "<<Time1<<" "<<Cost1<<endl;
					if(ansTransferStation==""||Cost<ansCost||(Cost==ansCost&&Time1<ansTime1)){
						ans1=Alltrain1[i],ans2=Alltrain2[j];
						ansTransferStation=transferStation;
						ansCost=Cost,ansTime1=Time1;
						ansFirday1=firday1,ansFirday2=firday2;
					}
				}
			}
		}
	}
	// cout<<"!!!!!"<<day_to_date(ansFirday1)<<endl;
	// cout<<"!!!!!"<<day_to_date(ansFirday2)<<endl;
	//未找到方案
	if(ansTransferStation==""){printf("0\n");return;}
	//输出最优方案
	cout<<ans1.trainID<<" ";
	cout<<startStation<<" ";
	cout<<GetLeavingTime(ans1,startStation,ansFirday1)<<" ";
	cout<<"-> ";
	cout<<ansTransferStation<<" ";
	cout<<GetArrivingTime(ans1,ansTransferStation,ansFirday1)<<" ";
	cout<<GetCost(ans1,startStation,ansTransferStation)<<" ";
	cout<<GetMaxSeatNum(ans1,startStation,ansTransferStation,ansFirday1)<<endl;

	cout<<ans2.trainID<<" ";
	cout<<ansTransferStation<<" ";
	cout<<GetLeavingTime(ans2,ansTransferStation,ansFirday2)<<" ";
	cout<<"-> ";
	cout<<endStation<<" ";
	cout<<GetArrivingTime(ans2,endStation,ansFirday2)<<" ";
	cout<<GetCost(ans2,ansTransferStation,endStation)<<" ";
	cout<<GetMaxSeatNum(ans2,ansTransferStation,endStation,ansFirday2)<<endl;
}
void Train_System::buy_ticket(){
	string username;
	Order order;
	int startday;
	string AllowQueue="false";
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
		if(d_order[i]=="-i")strcpy(order.trainID,d_order[i+1].c_str());
		if(d_order[i]=="-d")startday=date_to_day(d_order[i+1]);
		if(d_order[i]=="-n")order.seatNum=string_to_int(d_order[i+1]);
		if(d_order[i]=="-f")strcpy(order.startStation,d_order[i+1].c_str());
		if(d_order[i]=="-t")strcpy(order.endStation,d_order[i+1].c_str());
		if(d_order[i]=="-q")AllowQueue=d_order[i+1];
	}
	order.timestamp=string_to_int2(d_order[1]);
	//未登录 不合法
	if(!Is_login[username]){printf("-1\n");return;}
	//找不到该动车 不合法
	if(!TrainIndex.Find(order.trainID)){printf("-1\n");return;}
	Train train=GetTrainFromData(order.trainID);
	//动车在这一天不发车 不合法
	if(!Is_exist(train,order.startStation,order.endStation,startday)){printf("-1\n");return;}
	if(!train.isRelease){printf("-1\n");return;}
	order.firday=GetTrainStartDay(train,order.startStation,startday);
	//票不够
	if(GetMaxSeatNum(train,order.startStation,order.endStation,order.firday)<order.seatNum){
		//购买失败
		if(AllowQueue=="false"){printf("-1\n");return;}
		//加入queue序列
		else {
			order.status=0;//queue
			int pos=OrderData.write(order);
			OrderIndex.insert(username,pos);
			QueueIndex.insert(order.trainID,pos);
			printf("queue\n");
		}
	}
	//票够
	else {
		//购买成功
		//添加order
		order.status=1;//buy
		int pos=OrderData.write(order);
		OrderIndex.insert(username,pos);
		//更新TrainData
		updateSeatNum(train,order.startStation,order.endStation,-order.seatNum,order.firday);
		int pos2=TrainIndex.FindAll(train.trainID)[0];
		TrainData.update(train,pos2);
		printf("%lld\n",(long long)order.seatNum*GetCost(train,order.startStation,order.endStation));
	}
}

void Train_System::query_order(){
	string username;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
	}
	//未登录 不合法
	if(!Is_login[username]){printf("-1\n");return;}
	//读取数据并排序
	vector<int> G=OrderIndex.FindAll(username);
	vector<Order> AllOrder;
	Order tmpOrder;
	int Num=(int)G.size();
	for(int i=0;i<Num;i++){
		OrderData.read(tmpOrder,G[i]);
		AllOrder.push_back(tmpOrder);
	}
	tmp3=AllOrder,tmp4=G,SortOrderTime(AllOrder,G,0,Num-1,1);
	//输出各个订单信息
	cout<<Num<<endl;
	for(int i=0;i<Num;i++){
		// cout<<AllOrder[i].timestamp<<" ";
		Train train=GetTrainFromData(AllOrder[i].trainID);
		if(AllOrder[i].status==1)cout<<"[success] ";
		if(AllOrder[i].status==0)cout<<"[pending] ";
		if(AllOrder[i].status==-1)cout<<"[refunded] ";
		cout<<AllOrder[i].trainID<<" ";
		cout<<AllOrder[i].startStation<<" ";
		cout<<GetLeavingTime(train,AllOrder[i].startStation,AllOrder[i].firday)<<" ";
		cout<<"-> ";
		cout<<AllOrder[i].endStation<<" ";
		cout<<GetArrivingTime(train,AllOrder[i].endStation,AllOrder[i].firday)<<" ";
		cout<<GetCost(train,AllOrder[i].startStation,AllOrder[i].endStation)<<" ";
		cout<<AllOrder[i].seatNum<<endl;
	}
}
void Train_System::refund_ticket(){
	string username;
	int K=1;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
		if(d_order[i]=="-n")K=string_to_int(d_order[i+1]);
	}
	//未登录 不合法
	if(!Is_login[username]){printf("-1\n");return;}
	//读取数据并排序，找到第K个订单
	vector<int> G=OrderIndex.FindAll(username);
	vector<Order> AllOrder;
	Order tmpOrder;
	int Num=(int)G.size();
	for(int i=0;i<Num;i++){
		OrderData.read(tmpOrder,G[i]);
		AllOrder.push_back(tmpOrder);
	}
	tmp3=AllOrder,tmp4=G,SortOrderTime(AllOrder,G,0,Num-1,1);
	//订单数<k 不合法
	if(K>Num){printf("-1\n");return;}
	Order order=AllOrder[K-1];
	int pos=G[K-1];
	//已经退票 不合法
	if(order.status==-1){printf("-1\n");return;}
	//若该订单在pending中,在queueIndex中将其删掉
	if(order.status==0)QueueIndex.Delete(order.trainID,pos);
	//若是原来已经购买了，要退票，更新TrainData
	if(order.status==1){
		Train train=GetTrainFromData(order.trainID);
		updateSeatNum(train,order.startStation,order.endStation,order.seatNum,order.firday);
		int pos2=TrainIndex.FindAll(train.trainID)[0];
		TrainData.update(train,pos2);
		//queueUpdate
		queueUpdate(train.trainID);
	}
	//更新OrderData
	order.status=-1;
	OrderData.update(order,pos);
	printf("0\n");
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