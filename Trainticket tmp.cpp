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
extern long long Clock1,Clock2,Clock3,Clock4;

extern string OutputData;
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
static string int_to_string(int x){
	if(x==0){return "0";}
	int s[12],cnt=0;
	string str="";
	while(x)s[++cnt]=x%10,x/=10;
	while(cnt)str+=char(s[cnt]+'0'),cnt--;
	return str;
}
static string longlong_to_string(long long x){
	if(x==0){return "0";}
	int s[23],cnt=0;
	string str="";
	while(x)s[++cnt]=x%10,x/=10;
	while(cnt)str+=char(s[cnt]+'0'),cnt--;
	return str;
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
	// 	for(int j=1;j<G.stationNum;j++)cout<<G.seatNum[i][j]<<" ";
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
static void Output(Train_System::StationTrain G){
	cout<<"~~~~ "<<G.trainID<<" ~~~~"<<endl;
	cout<<"StationIndex="<<G.StationIndex<<endl;
	cout<<"sumprice="<<G.sumprice<<endl;
	cout<<"sumArrivingTime="<<G.sumArrivingTime<<endl;
	cout<<"sumLeavingTime="<<G.sumLeavingTime<<endl;
	cout<<"startTime="<<G.startTime<<endl;
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
static bool Is_exist2(const Train_System::StationTrain &stationtrain,const int &startday){
	int Day=startday-(stationtrain.startTime+stationtrain.sumLeavingTime)/1440;
	return stationtrain.saleDateL<=Day&&Day<=stationtrain.saleDateR;
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
int Train_System::GetMaxSeatNum(const Train_System::Train &train,const string &startStation,const string &endStation,const int &firday){
	DayTrain daytrain;
	DayTrainData.read(daytrain,train.DayseatPos[firday-train.saleDateL+1]);
	int MaxSeatNum=1e9,Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(string(train.stations[i])==startStation)Count=1;
		if(string(train.stations[i])==endStation)break;
		if(Count)MaxSeatNum=min(MaxSeatNum,daytrain.seatNum[i]);
	}
	return MaxSeatNum;
}
int Train_System::GetMaxSeatNum2(const Train_System::StationTrain &stationtrain,const int &L,const int &R,const int &firday){
	DayTrain daytrain;
	DayTrainData.read(daytrain,stationtrain.DayseatPos[firday-stationtrain.saleDateL+1]);
	int MaxSeatNum=1e9;
	for(int i=L;i<R;i++)MaxSeatNum=min(MaxSeatNum,daytrain.seatNum[i]);
	return MaxSeatNum;
}
void Train_System::updateSeatNum(Train_System::Train &train,const string &startStation,const string &endStation,const int &num,const int &firday){
	DayTrain daytrain;
	DayTrainData.read(daytrain,train.DayseatPos[firday-train.saleDateL+1]);
	int Count=0;
	for(int i=1;i<=train.stationNum;i++){
		if(string(train.stations[i])==startStation)Count=1;
		if(string(train.stations[i])==endStation)break;
		if(Count)daytrain.seatNum[i]+=num;
	}
	int pos=train.DayseatPos[firday-train.saleDateL+1];
	DayTrainData.update(daytrain,pos);
	
	int tmp_rollback=string_to_int2(d_order[1]);
	DayTrainData_rollback.write(tmp_rollback);
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
static int GetTrainStartDay2(const Train_System::StationTrain &stationtrain,const int &startday){
	return startday-(stationtrain.startTime+stationtrain.sumLeavingTime)/1440;
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
			if(L_ArrivingMinute>LeavingMinute)startday++;
			if(startday>train.saleDateR)return -1;
			if(startday<train.saleDateL)return train.saleDateL;
			return startday;
		}
	}
	return -1;
}
struct ansType{
	string trainID;
	int Time,Cost;
	string LeavingTime,ArrivingTime;
	int seatNum;
	ansType(string _trainID,int _Time,int _Cost,string _LeavingTime,string _ArrivingTime,int _seatNum):
		trainID(_trainID),Time(_Time),Cost(_Cost),LeavingTime(_LeavingTime),ArrivingTime(_ArrivingTime),seatNum(_seatNum){}
};

static void SortStationTrain(vector<Train_System::StationTrain> &Alltrain,int l,int r){
    if(l>=r)return;
    int g1=l,g2=r;
    Train_System::StationTrain tmp=Alltrain[(l+r)/2];
    while (g1<=g2) {
        while(string(Alltrain[g1].trainID)<string(tmp.trainID))++g1;
        while(string(tmp.trainID)<string(Alltrain[g2].trainID))--g2;
        if(g1<=g2)swap(Alltrain[g1],Alltrain[g2]),++g1,--g2;
    }
    if (l<g2)SortStationTrain(Alltrain,l,g2);
    if (g1<r)SortStationTrain(Alltrain,g1,r);
}
// static void SortStationTrain(vector<Train_System::StationTrain> &Alltrain,int l,int r){
// 	if(l>=r)return;
// 	int mid=(l+r)>>1;
// 	SortStationTrain(Alltrain,l,mid),SortStationTrain(Alltrain,mid+1,r);
// 	int g1=l,g2=mid+1,g=l;
// 	while(g1<=mid&&g2<=r){
// 		if(string(Alltrain[g1].trainID)<string(Alltrain[g2].trainID))tmp1[g]=Alltrain[g1],g++,g1++;
// 		else tmp1[g]=Alltrain[g2],g++,g2++;
// 	}
// 	while(g1<=mid)tmp1[g]=Alltrain[g1],g++,g1++;
// 	while(g2<=r)tmp1[g]=Alltrain[g2],g++,g2++;
// 	for(int i=l;i<=r;i++)Alltrain[i]=tmp1[i];
// 	return;
// }
static void SortAnsTime(vector<ansType> &res,int l,int r){
    if(l>=r)return;
    int g1=l,g2=r;
    ansType tmp=res[(l+r)/2];
    while (g1<=g2) {
        while(res[g1].Time<tmp.Time||(res[g1].Time==tmp.Time&&res[g1].trainID<tmp.trainID))++g1;
        while(tmp.Time<res[g2].Time||(tmp.Time==res[g2].Time&&tmp.trainID<res[g2].trainID))--g2;
        if(g1<=g2)swap(res[g1],res[g2]),++g1,--g2;
    }
    if (l<g2)SortAnsTime(res,l,g2);
    if (g1<r)SortAnsTime(res,g1,r);
}
// static void SortAnsTime(vector<ansType> &res,int l,int r){
// 	if(l>=r)return;
// 	int mid=(l+r)>>1;
// 	SortAnsTime(res,l,mid),SortAnsTime(res,mid+1,r);
// 	int g1=l,g2=mid+1,g=l;
// 	while(g1<=mid&&g2<=r){
// 		if(res[g1].Time<res[g2].Time||(res[g1].Time==res[g2].Time&&res[g1].trainID<res[g2].trainID))tmp2[g]=res[g1],g++,g1++;
// 		else tmp2[g]=res[g2],g++,g2++;
// 	}
// 	while(g1<=mid)tmp2[g]=res[g1],g++,g1++;
// 	while(g2<=r)tmp2[g]=res[g2],g++,g2++;
// 	for(int i=l;i<=r;i++)res[i]=tmp2[i];
// 	return;
// }
static void SortAnsCost(vector<ansType> &res,int l,int r){
    if(l>=r)return;
    int g1=l,g2=r;
    ansType tmp=res[(l+r)/2];
    while (g1<=g2) {
        while(res[g1].Cost<tmp.Cost||(res[g1].Cost==tmp.Cost&&res[g1].trainID<tmp.trainID))++g1;
        while(tmp.Cost<res[g2].Cost||(tmp.Cost==res[g2].Cost&&tmp.trainID<res[g2].trainID))--g2;
        if(g1<=g2)swap(res[g1],res[g2]),++g1,--g2;
    }
    if (l<g2)SortAnsCost(res,l,g2);
    if (g1<r)SortAnsCost(res,g1,r);
}
// static void SortAnsCost(vector<ansType> &res,int l,int r){
// 	if(l>=r)return;
// 	int mid=(l+r)>>1;
// 	SortAnsCost(res,l,mid),SortAnsCost(res,mid+1,r);
// 	int g1=l,g2=mid+1,g=l;
// 	while(g1<=mid&&g2<=r){
// 		if(res[g1].Cost<res[g2].Cost||(res[g1].Cost==res[g2].Cost&&res[g1].trainID<res[g2].trainID))tmp2[g]=res[g1],g++,g1++;
// 		else tmp2[g]=res[g2],g++,g2++;
// 	}
// 	while(g1<=mid)tmp2[g]=res[g1],g++,g1++;
// 	while(g2<=r)tmp2[g]=res[g2],g++,g2++;
// 	for(int i=l;i<=r;i++)res[i]=tmp2[i];
// 	return;
// }
static void SortOrderTime(vector<Train_System::Order> &AllOrder,int l,int r,int tp){//tp=0 timestamp从小到大 tp=1 timestamp从大到小
    if(l>=r)return;
    int g1=l,g2=r;
    Train_System::Order tmp=AllOrder[(l+r)/2];
    while (g1<=g2) {
        while((tp==0&&AllOrder[g1].timestamp<tmp.timestamp)||(tp==1&&AllOrder[g1].timestamp>tmp.timestamp))++g1;
        while((tp==0&&tmp.timestamp<AllOrder[g2].timestamp)||(tp==1&&tmp.timestamp>AllOrder[g2].timestamp))--g2;
        if(g1<=g2)swap(AllOrder[g1],AllOrder[g2]),++g1,--g2;
    }
    if (l<g2)SortOrderTime(AllOrder,l,g2,tp);
    if (g1<r)SortOrderTime(AllOrder,g1,r,tp);
}
// static void SortOrderTime(vector<Train_System::Order> &AllOrder,int l,int r,int tp){//tp=0 timestamp从小到大 tp=1 timestamp从大到小
// 	if(l>=r)return;
// 	int mid=(l+r)>>1;
// 	SortOrderTime(AllOrder,l,mid,tp),SortOrderTime(AllOrder,mid+1,r,tp);
// 	int g1=l,g2=mid+1,g=l;
// 	while(g1<=mid&&g2<=r){
// 		if((tp==0&&AllOrder[g1].timestamp<AllOrder[g2].timestamp)||(tp==1&&AllOrder[g1].timestamp>AllOrder[g2].timestamp))tmp4[g]=AllOrder[g1],g++,g1++;
// 		else tmp4[g]=AllOrder[g2],g++,g2++;
// 	}
// 	while(g1<=mid)tmp4[g]=AllOrder[g1],g++,g1++;
// 	while(g2<=r)tmp4[g]=AllOrder[g2],g++,g2++;
// 	for(int i=l;i<=r;i++)AllOrder[i]=tmp4[i];
// 	return;
// }
void Train_System::queueUpdate(const string &trainID){
	//读取数据并排序
	vector<Order> AllOrder=QueueIndex.FindAll(trainID);
	int Num=AllOrder.size();
	SortOrderTime(AllOrder,0,Num-1,0);
	for(int i=0;i<Num;i++){
		Train train=GetTrainFromData(AllOrder[i].trainID);
		// cout<<"!!!!!!!"<<AllOrder[i].timestamp<<" "<<AllOrder[i].seatNum<<" "<<GetMaxSeatNum(train,AllOrder[i].startStation,AllOrder[i].endStation,AllOrder[i].firday)<<endl;
		if(GetMaxSeatNum(train,AllOrder[i].startStation,AllOrder[i].endStation,AllOrder[i].firday)>=AllOrder[i].seatNum){

			//在QueueIndex中删除
			QueueIndex.Delete(AllOrder[i].trainID,AllOrder[i]);
			for_rollback<Order> tmp_rollback(string_to_int2(d_order[1]),-1,AllOrder[i].trainID,AllOrder[i]);
			QueueIndex_rollback.write(tmp_rollback);
			
			//在OrderIndex中修改
			OrderIndex.Delete(AllOrder[i].username,AllOrder[i]);//,cout<<"!!! "<<"Delete "<<AllOrder[i].username<<" "<<AllOrder[i].timestamp<<" "<<AllOrder[i].status<<endl;
			for_rollback<Order> tmp_rollback2(string_to_int2(d_order[1]),-1,AllOrder[i].username,AllOrder[i]);
			OrderIndex_rollback.write(tmp_rollback2);
			
			AllOrder[i].status=1;

			OrderIndex.insert(AllOrder[i].username,AllOrder[i]);//,cout<<"!!! "<<"insert "<<AllOrder[i].username<<" "<<AllOrder[i].timestamp<<" "<<AllOrder[i].status<<endl;
			for_rollback<Order> tmp_rollback3(string_to_int2(d_order[1]),1,AllOrder[i].username,AllOrder[i]);
			OrderIndex_rollback.write(tmp_rollback3);

			//更新DayTrainData
			updateSeatNum(train,AllOrder[i].startStation,AllOrder[i].endStation,-AllOrder[i].seatNum,AllOrder[i].firday);
		}
	}
}

Train_System::Train_System():TrainData("TrainData",true,true),DayTrainData("DayTrainData",true,true),
							 TrainIndex("TrainIndex"),StationIndex("StationIndex"),OrderIndex("OrderIndex"),QueueIndex("QueueIndex"),
							 TrainIndex_rollback("TrainIndex_rollback",false,false),StationIndex_rollback("StationIndex_rollback",false,false),
							 OrderIndex_rollback("OrderIndex_rollback",false,false),QueueIndex_rollback("QueueIndex_rollback",false,false),
							 TrainData_rollback("TrainData_rollback",false,false),DayTrainData_rollback("DayTrainData_rollback",false,false)
							{}
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
			// cout<<"!!"<<tmp[1]<<" "<<tmp[2]<<" "<<NewTrain.saleDateL<<" "<<NewTrain.saleDateR<<endl;
		}
		if(d_order[i]=="-y")NewTrain.type=d_order[i+1][0];
	}
	NewTrain.isRelease=0;
	NewTrain.MaxseatNum=seatNum;
	// for(int i=1;i<=NewTrain.saleDateR-NewTrain.saleDateL+1;i++)for(int j=1;j<NewTrain.stationNum;j++)NewTrain.seatNum[i][j]=seatNum;
	for(int i=1;i<=NewTrain.saleDateR-NewTrain.saleDateL+1;i++){
		DayTrain tmpdaytrain;
		for(int j=1;j<NewTrain.stationNum;j++)tmpdaytrain.seatNum[j]=seatNum;
		NewTrain.DayseatPos[i]=DayTrainData.write(tmpdaytrain);
		int tmp_rollback=string_to_int2(d_order[1]);
		DayTrainData_rollback.write(tmp_rollback);
	}
	//已经存在该动车 不合法
	if(TrainIndex.Find(NewTrain.trainID))throw Train_Is_Exist();
	// Output(NewTrain);
	//添加train
	int pos=TrainData.write(NewTrain);
	// cout<<"@@@@@@@"<<pos<<endl;

	int tmp_rollback2=string_to_int2(d_order[1]);
	TrainData_rollback.write(tmp_rollback2);

	// cout<<"GGG "<<NewTrain.trainID<<endl;
	TrainIndex.insert(NewTrain.trainID,pos);

	for_rollback<int> tmp_rollback(string_to_int2(d_order[1]),1,NewTrain.trainID,pos);
	TrainIndex_rollback.write(tmp_rollback);

	cout<<d_order[1]<<" ";
	printf("0\n");OutputData+="添加成功<br>";

	// Train Newtrain2=GetTrainFromData(NewTrain.trainID);
	// Output(Newtrain2);
}
void Train_System::release_train(){
	string trainID;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-i")trainID=d_order[i+1];
	}
	if(!TrainIndex.Find(trainID))throw Train_Not_Exist();
	Train train=GetTrainFromData(trainID);
	//动车已经发布 不合法
	if(train.isRelease)throw Train_Is_Release();
	//更新TrainData
	train.isRelease=1;
	int pos=TrainIndex.FindAll(trainID)[0];
	TrainData.update(train,pos);
	
	int tmp_rollback2=string_to_int2(d_order[1]);
	TrainData_rollback.write(tmp_rollback2);

	StationTrain stationtrain;
	strcpy(stationtrain.trainID,train.trainID);
	for(int i=0;i<MaxDay;i++)stationtrain.DayseatPos[i]=train.DayseatPos[i];
	stationtrain.saleDateL=train.saleDateL,stationtrain.saleDateR=train.saleDateR;
	stationtrain.startTime=train.startTime;
	stationtrain.StationIndex=1;
	stationtrain.sumprice=0;
	stationtrain.sumArrivingTime=-1;
	stationtrain.sumLeavingTime=0;
	for(int i=1;i<=train.stationNum;i++){
		// Output(stationtrain);
		StationIndex.insert(train.stations[i],stationtrain);
		
		for_rollback<StationTrain> tmp_rollback4(string_to_int2(d_order[1]),1,train.stations[i],stationtrain);
		StationIndex_rollback.write(tmp_rollback4);
		
		if(i==train.stationNum)continue;
		stationtrain.StationIndex++;
		stationtrain.sumprice+=train.prices[i];
		stationtrain.sumArrivingTime=stationtrain.sumLeavingTime+train.travelTimes[i];
		stationtrain.sumLeavingTime=stationtrain.sumArrivingTime+train.stopoverTimes[i+1];
	}

	cout<<d_order[1]<<" ";
	printf("0\n");OutputData+="发布成功<br>";
}
void Train_System::query_train(){
	string trainID;
	int day;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-i")trainID=d_order[i+1];
		if(d_order[i]=="-d")day=date_to_day(d_order[i+1]);
	}
	// cout<<trainID<<endl;
	//未找到该动车 不合法
	if(!TrainIndex.Find(trainID))throw Train_Not_Exist();
	//读取数据
	Train train=GetTrainFromData(trainID);
	// Output(train);
	if(day<train.saleDateL||day>train.saleDateR)throw Not_In_SaleDate();
	//输出动车信息
	cout<<d_order[1]<<" ";
	cout<<train.trainID<<" "<<train.type<<endl;
	OutputData+="查询成功<br>";
	OutputData+="列车ID："+string(train.trainID)+"<br>";
	OutputData+="列车类型：",OutputData+=train.type,OutputData+="<br>";
	int ArrivingDay,ArrivingMinute;
	int LeavingDay=day,LeavingMinute=train.startTime;
	int SumPrice=0;
	DayTrain daytrain;
	DayTrainData.read(daytrain,train.DayseatPos[day-train.saleDateL+1]);
	for(int i=1;i<=train.stationNum;i++){
		cout<<train.stations[i]<<" ";
		if(i==1)cout<<"xx-xx xx:xx ";
		else cout<<day_to_date(ArrivingDay)<<" "<<minute_to_time(ArrivingMinute)<<" ";
		cout<<"-> ";
		if(i==train.stationNum)cout<<"xx-xx xx:xx ";
		else cout<<day_to_date(LeavingDay)<<" "<<minute_to_time(LeavingMinute)<<" ";
		cout<<SumPrice<<" ";
		if(i==train.stationNum)cout<<"x"<<endl;
		else cout<<daytrain.seatNum[i]<<endl;
		
		OutputData+=string(train.stations[i])+" ";
		if(i==1)OutputData+="xx-xx xx:xx ";
		else OutputData+=string(day_to_date(ArrivingDay))+" "+string(minute_to_time(ArrivingMinute))+" ";
		OutputData+="-> ";
		if(i==train.stationNum)OutputData+="xx-xx xx:xx ";
		else OutputData+=string(day_to_date(LeavingDay))+" "+string(minute_to_time(LeavingMinute))+" ";
		OutputData+="从起始站到该站价格："+int_to_string(SumPrice)+" ";
		if(i==train.stationNum)OutputData+="到下一站剩余座位数:"+string("x")+"<br>";
		else OutputData+="到下一站剩余座位数:"+int_to_string(daytrain.seatNum[i])+"<br>";

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
	if(!TrainIndex.Find(trainID))throw Train_Not_Exist();
	Train train=GetTrainFromData(trainID);
	// Output(train);
	//动车已经发布，不合法
	if(train.isRelease)throw Train_Is_Release();
	//在TrainData中删除
	int pos=TrainIndex.FindAll(trainID)[0];
	// cout<<"Del "<<trainID<<" "<<pos<<endl;
	TrainData.Delete(pos);
	
	int tmp_rollback2=string_to_int2(d_order[1]);
	TrainData_rollback.write(tmp_rollback2);

	//在TrainIndex中删除
	TrainIndex.Delete(trainID,pos);

	for_rollback<int> tmp_rollback(string_to_int2(d_order[1]),-1,trainID,pos);
	TrainIndex_rollback.write(tmp_rollback);

	cout<<d_order[1]<<" ";
	printf("0\n");OutputData+="删除成功<br>";
}
void Train_System::query_ticket(){
	Clock1-=clock();
	string startStation,endStation;
	int startday;
	string sortType="time";
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-s")startStation=d_order[i+1];
		if(d_order[i]=="-t")endStation=d_order[i+1];
		if(d_order[i]=="-d")startday=date_to_day(d_order[i+1]);
		if(d_order[i]=="-p")sortType=d_order[i+1];
	}
	//读取并选出选出合法数据，并按照要求排序
	vector<StationTrain> Alltrain1=StationIndex.FindAll(startStation);
	vector<StationTrain> Alltrain2=StationIndex.FindAll(endStation);
	int Size1=Alltrain1.size(),Size2=Alltrain2.size();
	SortStationTrain(Alltrain1,0,Size1-1);
	SortStationTrain(Alltrain2,0,Size2-1);
	vector<ansType> res;
	int Num=0,j=0;
	for(int i=0;i<Size1;i++){
		while(j<Size2&&string(Alltrain2[j].trainID)<string(Alltrain1[i].trainID))j++;
		if(j==Size2||string(Alltrain1[i].trainID)!=string(Alltrain2[j].trainID))continue;
		if(Alltrain1[i].StationIndex>=Alltrain2[j].StationIndex)continue;
		if(!Is_exist2(Alltrain1[i],startday))continue;
		int firday=GetTrainStartDay2(Alltrain1[i],startday);
		int Time=Alltrain2[j].sumArrivingTime-Alltrain1[i].sumLeavingTime;
		int Cost=Alltrain2[j].sumprice-Alltrain1[i].sumprice;
		string LeavingTime=day_to_date(firday+(Alltrain1[i].startTime+Alltrain1[i].sumLeavingTime)/1440)+" "+minute_to_time((Alltrain1[i].startTime+Alltrain1[i].sumLeavingTime)%1440);
		string ArrivingTime=day_to_date(firday+(Alltrain2[j].startTime+Alltrain2[j].sumArrivingTime)/1440)+" "+minute_to_time((Alltrain2[j].startTime+Alltrain2[j].sumArrivingTime)%1440);
		int seatNum=GetMaxSeatNum2(Alltrain1[i],Alltrain1[i].StationIndex,Alltrain2[j].StationIndex,firday);
		res.push_back(ansType(Alltrain1[i].trainID,Time,Cost,LeavingTime,ArrivingTime,seatNum));
		Num++;
	}
	if(sortType=="time")SortAnsTime(res,0,Num-1);
	if(sortType=="cost")SortAnsCost(res,0,Num-1);
	//输出动车信息
	cout<<d_order[1]<<" ";
	cout<<Num<<endl;
	OutputData+="查询成功<br>";
	OutputData+="合法列车数："+int_to_string(Num)+"<br>";
	for(int i=0;i<Num;i++){
		cout<<res[i].trainID<<" ";
		cout<<startStation<<" ";
		cout<<res[i].LeavingTime<<" ";
		cout<<"-> ";
		cout<<endStation<<" ";
		cout<<res[i].ArrivingTime<<" ";
		cout<<res[i].Cost<<" ";
		cout<<res[i].seatNum<<endl;
		
		OutputData+=string(res[i].trainID)+" ";
		OutputData+=string(startStation)+" ";
		OutputData+=string(res[i].LeavingTime)+" ";
		OutputData+="-> ";
		OutputData+=string(endStation)+" ";
		OutputData+=string(res[i].ArrivingTime)+" ";
		OutputData+="价格："+int_to_string(res[i].Cost)+" ";
		OutputData+="剩余座位数："+int_to_string(res[i].seatNum)+"<br>";
	}
	Clock1+=clock();
}
void Train_System::query_transfer(){
	Clock2-=clock();
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
	vector<StationTrain> stationtrain1,stationtrain2;
	vector<Train> Alltrain1,Alltrain2;
	Train tmptrain1,tmptrain2;
	
	stationtrain1=StationIndex.FindAll(startStation);
	stationtrain2=StationIndex.FindAll(endStation);
	for(int i=0;i<(int)stationtrain1.size();i++)Alltrain1.push_back(GetTrainFromData(stationtrain1[i].trainID));
	for(int i=0;i<(int)stationtrain2.size();i++)Alltrain2.push_back(GetTrainFromData(stationtrain2[i].trainID));
	//寻找最优解
	Train ans1,ans2;
	string ansTransferStation="";
	int ansTime,ansCost;
	int ansFirday1,ansFirday2;
	for(int i=0;i<(int)Alltrain1.size();i++){
		tmptrain1=Alltrain1[i];
		if(!tmptrain1.isRelease)continue;
		int count=0;
		for(int k=1;k<=tmptrain1.stationNum;k++){
			if(string(tmptrain1.stations[k])==startStation){
				if(!Is_exist(tmptrain1,startStation,tmptrain1.stations[tmptrain1.stationNum],startday))break;
				count=1;
				continue;
			}
			if(!count)continue;
			string transferStation=tmptrain1.stations[k];
			int firday1=GetTrainStartDay(tmptrain1,startStation,startday);
			string ArrivingDateTime=GetArrivingTime(tmptrain1,transferStation,firday1);
			int MaxSeatNum1=GetMaxSeatNum(tmptrain1,startStation,transferStation,firday1);
			if(!MaxSeatNum1)continue;
			// cout<<"!!!"<<i<<" "<<k<<endl;
			for(int j=0;j<(int)Alltrain2.size();j++){
				tmptrain2=Alltrain2[j];
				if(string(tmptrain1.trainID)==string(tmptrain2.trainID))continue;
				if(!Is_exist(tmptrain2,transferStation,endStation))continue;
				if(!tmptrain2.isRelease)continue;
				int firday2=GetTransferStartDay(tmptrain2,transferStation,ArrivingDateTime);
				if(firday2==-1)continue;
				int MaxSeatNum2=GetMaxSeatNum(tmptrain2,transferStation,endStation,firday2);
				while(!MaxSeatNum2){
					firday2++;
					if(firday2>tmptrain2.saleDateR)break;
					MaxSeatNum2=GetMaxSeatNum(tmptrain2,transferStation,endStation,firday2);
				}
				if(firday2>tmptrain2.saleDateR)continue;
				string DateTimeBegin=GetLeavingTime(tmptrain1,startStation,firday1);
				string DateTimeEnd=GetArrivingTime(tmptrain2,endStation,firday2);
				int Time=DateTime_to_minute(DateTimeEnd)-DateTime_to_minute(DateTimeBegin);
				int Cost1=GetCost(tmptrain1,startStation,transferStation);
				int Cost2=GetCost(tmptrain2,transferStation,endStation);
				int Cost=Cost1+Cost2;
				// cout<<"!!!!"<<endl;
				// cout<<tmptrain1.trainID<<" "<<transferStation<<" "<<tmptrain2.trainID<<" "<<Cost<<" "<<Time<<endl;
				if(sortType=="time"){
					if(ansTransferStation==""||Time<ansTime||(Time==ansTime && (Cost<ansCost||(Cost==ansCost&& (tmptrain1.trainID<ans1.trainID || (tmptrain1.trainID==ans1.trainID&&tmptrain2.trainID<ans2.trainID) ) ) ) )){
						ans1=tmptrain1,ans2=tmptrain2;
						ansTransferStation=transferStation;
						ansTime=Time,ansCost=Cost;
						ansFirday1=firday1,ansFirday2=firday2;
					}
				}
				if(sortType=="cost"){
					if(ansTransferStation==""||Cost<ansCost||(Cost==ansCost && (Time<ansTime||(Time==ansTime&& (tmptrain1.trainID<ans1.trainID || (tmptrain1.trainID==ans1.trainID&&tmptrain2.trainID<ans2.trainID) ) ) ) )){
						ans1=tmptrain1,ans2=tmptrain2;
						ansTransferStation=transferStation;
						ansTime=Time,ansCost=Cost;
						ansFirday1=firday1,ansFirday2=firday2;
					}
				}
			}
		}
	}
	// cout<<"!!!!!"<<day_to_date(ansFirday1)<<endl;
	// cout<<"!!!!!"<<day_to_date(ansFirday2)<<endl;
	//未找到方案
	if(ansTransferStation==""){cout<<d_order[1]<<" ";printf("0\n");OutputData+="查询成功<br>未找到方案<br>";Clock2+=clock();return;}

	//输出最优方案
	OutputData+="查询成功<br>";
	cout<<d_order[1]<<" ";
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

	OutputData+=string(ans1.trainID)+" ";
	OutputData+=string(startStation)+" ";
	OutputData+=string(GetLeavingTime(ans1,startStation,ansFirday1))+" ";
	OutputData+="-> ";
	OutputData+=string(ansTransferStation)+" ";
	OutputData+=string(GetArrivingTime(ans1,ansTransferStation,ansFirday1))+" ";
	OutputData+="价格："+int_to_string(GetCost(ans1,startStation,ansTransferStation))+" ";
	OutputData+="剩余座位数："+int_to_string(GetMaxSeatNum(ans1,startStation,ansTransferStation,ansFirday1))+"<br>";

	OutputData+=string(ans2.trainID)+" ";
	OutputData+=string(ansTransferStation)+" ";
	OutputData+=string(GetLeavingTime(ans2,ansTransferStation,ansFirday2))+" ";
	OutputData+="-> ";
	OutputData+=string(endStation)+" ";
	OutputData+=string(GetArrivingTime(ans2,endStation,ansFirday2))+" ";
	OutputData+="价格："+int_to_string(GetCost(ans2,ansTransferStation,endStation))+" ";
	OutputData+="剩余座位数："+int_to_string(GetMaxSeatNum(ans2,ansTransferStation,endStation,ansFirday2))+"<br>";

	Clock2+=clock();
}
void Train_System::buy_ticket(){
	Order order;
	int startday;
	string AllowQueue="false";
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")strcpy(order.username,d_order[i+1].c_str());;
		if(d_order[i]=="-i")strcpy(order.trainID,d_order[i+1].c_str());
		if(d_order[i]=="-d")startday=date_to_day(d_order[i+1]);
		if(d_order[i]=="-n")order.seatNum=string_to_int(d_order[i+1]);
		if(d_order[i]=="-f")strcpy(order.startStation,d_order[i+1].c_str());
		if(d_order[i]=="-t")strcpy(order.endStation,d_order[i+1].c_str());
		if(d_order[i]=="-q")AllowQueue=d_order[i+1];
	}
	order.timestamp=string_to_int2(d_order[1]);
	//未登录 不合法
	if(!Is_login[order.username])throw User_Not_Login();
	//找不到该动车 不合法
	if(!TrainIndex.Find(order.trainID))throw Train_Not_Exist();
	Train train=GetTrainFromData(order.trainID);
	//动车在这一天不发车 不合法
	if(!Is_exist(train,order.startStation,order.endStation,startday))throw Not_In_SaleDate();
	if(!train.isRelease)throw Train_Not_Release();
	order.firday=GetTrainStartDay(train,order.startStation,startday);
	//票不够
	if(GetMaxSeatNum(train,order.startStation,order.endStation,order.firday)<order.seatNum){
		//购买失败
		if(AllowQueue=="false")throw Lack_Seat();
		//加入queue序列
		else {
			//购买票数大于总票数,购买失败
			if(train.MaxseatNum<order.seatNum)throw Lack_Seat();
			order.status=0;//queue

			OrderIndex.insert(order.username,order);//,cout<<"!!! "<<"insert "<<order.username<<" "<<order.timestamp<<" "<<order.status<<endl;
			for_rollback<Order> tmp_rollback(string_to_int2(d_order[1]),1,order.username,order);
			OrderIndex_rollback.write(tmp_rollback);
			
			QueueIndex.insert(order.trainID,order);
			for_rollback<Order> tmp_rollback_(string_to_int2(d_order[1]),1,order.trainID,order);
			QueueIndex_rollback.write(tmp_rollback_);

			cout<<d_order[1]<<" ";
			printf("queue\n");
			OutputData+="购票失败<br>";
			OutputData+="已加入等待序列<br>";
		}
	}
	//票够
	else {
		//购买成功
		//添加order
		order.status=1;//buy

		OrderIndex.insert(order.username,order);//,cout<<"!!! "<<"insert "<<order.username<<" "<<order.timestamp<<" "<<order.status<<endl;
		for_rollback<Order> tmp_rollback(string_to_int2(d_order[1]),1,order.username,order);
		OrderIndex_rollback.write(tmp_rollback);
		//更新DayTrainData
		updateSeatNum(train,order.startStation,order.endStation,-order.seatNum,order.firday);

		cout<<d_order[1]<<" ";
		printf("%lld\n",(long long)order.seatNum*GetCost(train,order.startStation,order.endStation));
		OutputData+="购票成功<br>";
		OutputData+="总价格："+longlong_to_string((long long)order.seatNum*GetCost(train,order.startStation,order.endStation))+"<br>";
	}
}

