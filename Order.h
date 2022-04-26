#ifndef ORDER_H
#define ORDER_H

#include<cstring>
#include "User.h"
#include "Trainticket.h"
using namespace std;
class Order_System{
private:
	User_System UserSystem;
	Train_System TrainSystem;
	void DivideOrder(string order);
public:
	void AnalysisOrder(string order);
};
#endif