/**
  \file
  \brief    Déclaration de la classe bullet.
  \author   Thibaud Nussbaumer
  \date     octobre 2019
*/
#include "bullet.h"

#include "resources.h"

#include <QDebug>

const int BULLET_VELOCITY = 600; // pixels par seconde

//! Construit et initialise une balle.
//! \param pParent  Objet propiétaire de cet objet.
Bullet::Bullet(QGraphicsItem* pParent) : Item(GameFramework::itemImagesPath() + "bullet.png", pParent)
{
    emitter = Sprite::Emitter::EM_NONE;
    spriteType = Sprite::SpriteType::ST_BULLET;
    m_bulletVelocity = QPointF(0,0);
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void Bullet::tick(int elapsedTimeInMilliseconds) {

    Sprite::tick(elapsedTimeInMilliseconds);

    // Calcul de la distance parcourue par la balle, selon sa vitesse et le temps écoulé.
    QPointF bulletDistance = elapsedTimeInMilliseconds * m_bulletVelocity / 1000.;

    // Positionne la bounding box de la balle à sa prochaine position.
    QRectF nextRect = this->globalBoundingBox().translated(bulletDistance);

    // Si la prochaine position reste dans les limites de la scène, la balle
    // y est positionnée. Sinon, elle reste sur place.
    if (isInsideScene(nextRect)) {
        this->setPos(this->pos() + bulletDistance);
    }else{
        delete this;
    }

}

//! Charge les différentes images qui composent l'animation de l'ennemi et
//! les ajoute à ce sprite.
void Bullet::configureAnimation() {
    /*
    for (int FrameNumber = 1; FrameNumber <= 2; ++FrameNumber)  {
        this->addAnimationFrame(QString(GameFramework::itemImagesPath() + "bullet.png").arg(FrameNumber));
    }
    this->setAnimationSpeed(100);  // Passe à la prochaine image de la marche toutes les 100 ms
    this->startAnimation();
    */
}

//! Met à jour le vecteur de vitesse de l'ennemi
void Bullet::updateVelocity(int x, int y)  {
    int XVelocity = x;
    int YVelocity = y;
    m_bulletVelocity = QPoint(XVelocity, YVelocity);
}

void Bullet::updateAnimationState(){
    if (this->isVisible()) {
        this->startAnimation();
    } else {
        this->stopAnimation();
    }
}
