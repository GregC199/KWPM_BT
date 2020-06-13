#include "obstacle.h"

QRectF Obstacle::boundingRect() const
{
    // Obstacle size
    return QRect(0, 0, 40, 40);
}


void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush Brush(Qt::gray);

    Brush.setColor(Qt::yellow);

    painter->fillRect(rec, Brush);
    painter->drawRect(rec);
}
