#include "mainscene.h"
#include<QIcon>//添加图标需要的头文件
#include<qpainter.h>
#include<iostream>

Mainscene::Mainscene(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(Background_Width,Background_Height);//初始化窗口大小
    setWindowTitle("飞机大战");//设置标题
    setWindowIcon(QIcon(":/resource/ico.ico"));//设置图标,":"表示虚拟路径
    m_timer.setInterval(10);//设置定时时间每隔10ms
    StartPlay();
}

Mainscene::~Mainscene()
{
}

void Mainscene::StartPlay(){
    m_timer.start();//启动定时器
    //监听定时器的信号,一种写法为connect(槽(操作对象),信号,lambda表达式)
    connect(&m_timer,&QTimer::timeout,[=](){//此处为每隔一段时间执行lambda表达式
        m_background.update_Map_position();//更新坐标
        update();//Qwidget类自带函数，可以更新一些数据，包括调用paintEvent()，mouseMoveEvent()
        //update()和Qtimer一起使用可以做到实时更新数据
    });
}
void Mainscene::paintEvent(QPaintEvent *event){//Qwidget类中的虚函数，一般负责输出到屏幕
    QPainter painter(this);//重建一个画家，把它和这个Qwidget窗口联系起来，通过它画出图片
    painter.drawPixmap(0,m_map.map1_Y,m_map.map1);//传入x,y坐标和Qpixmap(导入的图片),将其显示出来
    painter.drawPixmap(0,m_map.map2_Y,m_map.map2);
//    std::cout<<m_map.map1_Y<<" "<<m_map.map2_Y<<'\n';
    painter.drawPixmap(Myplane.x,Myplane.y,Myplane.picture);
}
void Mainscene::mouseMoveEvent(QMouseEvent *event){//Qwidget类中的虚函数，用它监测当前鼠标点击位置
    int x=event->x()-Myplane.picture.width()*0.5;
    int y=event->y()-Myplane.picture.height()*0.5;
    std::cout<<x<<" "<<y<<"\n";
    //此处设计的是调飞机左上角位置，所以可以考虑在上面将x,y减去飞机宽度/高度的一半，使得鼠标正好图片中间
    Myplane.update_plane_Position(x,y);
}
