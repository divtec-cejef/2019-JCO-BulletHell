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
    LivingEntity(QGraphicsItem* pParent = nullptr);
    virtual void tick(int elapsedTimeInMilliseconds);
    virtual void death() = 0;
protected:
    virtual void configureAnimation() = 0;
    virtual void updateVelocity() = 0;
    virtual void shoot() = 0;

private:
    QPointF m_livingEntityVelocity;
};

#endif // LIVINGENTITY_H
