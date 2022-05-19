#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<fstream>
#include "Exceptions.hpp"
#include "Order.h"
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;

string OutputData;
Order_System OrderSystem;

string Solve(string order){
	// OutputData=order+"<br>";
	OutputData="";
	try{
		OrderSystem.AnalysisOrder(order);
	}
	catch(Username_Is_Exist Ex){Ex.Output();}
	catch(User_Not_Login Ex){Ex.Output();}
	catch(Invalid_Privilege Ex){Ex.Output();}
	catch(Already_Login Ex){Ex.Output();}
	catch(Wrong_User_Or_Password Ex){Ex.Output();}
	catch(Username_Not_Exist Ex){Ex.Output();}
	catch(Train_Is_Exist Ex){Ex.Output();}
	catch(Train_Not_Exist Ex){Ex.Output();}
	catch(Train_Is_Release Ex){Ex.Output();}
	catch(Train_Not_Release Ex){Ex.Output();}
	catch(Not_In_SaleDate Ex){Ex.Output();}
	catch(Lack_Seat Ex){Ex.Output();}
	catch(Order_Kth_Invalid Ex){Ex.Output();}
	catch(Already_Refund Ex){Ex.Output();}
	catch(Rollback_Timestamp_Error Ex){Ex.Output();}
	return OutputData;
}

// int main() {
// 	// 创建套接字
// 	int slisten = socket(AF_INET, SOCK_STREAM, 0);
// 	//用来指定套接字使用的地址格式，通常使用AF_INET
// 	//指定套接字的类型，此处用的是SOCK_DGRAM，表示为udp不可靠传输
// 	//配合type参数使用，指定使用的协议类型（当指定套接字类型后，可以设置为0，因为默认为UDP或TCP）
	
// 	// 填充sockaddr_in结构 ,是个结构体
// 	/* struct sockaddr_in {

// 	short sin_family;  //地址族（指定地址格式） ，设为AF_INET
// 	u_short	sin_port; //端口号
// 	struct in_addr sin_addr; //IP地址
// 	char sin_zero[8]; //空子节，设为空
// 	} */
// 	sockaddr_in sin;
// 	sin.sin_family = AF_INET;
// 	sin.sin_port = htons(9030);//端口号(1024 ~ 49151)
// 	sin.sin_addr.s_addr = htonl(INADDR_ANY);

// 	// 绑定这个套节字到一个本地地址
// 	if (bind(slisten, (struct sockaddr *)&sin, sizeof(sin)) == -1){
// 		printf("Failed bind()\n");
// 		return 0;
// 	}

// 	// 进入监听模式
// 	//5指的是，监听队列中允许保持的尚未处理的最大连接数
// 	if (listen(slisten, 5) == -1) {
// 		printf("Failed listen()\n");
// 		return 0;
// 	}

// 	int sclient;
// 	sockaddr_in client_add;
// 	socklen_t naddrlen = sizeof(client_add);
// 	char data[100000];

// 	while (1) {
// 		memset(data, 0, sizeof(data));
// 		// 接受一个新连接
// 		//(struct sockaddr *)&client_add 一个指向sockaddr_in结构的指针，用于获取对方地址
// 		sclient = accept(slisten, (struct sockaddr *)&client_add, &naddrlen);
// 		if (sclient == -1) {printf("Failed accept()");continue;}
// 		//从客户端接收数据
// 		recv(sclient, data, 100000, 0);
// 		std::cout << "接受到一个连接："<<data<<endl;
// 		//处理数据
// 		std::string result = Solve(data);
// 		// 向客户端发送数据
// 		send(sclient, result.c_str(), result.length(), 0);
// 	}
// 	return 0;
// }



int main(){
	Order_System OrderSystem;
	string order;
	while(getline(cin,order)){
		Solve(order);
	}
	return 0;
}