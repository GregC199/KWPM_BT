// myitem.cpp

#include "robot.h"

Robot::Robot()
{

}

QRectF Robot::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,100,100);
}

void Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QPen pen(Qt::black, 4);
    painter->setPen(pen);
    painter->drawRect(rect);
}

