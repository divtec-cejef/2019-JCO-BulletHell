/**
  \file
  \brief    Déclaration de la classe enemy.
  \author   Thibaud Nussbaumer
  \date     décembre 2019
*/
#ifndef EYEOFCHAOS_H
#define EYEOFCHAOS_H

#include "enemy.h"
#include "sprite.h"
#include "livingentity.h"

class EyeOfChaos : public Enemy
{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT

public:
    EyeOfChaos(QGraphicsItem* pParent = nullptr);
    void tick(int elapsedTimeInMilliseconds);
    void setTimeBeforeShoot(int timeBeforeShoot);

private:
    QPointF m_EOCVelocity;
    void configureAnimation();
    void updateVelocity();
    void shoot();
};

#endif // EYEOFCHAOS_H
