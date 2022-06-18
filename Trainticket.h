#ifndef TRAINTICKET_H
#define TRAINTICKET_H

#include "MemoryRiver.hpp"
// #include "BlockList.hpp"
#include "CBPlusTree.hpp"
#include "User.h"
using namespace std;
class Train_System{
	#define MaxStation 101
	#define MaxDay 94
	#define MaxTrainName 21
	#define MaxStationName 31
public:
	class Train{
	public:
		char trainID[MaxTrainName];
		short stationNum;
		char stations[MaxStation][MaxStationName];
		int MaxseatNum;
		int sumprices[MaxStation];
		int startTime;
		// int travelTimes[MaxStation],stopoverTimes[MaxStation];
		int sumarrtime[MaxStation],sumleavingtime[MaxStation];
		short saleDateL,saleDateR;
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
		short StationIndex;
		int sumprice;
		int sumArrivingTime,sumLeavingTime;
		short saleDateL,saleDateR;
		int startTime;
		friend bool operator <(const StationTrain &A,const StationTrain &B){return string(A.trainID)<string(B.trainID)|| (string(A.trainID)==string(B.trainID)&&A.StationIndex<B.StationIndex);}
		friend bool operator >(const StationTrain &A,const StationTrain &B){return string(A.trainID)>string(B.trainID)|| (string(A.trainID)==string(B.trainID)&&A.StationIndex>B.StationIndex);}
		friend bool operator ==(const StationTrain &A,const StationTrain &B){return string(A.trainID)==string(B.trainID)&&A.StationIndex==B.StationIndex;}
		friend bool operator !=(const StationTrain &A,const StationTrain &B){return !(A==B);}
	};
	class Order{
	public:
		short status;//1:buy,0:queue,-1:refund
		char username[21];
		char trainID[MaxTrainName];
		short firday;
		int seatNum;
		char startStation[MaxStationName],endStation[MaxStationName];
		int timestamp;
		friend bool operator <(const Order &A,const Order &B){return A.timestamp<B.timestamp;}
		friend bool operator >(const Order &A,const Order &B){return A.timestamp>B.timestamp;}
		friend bool operator ==(const Order &A,const Order &B){return A.timestamp==B.timestamp;}
		friend bool operator !=(const Order &A,const Order &B){return !(A==B);}
	};
private:
	Train GetTrainFromData(const string &trainID);
	void queueUpdate(const string &trainID,const int Day);
	int GetMaxSeatNum(const Train_System::Train &train,const int &Lpos,const int &Rpos,const int &firday);
	int GetMaxSeatNum2(const Train_System::StationTrain &stationtrain,const int &L,const int &R,const int &firday);
	void updateSeatNum(Train_System::Train &train,const int &Lpos,const int &Rpos,const int &num,const int &firday);
public:
	MemoryRiver<Train> TrainData;
	// CBPlusTree<DayTrain,MaxStationName+4,20> DayTrainIndex;
	// CBPlusTree<StationTrain,MaxStationName,20> StationIndex;
	// CBPlusTree<int,MaxTrainName,10> TrainIndex;
	// CBPlusTree<Order,21,20> OrderIndex;
	// CBPlusTree<Order,MaxTrainName+4,20> QueueIndex;
	CBPlusTree<DayTrain,MaxStationName+4> DayTrainIndex;
	CBPlusTree<StationTrain,MaxStationName> StationIndex;
	CBPlusTree<int,MaxTrainName> TrainIndex;
	CBPlusTree<Order,21> OrderIndex;
	CBPlusTree<Order,MaxTrainName+4> QueueIndex;
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