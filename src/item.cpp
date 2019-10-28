/**
  \file
  \brief    Déclaration de la classe LivingEntity.
  \author   Thibaud Nussbaumer
  \date     octobre 2019
*/
#include "item.h"
#include "resources.h"
#include <QDebug>

//! Construit et initialise un item.
//! \param pParent  Objet propiétaire de cet objet.
Item::Item(const QPixmap& rPixmap, QGraphicsItem* pParent) : Sprite(rPixmap, pParent){

}

Item::Item(QGraphicsItem* pParent) : Sprite(pParent){

}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void Item::tick(int elapsedTimeInMilliseconds){

    // Calcul de la distance parcourue par l'entité, selon sa vitesse et le temps écoulé.
    QPointF itemEntityDistance = elapsedTimeInMilliseconds * m_itemVelocity / 1000.;

    // Positionne la bounding box de la balle à sa prochaine position.
    QRectF nextRect = this->globalBoundingBox().translated(itemEntityDistance);

    // Si la prochaine position reste dans les limites de la scène, la balle
    // y est positionnée. Sinon, elle reste sur place.
    if (isInsideScene(nextRect)) {
        this->setPos(this->pos() + itemEntityDistance);
    }
}
