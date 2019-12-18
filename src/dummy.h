/**
  \file
  \brief    Déclaration de la classe enemy.
  \author   Thibaud Nussbaumer
  \date     décembre 2019
*/
#ifndef DUMMY_H
#define DUMMY_H

#include "enemy.h"
#include "sprite.h"
#include "livingentity.h"
#include <QObject>

class Dummy : public Enemy{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT

public:
    Dummy(QGraphicsItem* pParent = nullptr);
    void tick(int elapsedTimeInMilliseconds);
    void setTimeBeforeShoot(int timeBeforeShoot);

private:
    QPointF m_EOCVelocity;
    void configureAnimation();
    void updateVelocity();
    void shoot();
    void updateAnimationState();
};

#endif // DUMMY_H
