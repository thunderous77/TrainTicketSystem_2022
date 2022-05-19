#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include<cstdio>
#include<cstring>
using namespace std;
extern string OutputData;
// class Username_Is_Exist{public:void Output(){printf("用户名已存在\n");OutputData+="用户名已存在<br>";}};
// class Username_Not_Exist{public:void Output(){printf("用户不存在\n");OutputData+="用户不存在<br>";}};
// class User_Not_Login{public:void Output(){printf("当前用户未登录\n");OutputData+="当前用户未登录<br>";}};
// class Invalid_Privilege{public:void Output(){printf("权限不足\n");OutputData+="权限不足<br>";}};
// class Already_Login{public:void Output(){printf("用户已登录\n");OutputData+="用户已登录<br>";}};
// class Wrong_User_Or_Password{public:void Output(){printf("用户不存在或密码错误\n");OutputData+="用户不存在或密码错误<br>";}};
// class Train_Is_Exist{public:void Output(){printf("列车ID已存在\n");OutputData+="列车ID已存在<br>";}};
// class Train_Not_Exist{public:void Output(){printf("列车ID不存在\n");OutputData+="列车ID不存在<br>";}};
// class Train_Is_Release{public:void Output(){printf("列车已发布\n");OutputData+="列车已发布<br>";}};
// class Train_Not_Release{public:void Output(){printf("列车未发布\n");OutputData+="列车未发布<br>";}};
// class Not_In_SaleDate{public:void Output(){printf("不在该列车运行时间内\n");OutputData+="不在该列车运行时间内<br>";}};
// class Lack_Seat{public:void Output(){printf("座位不足，购买失败\n");OutputData+="座位不足，购买失败<br>";}};
// class Order_Kth_Invalid{public:void Output(){printf("订单编号不合法\n");OutputData+="订单编号不合法<br>";}};
// class Already_Refund{public:void Output(){printf("该订单已经退票过，无需重复退票\n");OutputData+="该订单已经退票过，无需重复退票<br>";}};
// class Rollback_Timestamp_Error{public:void Output(){printf("回滚的时间戳不合法\n");OutputData+="回滚的时间戳不合法<br>";}};

class Username_Is_Exist{public:void Output(){printf("-1\n");}};
class Username_Not_Exist{public:void Output(){printf("-1\n");}};
class User_Not_Login{public:void Output(){printf("-1\n");}};
class Invalid_Privilege{public:void Output(){printf("-1\n");}};
class Already_Login{public:void Output(){printf("-1\n");}};
class Wrong_User_Or_Password{public:void Output(){printf("-1\n");}};
class Train_Is_Exist{public:void Output(){printf("-1\n");}};
class Train_Not_Exist{public:void Output(){printf("-1\n");}};
class Train_Is_Release{public:void Output(){printf("-1\n");}};
class Train_Not_Release{public:void Output(){printf("-1\n");}};
class Not_In_SaleDate{public:void Output(){printf("-1\n");}};
class Lack_Seat{public:void Output(){printf("-1\n");}};
class Order_Kth_Invalid{public:void Output(){printf("-1\n");}};
class Already_Refund{public:void Output(){printf("-1\n");}};
class Rollback_Timestamp_Error{public:void Output(){printf("-1\n");}};

#endif