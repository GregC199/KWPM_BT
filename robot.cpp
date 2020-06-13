#include "robot.h"

Robot::Robot(){
    // Set init roatation
    CurrentRobotAngle = 0;
    setRobotAngle(CurrentRobotAngle);

    // Set init speed
    speed = 0;

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
    // Robot location and size
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
// Ustawienie predkosci i orientacji robota
void Robot::setRobotSpeed(int t_speed)
{
    this->speed = t_speed;
}

void Robot::setRobotAngle(int t_angle)
{
    this->CurrentRobotAngle = t_angle; // Wpisanie do aktualnego obrotu, nowego obrotu z argumentu funkcji
    setRotation(CurrentRobotAngle);
}



// Odczytanie aktualnych wartosci polozenia/przemieszczenia robota
int Robot::getCurrentRobotSpeed() const
{
    return this->speed;
}

int Robot::getCurrentRobotAngle() const
{
    return this->CurrentRobotAngle;
}

qreal Robot::getCurrentXPos() const
{
    return this->y();
}

qreal Robot::getCurrentYPos() const
{
    return this->x();
}



////////////////////////////////
// Wykrycie i obsługa kolizji //
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
    // Zatryczmanie robota na przeszkodzie
    this->setRobotSpeed(0);
;}


