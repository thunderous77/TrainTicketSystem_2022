# 🚂火车票管理系统

> SJTU CS1951 课程大作业

## 概况

### 教学安排

#### 分组说明

- 本作业要求学生组队完成
- 本班共分为 16 组+1人 ，每组两位学生
- 每组需要由一位 A 班同学与一位 B 班同学组成（A 班包括 ACM 队队员）
- 分组方式为自行组队，确认分组后请在 [该QQ文档](https://docs.qq.com/sheet/DRVFoS2RYcFZsdHBr) 中进行登记
- 分组的DDL为4月25日晚23:59分前，逾期未填写的同学将由助教随机分组。
- 想要独立完成本作业的学生请联系助教
- 本作业要求在github的项目管理中，使用git的分支协作开发，维持良好的项目管理习惯。
  - 本项目的中期检查等检查方式均会通过查看登记的git仓库链接，因此如果想更换仓库的链接请及时联系助教。
  - 本部分的相关分数会在cr中检查。

#### 截止日期

- 5月3日 23:00 前提交文档
- 6月19日 23:00 前通过 Online Judge 测试

### 项目概述

#### 主要功能

- 本次作业要求实现一个类似于 [12306](https://www.12306.cn/) 的火车票订票系统，该系统向用户提供购票业务相关功能，包括车票查询、购票、订单操作等，以及向管理员提供后台管理功能
- 系统需将用户数据、购票数据、车次数据进行**本地存储**，并对其进行高效操作

#### 实现方式

- 项目主体为一个实现后端功能的可执行文件
- 程序通过标准输入输出进行交互，具体输入指令与输出格式见下说明
  - 程序在接受完整命令后应立即输出结果并再次进入等待输入状态
- 限定编程语言为 C++
- 源代码中仅允许调用 `iostream`, `cstring`, `cstdio`, `cmath`, `fstream` 五个库以及`STLite`作业里允许使用的库，如果有其他需求可以联系助教



## 评分标准

### 概述

> 本作业满分 100%，占本课程总成绩 10%
> 在所有测试中，内存使用均会被严格限制。测试过程中程序可能会被**多次**打开关闭
> 允许在可执行文件所在目录创建**不超过 50 个文件**
> 评测方式：评测代码通过 **git** 提交，在仓库根目录提供 Makefile 或者 cmakelist 即可

### A. 基础分 - 85%

基础分各项目及对应分数如下：

- **性能 - 75%**
  - 正确性测试 - 35%
    - 在正确性测试中，每一个测试点都有一个相对宽松的时间和磁盘使用限制，以仅检验程序的正确性和鲁棒性，只要通过测试即可得到满分。因此请不要尝试针对特定情况进行有损正确性和鲁棒性的优化。
  - 压力测试 - 30%
    - 在压力测试中，每一个测试点会有两档时间和磁盘限制，通过所有Easy的测试可以得到 (60% * 30% =) 18% 的分数，通过所有Hard测试可以得到另外 (40% * 30% =)  12% 的分数。
  - 回滚测试 - 10%
    - 在回滚测试中，测试点内可能存在 `rollback` 指令用于测试回滚功能。同样会有时间和磁盘的限制。

- **文档 - 10%**
  - 文档方面要求提供完整的 **《开发文档》**(包括模块划分图、每个模块的功能、类设计、文件设计等) 以及**各个组员工作情况**的说明。
  - DDL：**5月3日23:00**前提交到github仓库上。


### B. bonus - 15%

- 最终 bonus 得分为完成的各项 bonus 评分之和与 15% 中取较低者，**分数不会溢出**。
> 如果正确性测试未通过则无法获得bonus的任何分数

以下为可供选择的 bonus 及其对应分数：

#### 后端

> 推荐学习数据库相关算法，或是在不影响正常功能的情况下，添加其他特性
- **添加缓存**（推荐）
  - 对于一棵 B+ 树，有一些节点我们会进行反复的读取，比如根节点、根节点的子节点，以及一些经常访问到的数据等。我们如果把部分这样的数据放在内存中，就可以提升程序的运行速度（当然，你不能把整棵 B+ 树放在内存中，因为我们会限制你使用的内存大小）。
    - 分数：3%~6% （根据缓存实现难度给分）

- **B+ 树空间回收**（推荐）
  - 在删除节点后，能否把那个删除掉的节点的空间利用起来呢？请尝试实现空间回收功能。

  - 分数：3%

- **命令行交互扩展**（推荐）
  - 在我们的实现中，所有的错误信息都只会返回`-1`，成功的信息也只会返回`0`，用户无法通过返回值判断自己的操作哪里出现了问题。请输出一些信息提示用户，以实现更好的命令行交互体验。例如，登录失败是因为用户名不存在、还是密码错误？
  - 本任务不会设置OnlineJudge测试，评测方式为在code review阶段向助教手动展示。
  - 分数：最高2%


- **并发**
  - 中国的 12306 火车票系统以变态的高并发性能闻名。所谓高并发，通俗来说就是同时接收处理许多来自客户的请求。当代家用和服务器CPU核心都很多，可以同时运行很多线程，在本次大作业中我们鼓励大家对后端程序主逻辑以及 B+ 树运用多线程优化。对于做并发 bonus 的同学，鼓励大家自行设计一些测试来证明自己运行效率的优势，通过 Code Review 进行展示
    - **⚠ 注意**：准备尝试此 bonus 的同学请务必与助教联系并讨论自己的实现思路
    - 分数：最高 15%

- **数据库的导出 / 备份 / 增量备份**
  - 对于一个强大的数据库系统来说，支持备份与恢复功能是非常重要的。这极大提升了你的数据库的安全性，如果一个数据库拥有了定时自动备份的功能，就算被误操作kill掉了，仍旧可以恢复到最近的一次备份状态
    - 第一次备份必须是完全的备份。接下来的备份可以是完全备份，或者是增量备份，这取决于你对安全和性能的平衡。增量备份可以支持恢复到任何一个之前的版本
    - 同时，你可以用你的备份功能来支持数据库的迁移。在另一台全新的机器上，只要拥有了你的可执行程序和导出的数据包，就可以继续该数据库之前的执行状态
    - **⚠ 注意**：准备尝试此 bonus 的同学请务必与助教联系并讨论自己的实现思路
    - 分数：最高 10%


#### 前端

- 提供一个用户友好的**图形操作页面**，推荐实现跨平台的前端，如网页，Qt 等。基本要求为：
  1. 前后端分离
  2. 用户能通过用户友好的图形界面完成后端接口提供的任何操作，并且能够实现前后端通信
  3. 提供完整的 **《使用手册》**(包括系统安装手册、用户手册等)
- **⚠ 注意**：准备尝试此部分 bonus 的同学请务必与助教联系并讨论自己的实现思路
- 分数：最高 15% 

#### bonus申请
  - 💡 准备自行设计并实现其他 bonus 的同学可以联系助教协商。
  - 较为复杂的bonus（除缓存，B+树空间回收，命令行交互扩展之外的bonus）需要在第十四周周日6月5日之前向助教提出申请。

### C. 扣分

- 每位小组成员都需要有绝对参与度，不要求相对参与度，小组内自己协商工作。我们建议有两位成员的小组，一位同学写B+树主体，一位同学写后端主体逻辑部分，若小组内不是这样的分工，请联系助教说明分工情况。若小组内某位组员参与度过低，会扣除一定分数，情况恶劣者扣分无上限。
- 请保证自己项目结构的可读性，可以包括优化项目结构、完善README的内容、适当的文件树指南等，晦涩难懂的项目可能会加大助教的工作量，也可能会影响你的成绩。
- 如有出现任何抄袭现象按0分计。

## 系统说明

> 以下说明中 “整数” 均为非负整数

### 用户相关

账户数据涉及以下内容：

- `username`：用户的账户名，作为用户的**唯一**标识符，为由字母开头，由字母、数字和下划线组成的字符串，长度不超过 20。
- `password`：由可见字符组成的字符串，长度不低于 1，不超过 30。
- `name`：用户真实姓名，由 2 至 5 个汉字组成的字符串。
- `mailAddr`：同一般邮箱地址格式，且仅含数字、大小写字母，`@`和`.`，长度不超过 30（**无需检验邮箱格式的合法性**）。
- `privilege`：所处用户组优先级，为 0~10 中的一个整数。

### 车次相关

#### 车次数据涉及以下属性：

- `trainID`：车次的**唯一**标识符，由字母开头，字母、数字和下划线组成的字符串，长度不超过 20。
  
- `stationNum`：车次经过的车站数量，一个不低于 2 且不超过 100 的整数。
  
- `stations`：车次经过的所有车站名，共 `stationNum` 项，每个站名由汉字组成，不超过 10 个汉字。
- `seatNum`：该车次的座位数，一个不超过 $10^5$ 的整数。
  
- `prices`：每两站之间的票价，共 `(stationNum - 1)` 项，第 `i` 项表示第 `i` 站到第 `(i+1)` 站的票价，为一个不超过 $10^5$ 的整数。
- `startTime`：列车每日的发车时间。
  - 时间格式为 `hh:mm` ，例如 `23:51`
- `travelTimes`：每两站之间行车所用的时间，共 `(stationNum - 1)` 项。
  - 单位：分钟
  - 每一项是一个不超过 $10^4$ 的整数
- `stopoverTimes`：除始发站和终点站之外，列车在每一站停留的时间，共 `(stationNum - 2)` 项。
  - 单位：分钟
  - 每一项为一个不超过 $10^4$ 的整数
- `saleDate`：车次的售卖时间区间。
  - 由 2 项时间表示，每一项均为 2021 年 6 月至 8 月的某一日期
  - 日期格式为 `mm-dd`，例如：`06-01`、`08-31`
- `type`：列车类型，一个大写字母。

#### 具体说明：

- 在时间区间（`saleDate`）内，该车次每日发一班车

- 关于 `travelTimes` 和 `stopoverTimes`：保证每一辆车从始发站到终点站用时不超过 72 小时

### 指令说明

#### 指令概述

- 每一条指令有一个 “常用度” 参数，为其在实际情况中的常用程度，也体现其在压力测试中出现频率，你可以将其作为设计算法的依据。“常用度” 分为以下几个等级，括号中数字为最大数据中该指令出现次数：
  - **[SF]**：Super frequently used (~1000000)
  - **[F]**：Frequently used (~100000)
  - **[N]**：Normally used (~10000)
  - **[R]**：Rarely used (~100)

- 一条指令的格式为 `[<timestamp>] <cmd> -<key_1> <argument_1> -<key_2> <argument_2> ...`
  - 其中 `<key>` 为一个小写字母。为简化描述，下文将在参数列表中省略 `<argument>` 并在说明中以 `-<key>` 代指其对应的参数
  - 如无特殊说明，每个 `<argument>` 都是一个不含空格的字符串
- 为了模拟更加真实的操作，每条指令之前都会有一个时间戳 `<timestamp>` 表示这条指令接收到的时间
    - 时间戳为一个整数
    - 各个指令间时间戳的值严格单调递增
    - 时间戳的意义仅在于当我们需要进行回滚操作的时候，可以定位到我们需要回滚的地方
- 指令参数列表的顺序不限，可选参数可以省略。对于一些指令，可选参数具有初始值。
- 指令的返回值为命令行交互中输出在标准输出流中的执行结果字符串

#### 指令列表

##### [N] `add_user`

- 参数列表
  - `-c -u -p -n -m -g`
- 返回值
  - 注册成功：`0`
  - 注册失败：`-1`
- 权限要求
  - `-c` 已登录，且新用户的权限低于 `-c` 的权限

- 说明
  - 当前用户 `-c`（`<cur_username>`），创建一个账户名为 `-u`（`<username>`），密码为 `-p`（`<password>`），用户真实姓名为 `-n`（`<name>`），用户邮箱为 `-m`（`<mailAddr>`），权限为 `-g`（`<privilege>`）的用户
  - 特别地，创建第一个用户时，新用户权限为 `10`，不受上述权限规则约束。具体来讲，当创建第一个用户时，忽略 `-c` 和 `-g` 参数，并认为新用户优先级为 `10`
  - 如果 `<username>` 已经存在则注册失败


##### [F] `login`

  - 参数列表

    `-u -p`

  - 说明

    用户`-u`(`<username>`)尝试用`-p`（`<password>`）登录。
    若登录成功，当前用户列表增加此用户。第一次进入系统时，当前用户列表为空。
    若用户不存在或密码错误或者用户已经登录，则登陆失败。
  - 返回值

    登录成功：`0`

    登录失败：`-1`

##### [F] `logout`

  - 参数列表

    `-u`

  - 说明

    `-u`（`<username>`）用户退出登录。若退出成功，将此用户移出当前用户列表。

  - 返回值

    登出成功：`0`

    登出失败：`-1`

##### [SF] `query_profile`

  - 参数列表

    `-c -u`

  - 说明

    用户`-c`（`<cur_username>`）查询`-u`(`<username>`)的用户信息。

    权限要求：`-c`已登录，且「`-c`的权限大于`-u`的权限，或是`-c`和`-u`相同」。

  - 返回值

    查询成功：一行字符串，依次为被查询用户的`<username>`，`<name>`，`<mailAddr>`和`<privilege>`，用一个空格隔开。

    查询失败：`-1`

##### [F] `modify_profile`

  - 参数列表

    `-c -u (-p) (-n) (-m) (-g)`

  - 说明

    用户`-c`(`<cur_username>`)修改`-u`(`<username>`)的用户信息。修改参数同注册参数，且均可以省略。

    权限要求：`-c`已登录，且「`-c`的权限大于`-u`的权限，或是`-c`和`-u`相同」，且`-g`需低于`-c`的权限。

  - 返回值

    修改成功：返回被修改用户的信息，格式同`query_profile`。
  
    修改失败：`-1`

##### [N] `add_train`

  - 参数列表
  
    `-i -n -m -s -p -x -t -o -d -y`
  
  - 说明
  
     添加`<trainID>`为`-i`，`<stationNum>`为`-n`，`<seatNum>`为`-m`，`<stations>`为`-s`，`<prices>`为`-p`，`<startTime>`为`-x`，`<travelTimes>`为`-t`，`<stopoverTimes>`为`-o`，`<saleDate>`为`-d`，`<type>`为`-y`的车次。
    由于`-s`、`-p`、`-t`、`-o`和`-d`由多个值组成，输入时两个值之间以`|`隔开（仍是一个不含空格的字符串）。
    
    输入保证火车的座位数大于0,站的数量不少于2不多于100，且如果火车只有两站 `-o`后的参数用下划线代替（见举例2）,且火车不会经过同一个站两次。
    如果`<trainID>`已经存在则添加失败。

  - 返回值
  
    添加成功：`0`
  
    添加失败：`-1`
    
  - 举例1：
  
    `>[666] add_train -i HAPPY_TRAINA -n 3 -m 1000 -s 上院|中院|下院 -p 114|514 -x 19:19 -t 600|600 -o 5 -d 06-01|08-17 -y G`
    
    `0`
  - 举例2：
  
    `>[667] add_train -i HAPPY_TRAINB -n 2 -m 1000 -s 上院|下院 -p 114 -x 19:19 -t 600 -o _ -d 06-01|08-17 -y G`
    
    `0`

##### [N] `delete_train`

  - 参数列表

    `-i`

  - 说明

    删除`<trainID>`为`-i`的车次，删除车次必须保证未发布。

  - 返回值

    删除成功：`0`

    删除失败：`-1`
    
##### [N] `release_train`

  - 参数列表

    `-i`

  - 说明

    将车次`-i`(`<trainID>`)发布。发布前的车次，不可发售车票；发布后的车次不可被删除，可发售车票。

  - 返回值

    发布成功：`0`

    发布失败：`-1`

  - 举例：

    `>[668] release_train -i HAPPY_TRAIN `

    `0` 

##### [N] `query_train`

  - 参数列表

    `-i -d`

  - 说明

    查询在日期`-d`发车的，车次`-i`(`<trainID>`)的情况，`-d`的格式为`mm-dd`。

  - 返回值

    查询成功：输出共`(<stationNum> + 1)`行。

    第一行为`<trainID> <type>`。

    接下来`<stationNum>`行，第`i`行为`<stations[i]> <ARRIVING_TIME> -> <LEAVING_TIME> <PRICE> <SEAT>`，其中`<ARRIVING_TIME>`和`<LEAVING_TIME>`为列车到达本站和离开本站的绝对时间，格式为`mm-dd hr:mi`。`<PRICE>`为从始发站乘坐至该站的累计票价，`<SEAT>`为从该站到下一站的剩余票数。对于始发站的到达时间和终点站的出发时间，所有数字均用`x`代替；终点站的剩余票数用`x`代替。如果车辆还未`release`则认为所有票都没有被卖出去。
    
    查询失败：`-1`
    
  - 举例

    （上接添加列车的例子）

    `>[669] query_train -d 07-01 -i HAPPY_TRAIN`
    
    `HAPPY_TRAIN G`
    
    `上院 xx-xx xx:xx -> 07-01 19:19 0 1000`
    
    `中院 07-02 05:19 -> 07-02 05:24 114 1000`
    
    `下院 07-02 15:24 -> xx-xx xx:xx 628 x`

##### [SF] `query_ticket`

  - 参数列表

    `-s -t -d (-p time)`

  - 说明

    查询日期为`-d`时从`-s`出发，并到达`-t`的车票。请注意：这里的日期是列车从`-s`出发的日期，不是从列车始发站出发的日期。
    
    `-p`的值为`time`和`cost`中的一个，若为`time`表示输出按照该车次所需时间从小到大排序，否则按照票价从低到高排序。如果按照时间排序车次所需时间相同，则把`<trainID>`作为第二关键字进行排序，按照票价排序；同理若出现车次票价相同，则同样把`<trainID>`作为第二关键字进行排序。

  - 返回值
  
    第一行输出一个整数，表示符合要求的车次数量。

    接下来每一行输出一个符合要求的车次，按要求排序。格式为`<trainID> <FROM> <LEAVING_TIME> -> <TO> <ARRIVING_TIME> <PRICE> <SEAT>`，其中出发时间、到达时间格式同`query_train`，`<FROM>`和`<TO>`为出发站和到达站，`<PRICE>`为累计价格，`<SEAT>`为最多能购买的票数。
  
- 样例
  
  （上接查询列车的例子）
  
  `>[670] query_ticket -s 中院 -t 下院 -d 08-17`
  
    `1`
  
    `HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000`
  
##### [N] `query_transfer`

  参数列表及其意义同`query_ticket`。

  - 说明

    在恰好换乘一次（换乘同一辆车不算恰好换乘一次）的情况下查询符合条件的车次，仅输出最优解。如果出现多个最优解（`-p time`总时间相同,`-p cost`总价格相同)，则选择在第一辆列车上花费的时间更少的方案。请注意：这里的日期是列车从`-s`出发的日期，不是从列车始发站出发的日期。

  - 返回值

  	查询失败（没有符合要求的车次）：`0`
  	
  	 查询成功：输出2行，换乘中搭乘的两个车次，格式同`query_ticket`。

##### [SF] `buy_ticket`

  - 参数列表
    `-u -i -d -n -f -t (-q false)`
    
  - 说明
    
    用户`-u`(`<username>`)购买：车次`-i`(`<trainID>`)，日期为`-d`，从站`-f`到站`-t`的车票`-n`张。
    
    `-q`可选`false`或`true`，若为`true`，表明在**余票不足**的情况下愿意接受候补购票，当有余票时**立即**视为此用户购买了车票，且保证购买的车票的数量大于0。请注意：这里的日期是列车从`-s`出发的日期，不是从列车始发站出发的日期。
    
    权限要求：`-u`已登录，且购买的车次必须已经被`release`。

  - 返回值
  
    购买成功：一个整数，表示订单总价。
    
    加入候补：`queue`
    
    购票失败：`-1`
    
  - 样例
  
    （上接查询车票的例子，假设用户均已登录）
  
    `>[671] buy_ticket -u Texas -i HAPPY_TRAIN -d 08-17 -n 800 -f 中院 -t 下院`
  
    `411200`
  
    `>[672] buy_ticket -u Lappland -i HAPPY_TRAIN -d 08-16 -n 500 -f 上院 -t 下院 -q true`
  
    `queue`
    
  - 样例解释
  
    用户`Texas`购买了8.17从中院出发的火车票800张，在上面的`add_train`操作中，8.16从上院出发的火车会在8.17到达中院，所以事实上`Lappland`试图购买的票的火车和`Texas`是同一辆车，所以`Lappland`购买不了500张票。
  
  - 候补队列解释：
    对于余票不足且需候补购票的订单，将加入候补队列。当有订单出现退单的时候，车次的剩余车票会增加，你需要按照候补队列的下单先后顺序满足候补队列中的订单。
    
    ```text
    假设当前车次有100个座位，为了简单假设只有两个站，且接下来的描述均允许候补购票。
    1 buy 50
    2 buy 30
    3 buy 60
    4 buy 10
    5 buy 20
    经过五次购票订单后订单1，2，4被满足，车次还剩下10个座位，候补队列内剩下订单3和订单5。
    6 refund (2 buy 30)
    当2号订单被退票，车次内还剩下40个座位，但是候补队列中订单3需要60个座位，因此无法满足。 
    继续向后寻找能够满足的订单5，因此订单5通过候补成功购票，订单5从候补队列中被删除，并且该车次只剩下了20个座位。
    虽然订单有先后顺序，但不代表早的订单一定先完成，只是拥有更高的优先级，且订单为最小单位，不能先把部分票卖给该订单，只能完全完成一个订单。
    ```
##### [F] `query_order`

  - 参数列表

    `-u`

  - 说明

    查询用户`-u`(`<username>`)的所有订单信息，按照交易时间顺序从新到旧排序（候补订单即使补票成功，交易时间也以下单时刻为准）。

    权限要求：`-u`已登录。

  - 返回值

    查询成功：第一行输出一个整数，表示订单数量。

    接下来每一行表示一个订单，格式为`[<STATUS>] <trainID> <FROM> <LEAVING_TIME> -> <TO> <ARRIVING_TIME> <PRICE> <NUM>`，其中`<NUM>`为购票数量，`<STATUS>`表示该订单的状态，可能的值为：`success`（购票已成功）、`pending`（位于候补购票队列中）和`refunded`（已经退票）。
    
    查询失败：`-1`
    
  - 样例

    `>[673] query_order -u Lappland`

    `1`

    `[pending] HAPPY_TRAIN 上院 08-17 05:24 -> 下院 08-17 15:24 628 500`

##### [N] `refund_ticket`

  - 参数列表

    `-u (-n 1)`

  - 说明

    用户`-u`(`<username>`）退订从新到旧（即`query_order`的返回顺序）第`-n`个(1-base)订单。

    权限要求：`-u`已登录。

  - 返回值

    退票成功：`0`

    退票失败：`-1`

##### [R] `rollback`

  - 参数列表

    `-t` 

  - 说明

    将所有用户下线，并将数据库回滚到时间戳为`-t`时刻的状态。
    
    仅在回滚测试点中出现。
    
  - 返回值

    回滚成功：`0`

    回滚失败：`-1`
  
  - 样例1：
  
    `>[19260817] rollback -t 19260816`
    
    `0`

  - 样例2：
  
    `>[19260817] rollback -t 20220222`
    
    `-1`
    
  - 样例3:
    
    输入：
    
    ```
    [5] add_user -c cur -u I_am_the_admin -p qwqq -n RainyMemory -m memo@rainymemory.net -g 10
    [10] login -u I_am_the_admin -p qwqq
    [15] query_profile -c I_am_the_admin -u I_am_the_admin
    [20] add_user -c I_am_the_admin -u Lappland -p texas -n 拉普兰德 -m lappy@siracusa.com -g 7
    [25] add_user -c I_am_the_admin -u Texas -p lappland -n 德克萨斯 -m texas@siracusa.com -g 7
    [30] query_profile -c I_am_the_admin -u Lappland
    [35] query_profile -c I_am_the_admin -u Texas
    [40] rollback -t 23
    [45] login -u I_am_the_admin -p qwqq
    [50] query_profile -c I_am_the_admin -u I_am_the_admin
    [55] query_profile -c I_am_the_admin -u Lappland
    [60] query_profile -c I_am_the_admin -u Texas
    [65] exit
    ```
    
    输出：
    
    ```
    0
    0
    I_am_the_admin RainyMemory memo@rainymemory.net 10
    0
    0
    Lappland 拉普兰德 lappy@siracusa.com 7
    Texas 德克萨斯 texas@siracusa.com 7
    0
    I_am_the_admin RainyMemory memo@rainymemory.net 10
    Lappland 拉普兰德 lappy@siracusa.com 7
    -1
    bye
    ```

##### [R] `clean`

- 参数列表

  无

- 说明

  清除所有数据。

  保证不会在`rollback`指令的测试中出现。

- 返回值

  `0`

##### [R] `exit`

- 参数列表

  无

- 说明

  退出程序，所有在线用户均下线。

- 返回值

  `bye`

##### 指令限制
* 保证所有指令输入格式均合法。
* 保证所有指令内出现的时间均为合法且在2021年范围内。
* 指令内要求排序的情况，保证指令内所有排序关键词不会相同。
* 样例中>后内容表示的是输入，不带>表示的是std的输出。

