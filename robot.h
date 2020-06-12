#ifndef ROBOT_H
#define ROBOT_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class Robot : public QGraphicsItem
{
public:
    Robot();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void robotControl(int t_angle, int t_speed);

protected:
    void advance(int phase) override;

private:
    qreal angle;
    qreal speed;
    void doCollision();
};

#endif // ROBOT_H
