#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtDebug>

class Obstacle : public QGraphicsItem
{
public:
    Obstacle()
    {
        int Xpos = 50;
        int Ypos = 50;

        setPos(mapToParent(Xpos, Ypos));
    }

    Obstacle(int t_xPos, int t_yPos)
    {
        int Xpos = t_xPos;
        int Ypos = t_yPos;

        setPos(mapToParent(Xpos, Ypos));
    }


    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    // void advance(int phase) override;

};

#endif // OBSTACLE_H
