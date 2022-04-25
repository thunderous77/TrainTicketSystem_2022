# TrainTicketSystem_2022

## 指令说明

注：指令参数顺序可换

#### 用户相关

add_user -c "cur_username" -u "username" -p "password" -n "name" -m "mailAddr" -g "privilege"（特别的,创建的第一个用户忽略-c -g参数

login -u "username" -p "password"

logout -u "username"

query_profile -c "cur_username" -u "username"

modify_profile -c "cur_username" -u "username" (-p "password") (-m "mailAddr") (-g "privilege")

#### 车票相关

[”timestamp“] add_train -i "trainID" -n "stationNum" -m "seatNum" -s "stations" -p "prices" -x "startTime" -t "travelTimes" -o "stopoverTimes" -d "saleDate" -y "type"(其中-s,-p,-t,-o,-d可能由多个值组成，两个值之间以"|"隔开且不带空格

[”timestamp“] release_train -i "trainID"

[”timestamp“] query_train -i "trainID" -d "mm-dd"

[”timestamp“] query_ticket -s "startStation" -t "destinationStation" -d "mm-dd" (-p time/cost) (不加-p默认以票价排序)

[”timestamp“] query_transfer -s "startStation" -t "destinationStation" -d "mm-dd" (-p time/cost) (恰好换乘一次,只输出最优解)

[”timestamp“] buy_ticket -u "username" -i "trainID" -d "mm-dd" -n "number" -f "startStation" -t "destinationStation" (-q false/true) 

[”timestamp“] query_order -u "username"

[”timestamp“] refund_ticket -u "username" (-n "TDnumber")

[”timestamp“] rollback -t "preTime"

[”timestamp“] clean

[”timestamp“] exit

## 模块划分

#### 命令读取模块

```c++
string order;
int main(){
	while(getcline(cin,order)){
		try{
			AnalysisOrder(order);
		}
		catch(Exception Ex){
			if()printf("");
			if()printf("");
			if()printf("");
		}
	}
	return 0;
}
```



#### 用户模块



#### 火车票模块



#### 数据库存储（B+Tree）



#### 异常抛出