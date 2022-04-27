#ifndef TRAINTICKET_H
#define TRAINTICKET_H

#include "MemoryRiver.hpp"
#include "BlockList.hpp"
using namespace std;
class Train_System{
public:
	class Train{
		char trainID[22];
		int stationNum;
		char stations[103][22];
		int seatNum[62][103],prices[103];
		int startTime,travelTimes[103],stopoverTimes[103];
		int saleDateL,saleDataR;
		char type;
		bool isRelease=0;
	};
	class Order{
		int status;
		int trainID,Date,num;
		char startStation[22],EndStation[22];
		int timestamp;
	};
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