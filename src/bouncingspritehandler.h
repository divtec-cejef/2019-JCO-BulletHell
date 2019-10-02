/**
  \file
  \brief    Déclaration de la classe BouncingSpriteHandler.
  \author   JCO
  \date     septembre 2018
*/
#ifndef BOUNCINGSPRITEHANDLER_H
#define BOUNCINGSPRITEHANDLER_H

#include <QPointF>

#include "spritetickhandler.h"

class BouncingSpriteHandler : public SpriteTickHandler
{
public:
    BouncingSpriteHandler(Sprite* pParentSprite = nullptr);

    void tick(int elapsedTimeInMilliseconds);

private:
    QPointF m_ballVelocity;

};

#endif // BOUNCINGSPRITEHANDLER_H
