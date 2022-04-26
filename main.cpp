#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<fstream>
#include "Exceptions.hpp"
#include "Order.h"
using namespace std;
int main(){
	Order_System OrderSystem;
	string order;
	while(getline(cin,order)){
		try{
			OrderSystem.AnalysisOrder(order);
		}
		catch(Order_Invalid Ex){
			Ex.Output();
		}
		catch(Other_Error Ex){
			Ex.Output();
		}
	}
	return 0;
}