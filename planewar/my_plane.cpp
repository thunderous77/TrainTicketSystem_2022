#include<config.h>
#include "my_plane.h"
#include<iostream>
my_plane::my_plane()
{
    picture.load(":/resource/self.png");
    x=Background_Width*0.5-picture.width()*0.5;//左上角为(0,0)
    y=Background_Height-20-picture.height();
    //width(),weight()为图片宽度，所以要注意导入的图片边缘不要留白太多
//    std::cout<<picture.width()<<" "<<picture.height()<<"\n";

    //Rect矩形用于判断是否碰撞
    Rect.setWidth(picture.width());
    Rect.setHeight(picture.height());
    Rect.moveTo(x,y);
}
void my_plane::shoot(){
}
void my_plane::update_plane_Position(int _x,int _y){
    //边界检测
    if(_x<0)_x=0;
    if(_y<0)_y=0;
    if(_x+picture.width()>Background_Width)_x=Background_Width-picture.width();
    if(_y+picture.height()>Background_Height)_y=Background_Height-picture.height();

    x=_x,y=_y;
    Rect.moveTo(x,y);
}
