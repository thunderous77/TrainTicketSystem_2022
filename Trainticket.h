#ifndef TRAINTICKET_H
#define TRAINTICKET_H

#include "MemoryRiver.hpp"
#include "BlockList.hpp"
#include "User.h"
using namespace std;
class Train_System{
	#define MaxStation 102
	#define MaxDay 96
	#define MaxTrainName 22
	#define MaxStationName 46
public:
	class Train{
	public:
		char trainID[MaxTrainName];
		int stationNum;
		char stations[MaxStation][MaxStationName];
		int MaxseatNum;
		int DayseatPos[MaxDay];
		int prices[MaxStation];
		int startTime,travelTimes[MaxStation],stopoverTimes[MaxStation];
		int saleDateL,saleDateR;
		bool isRelease=0;
		char type;
		friend bool operator <(const Train &A,const Train &B){return string(A.trainID)<string(B.trainID);}
		friend bool operator >(const Train &A,const Train &B){return string(A.trainID)>string(B.trainID);}
		friend bool operator ==(const Train &A,const Train &B){return string(A.trainID)==string(B.trainID);}
		friend bool operator !=(const Train &A,const Train &B){return !(A==B);}
	};
	class DayTrain{
	public:
		int seatNum[MaxStation];
	};
	class StationTrain{
	public:
		char trainID[MaxTrainName];
		int StationIndex;
		int DayseatPos[MaxDay];
		int sumprice;
		int sumArrivingTime,sumLeavingTime;
		int saleDateL,saleDateR;
		int startTime;
		friend bool operator <(const StationTrain &A,const StationTrain &B){return string(A.trainID)<string(B.trainID)|| (string(A.trainID)==string(B.trainID)&&A.StationIndex<B.StationIndex);}
		friend bool operator >(const StationTrain &A,const StationTrain &B){return string(A.trainID)>string(B.trainID)|| (string(A.trainID)==string(B.trainID)&&A.StationIndex>B.StationIndex);}
		friend bool operator ==(const StationTrain &A,const StationTrain &B){return string(A.trainID)==string(B.trainID)&&A.StationIndex==B.StationIndex;}
		friend bool operator !=(const StationTrain &A,const StationTrain &B){return !(A==B);}
	};
	class Order{
	public:
		int status;//1:buy,0:queue,-1:refund
		char trainID[MaxTrainName];
		int firday,seatNum;
		char startStation[MaxStationName],endStation[MaxStationName];
		int timestamp;
	};
private:
	Train GetTrainFromData(const string &trainID);
	void queueUpdate(const string &trainID);
	int GetMaxSeatNum(const Train_System::Train &train,const string &startStation,const string &endStation,const int &firday);
	int GetMaxSeatNum2(const Train_System::StationTrain &stationtrain,const int &L,const int &R,const int &firday);
	void updateSeatNum(Train_System::Train &train,const string &startStation,const string &endStation,const int &num,const int &firday);
public:
	MemoryRiver<Train> TrainData;
	MemoryRiver<DayTrain> DayTrainData;
	MemoryRiver<Order> OrderData;
	Key_value_database<StationTrain> StationIndex;
	Key_value_database<int> TrainIndex,OrderIndex,QueueIndex;
	MemoryRiver< for_rollback<StationTrain> >StationIndex_rollback;
	MemoryRiver< for_rollback<int> > TrainIndex_rollback,OrderIndex_rollback,QueueIndex_rollback;
	MemoryRiver<int> TrainData_rollback,DayTrainData_rollback,OrderData_rollback;
	Train_System();
	void add_train();
	void release_train();
	void query_train();
	void delete_train();
	void query_ticket();
	void query_transfer();
	void buy_ticket();
	void query_order();
	void refund_ticket();
	void clean();
	void rollback();
};

#endif