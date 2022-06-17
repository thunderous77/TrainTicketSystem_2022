#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include<qtimer.h>
#include<config.h>
#include<background.h>
#include<my_plane.h>
#include<QMouseEvent>

class Mainscene : public QWidget
{
    Q_OBJECT

public:
    Mainscene(QWidget *parent = nullptr);
    ~Mainscene();
private:
    QTimer m_timer;//定时器对象
    background m_background;
    my_plane Myplane;
    void StartPlay();
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};
#endif // MAINSCENE_H
