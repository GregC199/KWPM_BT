// myitem.cpp

#include "robot.h"

Robot::Robot(){
    // Set init roatation
    angle = 0;
    setRotation(angle);

    // Set init speed
    speed = 5;

    // Set init position
    int startX = 0;
    int startY = 0;

    setPos(mapToParent(startX, startY));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Robot::boundingRect
/// \return
///
QRectF Robot::boundingRect() const
{
    // Robot location size
    return QRect(0, 0, 20, 20);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Robot::paint
/// \param painter
/// \param option
/// \param widget
///
void Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush Brush(Qt::gray);


    // Collision detection
    if(scene()->collidingItems(this).isEmpty()){
        // no collision => green
        Brush.setColor(Qt::green);
    }
    else{
        // collision => red
        Brush.setColor(Qt::red);

        // Set position
        doCollision();
    }

    painter->fillRect(rec, Brush);

    // Draw Robot
    painter->drawRect(rec);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Robot::advance
/// \param phase
///
void Robot::advance(int phase)
{
    if(!phase) return;

    QPointF location = this->pos();

    setPos(mapToParent(0, -(speed)));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Robot::doCollision
///
void Robot::doCollision()
{
    // Set position

    // Set new angle


}


