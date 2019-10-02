/**
  \file
  \brief    Définition de la classe BouncingSpriteHandler.
  \author   JCO
  \date     septembre 2018
*/
#include "bouncingspritehandler.h"

#include "sprite.h"
#include "gamescene.h"

#include <QDebug>
#include <cmath>

const int TENNISBALL_INITIAL_VELOCITY = 200;

//! Constructeur.
//! \param pParentSprite Sprite dont le déplacement doit être géré.
BouncingSpriteHandler::BouncingSpriteHandler(Sprite* pParentSprite) : SpriteTickHandler (pParentSprite) {
    m_ballVelocity = QPointF(TENNISBALL_INITIAL_VELOCITY ,TENNISBALL_INITIAL_VELOCITY);
}

//! Cadence : détermine le mouvement que fait la balle durant le temps écoulé,
//! vérifie si elle doit rebondir et la positionne à son nouvel emplacement.
void BouncingSpriteHandler::tick(int elapsedTimeInMilliseconds) {
    QPointF tennisBallMovement = m_ballVelocity * elapsedTimeInMilliseconds / 1000.;

    // Détermine la prochaine position de la balle
    QRectF nextTennisBallRect = m_pParentSprite->globalBoundingBox().translated(tennisBallMovement);

    // Récupère tous les sprites de la scène que toucherait la balle à sa prochaine position
    QList<Sprite*> collidingSprites = m_pParentSprite->parentScene()->collidingSprites(nextTennisBallRect);

    // Supprimer le sprite de la balle lui-même, qui collisionne toujours avec sa boundingbox
    collidingSprites.removeAll(m_pParentSprite);

    if (!collidingSprites.isEmpty())  {
        // On ne considère que la première collision (au cas où il y en aurait plusieurs)
        Sprite* pCollidingSprite = collidingSprites[0];

        // Technique très approximative pour simuler un rebond en simplifiant
        // la façon de déterminer le vecteur normal de la surface du rebond.
        float overlapLeft = m_pParentSprite->right() - pCollidingSprite->left();
        float overlapRight = pCollidingSprite->right() - m_pParentSprite->left();
        float overlapTop = m_pParentSprite->bottom() - pCollidingSprite->top();
        float overlapBottom = pCollidingSprite->bottom() - m_pParentSprite->top();

        bool ballFromLeft(std::abs(overlapLeft) < std::abs(overlapRight));
        bool ballFromTop(std::abs(overlapTop) < std::abs(overlapBottom));

        float minOverlapX = ballFromLeft ? overlapLeft : overlapRight;
        float minOverlapY = ballFromTop ? overlapTop : overlapBottom;

        if(std::abs(minOverlapX) < std::abs(minOverlapY))
            m_ballVelocity.setX(ballFromLeft ? -TENNISBALL_INITIAL_VELOCITY :TENNISBALL_INITIAL_VELOCITY);
        else
            m_ballVelocity.setY(ballFromTop ? -TENNISBALL_INITIAL_VELOCITY : TENNISBALL_INITIAL_VELOCITY);

        tennisBallMovement = m_ballVelocity * elapsedTimeInMilliseconds / 1000.;
    }

    m_pParentSprite->setPos(m_pParentSprite->pos() + tennisBallMovement);
}


