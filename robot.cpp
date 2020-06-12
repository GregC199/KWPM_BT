// myitem.cpp

#include "robot.h"

Robot::Robot(){

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Robot::boundingRect
/// \return
///
QRectF Robot::boundingRect() const
{
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

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Robot::doCollision
///
void Robot::doCollision()
{

}


