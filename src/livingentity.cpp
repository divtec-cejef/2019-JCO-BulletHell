/**
  \file
  \brief    Déclaration de la classe LivingEntity.
  \author   Thibaud Nussbaumer
  \date    octobre 2019
*/
#include "livingentity.h"
#include "resources.h"
#include <QDebug>

//! Construit et initialise une LivingEntity.
//! \param pParent  Objet propiétaire de cet objet.
LivingEntity::LivingEntity(const QPixmap& rPixmap, QGraphicsItem* pParent) : Sprite(rPixmap, pParent) {

}

LivingEntity::LivingEntity(QGraphicsItem* pParent) : Sprite(pParent){

}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void LivingEntity::tick(int elapsedTimeInMilliseconds){

    // Calcul de la distance parcourue par l'entité, selon sa vitesse et le temps écoulé.
    QPointF livingEntityDistance = elapsedTimeInMilliseconds * m_livingEntityVelocity / 1000.;

    // Positionne la bounding box de la balle à sa prochaine position.
    QRectF nextRect = this->globalBoundingBox().translated(livingEntityDistance);

    // Si la prochaine position reste dans les limites de la scène, la balle
    // y est positionnée. Sinon, elle reste sur place.
    if (isInsideScene(nextRect)) {
        this->setPos(this->pos() + livingEntityDistance);
    }
}

