#ifndef TRAINTICKET_CPP
#define TRAINTICKET_CPP

#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<fstream>
#include "Exceptions.hpp"
#include "Trainticket.h"
using namespace std;
extern string d_order[30];
extern int dcnt;
Train_System::Train_System():TrainData("TrainData"),OrderData("OrderData"),TrainIndex("TrainIndex"),StationIndex("StationIndex"),OrderIndex("OrderIndex"),QueueIndex("QueueIndex"){}
void Train_System::add_train(){
}
void Train_System::release_train(){
}
void Train_System::query_train(){
}
void Train_System::query_ticket(){
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
}
void Train_System::rollback(){
}

#endif