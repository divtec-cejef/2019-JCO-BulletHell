#ifndef MANUALWALKINGHANDLER_H
#define MANUALWALKINGHANDLER_H
/**
  \file
  \brief    Déclaration de la classe ManualWalkingHandler.
  \author   JCO
  \date     septembre 2018
*/
#include "spritetickhandler.h"

#include <QGraphicsTransform>
#include <QList>

//! \brief Gestionnaire de déplacement manuel.
//!
//! Gestionnaire de déplacement qui se base sur le tick.
//!
//! Déplace le sprite de gauche à droite jusqu'à rencontrer un obstacle.
//! La direction est alors changée et la marche se poursuite.
//!
//! La vitesse de la marche est ajustable avec setWalkingSpeed().
//!
class ManualWalkingHandler : public SpriteTickHandler
{
public:
    enum WalkingDirection {
        WALKING_RIGHT,
        WALKING_LEFT
    };

    ManualWalkingHandler(Sprite* pParentSprite = nullptr);

    virtual void init();
    virtual void tick(int elapsedTimeInMilliseconds);

    void setWalkingSpeed(int walkingSpeed);
    int walkingSpeed() const;

    void changeWalkingDirection();
    void setWalkingDirection(WalkingDirection newWalkingDirection);

private:
    void configureTransformationMatrix();

    WalkingDirection m_walkingDirection;
    QList<QGraphicsTransform*> m_transformsForFlip;
    int m_walkingSpeed;
};

#endif // MANUALWALKINGHANDLER_H
