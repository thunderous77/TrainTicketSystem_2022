#ifndef MAP_H
#define MAP_H

#include<Qpixmap>

class background
{
public:
    QPixmap map1,map2;//地图图片对象
    int map1_Y,map2_Y;//地图y轴坐标
    int scroll_speed=2;//地图滚动速度
    Map();//构造函数
    void update_Map_position();//地图滚动坐标计算
};

#endif // MAP_H
