//注意在编译时加上 -lwsock32 指令,表示 链接到WS2_32.lib动态库,其是Windows Sockets应用程序接口
#include <stdio.h> 
#include <iostream>
#include <winsock2.h> 
#include <stdlib.h>  
#include <conio.h>  
#include <stdio.h>
#include<cstring>
using namespace std;
class CInitSock{
public:
	CInitSock(BYTE minorVer=2, BYTE majorVer=2){
		// 初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion=MAKEWORD(minorVer, majorVer);
		if(::WSAStartup(sockVersion,&wsaData)!=0){exit(0);}
	}
	~CInitSock(){::WSACleanup();}
};
CInitSock initSock;//初始化Winsock库
int main(){
	system("chcp 65001");//更改格式，防止出现中文乱码
	// 创建套接字
	SOCKET sListen=::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//用来指定套接字使用的地址格式，通常使用AF_INET
	//指定套接字的类型，此处用的是SOCK_DGRAM，表示为udp不可靠传输
	//配合type参数使用，指定使用的协议类型（当指定套接字类型后，可以设置为0，因为默认为UDP或TCP）
	if(sListen==INVALID_SOCKET){
		printf("Failed socket()\n");
		return 0;
	}

	// 填充sockaddr_in结构 ,是个结构体
	/* struct sockaddr_in {

	short sin_family;  //地址族（指定地址格式） ，设为AF_INET
	u_short	sin_port; //端口号
	struct in_addr sin_addr; //IP地址
	char sin_zero[8]; //空子节，设为空
	} */

	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(9030);//端口号(1024 ~ 49151)
	sin.sin_addr.S_un.S_addr=INADDR_ANY;

	// 绑定这个套节字到一个本地地址
	if(::bind(sListen,(LPSOCKADDR)&sin,sizeof(sin))==SOCKET_ERROR){
		printf("Failed bind()\n");
		return 0;
	}

	// 进入监听模式
	//2指的是，监听队列中允许保持的尚未处理的最大连接数
	if(::listen(sListen,2)==SOCKET_ERROR){
		printf("Failed listen()\n");
		return 0;
	}

	while(TRUE){
		sockaddr_in remoteAddr;
		int nAddrLen=sizeof(remoteAddr);
		SOCKET sClient=0;
		// 接受一个新连接
		//（(SOCKADDR*)&remoteAddr）一个指向sockaddr_in结构的指针，用于获取对方地址
		sClient=::accept(sListen,(SOCKADDR*)&remoteAddr, &nAddrLen);
		if(sClient==INVALID_SOCKET){printf("Failed accept()");continue;}
		printf("接受到一个连接：%s\r\n",inet_ntoa(remoteAddr.sin_addr));
		//从客户端接收数据
		char data[300];
		int nRecv=::recv(sClient,data,300,0);//从socket缓存读取数据,300为接收数据字节上限,返回值为接收数据字节数
		if(nRecv>0){
			data[nRecv]='\0';
			printf("接收到数据：%s\n",data);
		}
		strcpy(data,"AA 6.10 10:00 -> BB 6.11 23:00");
		/*
		...
		处理数据
		*/
		// 向客户端发送数据
		::send(sClient,data,strlen(data),0);
		// 关闭同客户端的连接
		::closesocket(sClient);
	}
	// // 关闭监听套节字
	// ::closesocket(sListen);
	return 0; 
} 