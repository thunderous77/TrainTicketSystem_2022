#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
int main(){
	char A[7]={'2','3','3','3','\0'};
	string B="2333";
	if(A==B)cout<<"!!"<<endl;
	// strcpy(A,B.c_str());
	// cout<<A<<endl;
	return 0;
}