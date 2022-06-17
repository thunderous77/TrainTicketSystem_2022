#ifndef MY_PLANE_H
#define MY_PLANE_H
#include<Qpixmap>

class my_plane
{
public:
    QPixmap picture;
    int x,y;
    QRect Rect;//矩形
    my_plane();
    void shoot();
    void update_plane_Position(int x,int y);
};

#endif // MY_PLANE_H
