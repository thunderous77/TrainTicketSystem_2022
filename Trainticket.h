#ifndef TRAINTICKET_H
#define TRAINTICKET_H

#include "MemoryRiver.hpp"
// #include "BlockList.hpp"
#include "CBPlusTree.hpp"
#include "User.h"
using namespace std;
class Train_System{
	#define MaxStation 102
	#define MaxDay 96
	#define MaxTrainName 22
	#define MaxStationName 32
public:
	class Train{
	public:
		char trainID[MaxTrainName];
		int stationNum;
		char stations[MaxStation][MaxStationName];
		int MaxseatNum;
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
		friend bool operator <(const DayTrain &A,const DayTrain &B){return 0;}
		friend bool operator >(const DayTrain &A,const DayTrain &B){return 0;}
		friend bool operator ==(const DayTrain &A,const DayTrain &B){return 1;}
		friend bool operator !=(const DayTrain &A,const DayTrain &B){return !(A==B);}
	};
	class StationTrain{
	public:
		char trainID[MaxTrainName];
		int StationIndex;
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
		char username[22];
		char trainID[MaxTrainName];
		int firday,seatNum;
		char startStation[MaxStationName],endStation[MaxStationName];
		int timestamp;
		friend bool operator <(const Order &A,const Order &B){return A.timestamp<B.timestamp;}
		friend bool operator >(const Order &A,const Order &B){return A.timestamp>B.timestamp;}
		friend bool operator ==(const Order &A,const Order &B){return A.timestamp==B.timestamp;}
		friend bool operator !=(const Order &A,const Order &B){return !(A==B);}
	};
private:
	Train GetTrainFromData(const string &trainID);
	void queueUpdate(const string &trainID);
	int GetMaxSeatNum(const Train_System::Train &train,const string &startStation,const string &endStation,const int &firday);
	int GetMaxSeatNum2(const Train_System::StationTrain &stationtrain,const int &L,const int &R,const int &firday);
	void updateSeatNum(Train_System::Train &train,const string &startStation,const string &endStation,const int &num,const int &firday);
public:
	MemoryRiver<Train> TrainData;
	CBPlusTree<DayTrain,MaxStationName+5> DayTrainIndex;
	CBPlusTree<StationTrain,MaxStationName> StationIndex;
	CBPlusTree<int,MaxTrainName> TrainIndex;
	CBPlusTree<Order,22> OrderIndex;
	CBPlusTree<Order,MaxTrainName> QueueIndex;
	// Key_value_database<DayTrain> DayTrainIndex;
	// Key_value_database<StationTrain> StationIndex;
	// Key_value_database<int> TrainIndex;
	// Key_value_database<Order> OrderIndex;
	// Key_value_database<Order> QueueIndex;
	MemoryRiver< for_rollback<StationTrain> >StationIndex_rollback;
	MemoryRiver< for_rollback<int> > TrainIndex_rollback;
	MemoryRiver< for_rollback<DayTrain> > DayTrainIndex_rollback;
	MemoryRiver< for_rollback<Order> >OrderIndex_rollback,QueueIndex_rollback;
	MemoryRiver<int> TrainData_rollback;
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