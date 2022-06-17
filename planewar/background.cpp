#include<config.h>
#include "background.h"
Map::Map()
{
    map1.load(":/resource/background.webp");//导入图片，返回一个bool类型表示成功/失败
    map2.load(":/resource/background.webp");
    map1_Y=-Background_Height;
    map2_Y=0;
    scroll_speed=2;
}

void Map::update_Map_position()
{
    map1_Y+=scroll_speed;
    map2_Y+=scroll_speed;
    if(map1_Y>=0)map1_Y=-Background_Height,map2_Y=0;

}
