#ifndef TRAINTICKET_H
#define TRAINTICKET_H

#include "MemoryRiver.hpp"
#include "BlockList.hpp"
#include "User.h"
using namespace std;
class Train_System{
	#define MaxStation 103
	#define MaxDay 95
	#define MaxName 22
public:
	class Train{
	public:
		char trainID[MaxName];
		int stationNum;
		char stations[MaxStation][MaxName];
		int seatNum[MaxDay][MaxStation],prices[MaxStation];
		int startTime,travelTimes[MaxStation],stopoverTimes[MaxStation];
		int saleDateL,saleDateR;
		char type;
		bool isRelease=0;
	};
	class Order{
	public:
		int status;
		int trainID,Date,num;
		char startStation[22],EndStation[22];
		int timestamp;
	};
private:
	Train GetTrainFromData(const string &trainID);
	void queueUpdate(const string &trainID);
public:
	MemoryRiver<Train> TrainData;
	MemoryRiver<Order> OrderData;
	Key_value_database<int> TrainIndex,StationIndex,OrderIndex,QueueIndex;
	Train_System();
	void add_train();
	void release_train();
	void query_train();
	void query_ticket();
	void query_transfer();
	void buy_ticket();
	void query_order();
	void refund_ticket();
	void clean();
	void rollback();
};

#endif