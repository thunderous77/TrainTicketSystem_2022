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
	#define MaxStationName 32
public:
	class Train{
	public:
		char trainID[MaxTrainName];
		int stationNum;
		char stations[MaxStation][MaxStationName];
		int MaxseatNum,seatNum[MaxDay][MaxStation],prices[MaxStation];
		int startTime,travelTimes[MaxStation],stopoverTimes[MaxStation];
		int saleDateL,saleDateR;
		bool isRelease=0;
		char type;
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
public:
	MemoryRiver<Train> TrainData;
	MemoryRiver<Order> OrderData;
	Key_value_database<int> TrainIndex,StationIndex,OrderIndex,QueueIndex;
	MemoryRiver< for_rollback<int> > TrainIndex_rollback,StationIndex_rollback,OrderIndex_rollback,QueueIndex_rollback;
	MemoryRiver<int> TrainData_rollback,OrderData_rollback;
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