#include "obstacle.h"

QRectF Obstacle::boundingRect() const
{
    // Obstacle size
    return QRect(0, 0, 20, 20);
}


void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush Brush(Qt::gray);

    Brush.setColor(Qt::yellow);

    /*
    // Collision detection
    if(scene()->collidingItems(this).isEmpty()){
        // no collision => green
        Brush.setColor(Qt::green);
    }
    else{
        // collision => red
        Brush.setColor(Qt::red);

        // Set position
        // doCollision();
    }
    */

    painter->fillRect(rec, Brush);

    // Draw Robot
    painter->drawRect(rec);
}