void Train_System::query_order(){
	Clock3-=clock();

	string username;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
	}
	//未登录 不合法
	if(!Is_login[username]){Clock3+=clock();throw User_Not_Login();}
	//读取数据并排序
	vector<Order> AllOrder=OrderIndex.FindAll(username);
	int Num=AllOrder.size();
	SortOrderTime(AllOrder,0,Num-1,1);
	//输出各个订单信息
	cout<<d_order[1]<<" ";
	cout<<Num<<endl;
	OutputData+="查询成功<br>";
	OutputData+="订单数："+int_to_string(Num)+"<br>";
	for(int i=0;i<Num;i++){
		// cout<<AllOrder[i].timestamp<<" ";
		Train train=GetTrainFromData(AllOrder[i].trainID);
		// Output(train);
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

		if(AllOrder[i].status==1)OutputData+="[success] ";
		if(AllOrder[i].status==0)OutputData+="[pending] ";
		if(AllOrder[i].status==-1)OutputData+="[refunded] ";
		OutputData+=string(AllOrder[i].trainID)+" ";
		OutputData+=string(AllOrder[i].startStation)+" ";
		OutputData+=string(GetLeavingTime(train,AllOrder[i].startStation,AllOrder[i].firday))+" ";
		OutputData+="-> ";
		OutputData+=string(AllOrder[i].endStation)+" ";
		OutputData+=string(GetArrivingTime(train,AllOrder[i].endStation,AllOrder[i].firday))+" ";
		OutputData+="总价格："+int_to_string(GetCost(train,AllOrder[i].startStation,AllOrder[i].endStation))+" ";
		OutputData+="购票数量："+int_to_string(AllOrder[i].seatNum)+"<br>";
	}

	Clock3+=clock();
}
void Train_System::refund_ticket(){
	Clock4-=clock();
	string username;
	int K=1;
	for(int i=3;i<=dcnt;i+=2){
		if(d_order[i]=="-u")username=d_order[i+1];
		if(d_order[i]=="-n")K=string_to_int(d_order[i+1]);
	}
	//未登录 不合法
	if(!Is_login[username]){Clock4+=clock();throw User_Not_Login();}
	//读取数据并排序，找到第K个订单
	vector<Order> AllOrder=OrderIndex.FindAll(username);
	int Num=AllOrder.size();
	SortOrderTime(AllOrder,0,Num-1,1);
	//订单数<k 不合法
	if(K>Num){Clock4+=clock();throw Order_Kth_Invalid();}
	Order order=AllOrder[K-1];
	//已经退票 不合法
	if(order.status==-1){Clock4+=clock();throw Already_Refund();}
	//若该订单在pending中,在queueIndex中将其删掉
	if(order.status==0){
		QueueIndex.Delete(order.trainID,order);
		for_rollback<Order> tmp_rollback(string_to_int2(d_order[1]),-1,order.trainID,order);
		QueueIndex_rollback.write(tmp_rollback);
	}
	//若是原来已经购买了，要退票，更新DayTrainData
	if(order.status==1){
		Train train=GetTrainFromData(order.trainID);
		//更改DayTrainData
		updateSeatNum(train,order.startStation,order.endStation,order.seatNum,order.firday);

		//queueUpdate
		queueUpdate(train.trainID);
	}
	//更新OrderIndex
	OrderIndex.Delete(order.username,order);//,cout<<"!!! "<<"Delete "<<order.username<<" "<<order.timestamp<<" "<<order.status<<endl;
	for_rollback<Order> tmp_rollback(string_to_int2(d_order[1]),-1,order.username,order);
	OrderIndex_rollback.write(tmp_rollback);

	order.status=-1;
	
	OrderIndex.insert(order.username,order);//,cout<<"!!! "<<"insert "<<order.username<<" "<<order.timestamp<<" "<<order.status<<endl;
	for_rollback<Order> tmp_rollback2(string_to_int2(d_order[1]),1,order.username,order);
	OrderIndex_rollback.write(tmp_rollback2);
	


	cout<<d_order[1]<<" ";
	printf("0\n");
	OutputData+="退票成功<br>";
	
	Clock4+=clock();
}
void Train_System::clean(){
	TrainData.clean();
	DayTrainData.clean();
	TrainIndex.clean();
	StationIndex.clean();
	OrderIndex.clean();
	QueueIndex.clean();
	TrainData_rollback.clean();
	DayTrainData_rollback.clean();
	TrainIndex_rollback.clean();
	StationIndex_rollback.clean();
	OrderIndex_rollback.clean();
	QueueIndex_rollback.clean();
	OutputData+="车票系统数据清除成功<br>";
	// cout<<sizeof(Train)/1024.0<<"kb"<<endl;
	// cout<<sizeof(Order)/1024.0<<"kb"<<endl;
}
void Train_System::rollback(){
	int Backtimestmp=string_to_int(d_order[4]);
	int timestamp=string_to_int2(d_order[1]);
	if(Backtimestmp>timestamp)throw Rollback_Timestamp_Error();
	// cout<<"!!!!!"<<Backtimestmp<<endl;
	for_rollback<int> tmp;
	for_rollback<StationTrain> tmp2;
	for_rollback<Order> tmp3;
	int tmp4;

	//回滚TrainIndex
	int pos=TrainIndex_rollback.Maxpos();
	while(pos!=-1){
		TrainIndex_rollback.read(tmp,pos);
		if(tmp.timestamp<Backtimestmp)break;
		// cout<<(tmp.type==1?"insert":"Delete")<<" "<<tmp.key<<" "<<tmp.val<<endl;
		if(tmp.type==1)TrainIndex.Delete(tmp.key,tmp.val);//在原来该时间戳insert(key,val)
		if(tmp.type==-1)TrainIndex.insert(tmp.key,tmp.val);//在原来该时间戳Delete(key,val)
		TrainIndex_rollback.Delete(pos,1);
		pos=TrainIndex_rollback.Maxpos();
	}

	//回滚StationIndex
	pos=StationIndex_rollback.Maxpos();
	while(pos!=-1){
		StationIndex_rollback.read(tmp2,pos);
		if(tmp2.timestamp<Backtimestmp)break;
		// cout<<(tmp2.type==1?"insert":"Delete")<<" "<<tmp2.key<<" "<<tmp2.val<<endl;
		if(tmp2.type==1)StationIndex.Delete(tmp2.key,tmp2.val);//在原来该时间戳insert(key,val)
		if(tmp2.type==-1)StationIndex.insert(tmp2.key,tmp2.val);//在原来该时间戳Delete(key,val)
		StationIndex_rollback.Delete(pos,1);
		pos=StationIndex_rollback.Maxpos();
	}

	//回滚OrderIndex
	pos=OrderIndex_rollback.Maxpos();
	while(pos!=-1){
		OrderIndex_rollback.read(tmp3,pos);
		if(tmp3.timestamp<Backtimestmp)break;
		// cout<<(tmp3.type==1?"insert":"Delete")<<" "<<tmp3.key<<" "<<tmp3.val<<endl;
		if(tmp3.type==1)OrderIndex.Delete(tmp3.key,tmp3.val);//,cout<<"!!! "<<"Delete "<<tmp3.key<<" "<<tmp3.val.timestamp<<" "<<tmp3.val.status<<endl;//在原来该时间戳insert(key,val)
		if(tmp3.type==-1)OrderIndex.insert(tmp3.key,tmp3.val);//,cout<<"!!! "<<"insert "<<tmp3.key<<" "<<tmp3.val.timestamp<<" "<<tmp3.val.status<<endl;//在原来该时间戳Delete(key,val)
		OrderIndex_rollback.Delete(pos,1);
		pos=OrderIndex_rollback.Maxpos();
	}

	//回滚QueueIndex
	pos=QueueIndex_rollback.Maxpos();
	while(pos!=-1){
		QueueIndex_rollback.read(tmp3,pos);
		if(tmp3.timestamp<Backtimestmp)break;
		// cout<<(tmp3.type==1?"insert":"Delete")<<" "<<tmp3.key<<" "<<tmp3.val<<endl;
		if(tmp3.type==1)QueueIndex.Delete(tmp3.key,tmp3.val);//在原来该时间戳insert(key,val)
		if(tmp3.type==-1)QueueIndex.insert(tmp3.key,tmp3.val);//在原来该时间戳Delete(key,val)
		QueueIndex_rollback.Delete(pos,1);
		pos=QueueIndex_rollback.Maxpos();
	}

	//回滚TrainData
	pos=TrainData_rollback.Maxpos();
	while(pos!=-1){
		TrainData_rollback.read(tmp4,pos);
		if(tmp4<Backtimestmp)break;
		// cout<<"@@@ TrainData "<<tmp4<<endl;
		TrainData.rollback();
		TrainData_rollback.Delete(pos,1);
		pos=TrainData_rollback.Maxpos();
	}

	//回滚DayTrainData
	pos=DayTrainData_rollback.Maxpos();
	while(pos!=-1){
		DayTrainData_rollback.read(tmp4,pos);
		if(tmp4<Backtimestmp)break;
		// cout<<"@@@ DayTrainData "<<tmp4<<endl;
		DayTrainData.rollback();
		DayTrainData_rollback.Delete(pos,1);
		pos=DayTrainData_rollback.Maxpos();
	}
}

#endif