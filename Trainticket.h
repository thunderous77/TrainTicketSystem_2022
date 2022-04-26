#ifndef TRAINTICKET_H
#define TRAINTICKET_H

using namespace std;
class Train_System{
public:
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