#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
using namespace std;
int main(){
	vector<int>W;
	W.push_back(0);
	W.push_back(1);
	W.push_back(2);
	W.push_back(3);
	W.push_back(4);
	W.push_back(5);
	swap(W[2],W[7]);
	cout<<W[2]<<endl;
	return 0;
}