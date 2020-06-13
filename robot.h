#ifndef ROBOT_H
#define ROBOT_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtDebug>

class Robot : public QGraphicsItem
{
public:
    Robot();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setRobotSpeed(int t_speed);
    void setRobotAngle(int t_angle);

    int getCurrentRobotSpeed() const;
    int getCurrentRobotAngle() const;

protected:
    void advance(int phase) override;

private:
    int CurrentRobotAngle; // Aktualny obrot robota

    qreal angle;
    qreal speed;
    void doCollision();
};

#endif // ROBOT_H
