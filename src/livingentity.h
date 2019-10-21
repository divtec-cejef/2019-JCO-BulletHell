/**
  \file
  \brief    Déclaration de la classe LivingEntity.
  \author   Thibaud Nussbaumer
  \date    octobre 2019
*/
#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "sprite.h"

class LivingEntity : public Sprite
{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT
public:
    LivingEntity(const QPixmap& rPixmap, QGraphicsItem* pParent = nullptr);
    virtual void tick(int elapsedTimeInMilliseconds);

private:
    QPointF m_livingEntityVelocity;
};

#endif // LIVINGENTITY_H
