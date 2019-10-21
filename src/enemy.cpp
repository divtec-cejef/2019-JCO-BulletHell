/**
  \file
  \brief    Déclaration de la classe enemy.
  \author   Thibaud Nussbaumer
  \date     octobre 2019
*/
#include "enemy.h"

#include "resources.h"

#include <QDebug>

const int ENEMY_VELOCITY = 400; // pixels par seconde

//! Construit et initialise un enemy.
//! \param pParent  Objet propiétaire de cet objet.
Enemy::Enemy(QGraphicsItem* pParent) : LivingEntity(GameFramework::imagesPath() + "earth.png", pParent) {
    m_enemyVelocity = QPointF(0,0);
    m_enemyCollidePlayer = true;

}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void Enemy::tick(int elapsedTimeInMilliseconds) {

    // Calcul de la distance parcourue par la balle, selon sa vitesse et le temps écoulé.
    QPointF enemyDistance = elapsedTimeInMilliseconds * m_enemyVelocity / 1000.;

    // Positionne la bounding box de la balle à sa prochaine position.
    QRectF nextRect = this->globalBoundingBox().translated(enemyDistance);

    // Si la prochaine position reste dans les limites de la scène, la balle
    // y est positionnée. Sinon, elle reste sur place.
    if (isInsideScene(nextRect)) {
        this->setPos(this->pos() + enemyDistance);
    }
}


//! Met à jour le vecteur de vitesse de la balle en fonction des touches appuyées.
void Enemy::updateEnemyVelocity()  {
    int XVelocity = 0;
    int YVelocity = 0;
    /*
    if (m_keyUpPressed)    YVelocity = -BALL_VELOCITY;
    if (m_keyDownPressed)  YVelocity = BALL_VELOCITY;
    if (m_keyRightPressed) XVelocity = BALL_VELOCITY;
    if (m_keyLeftPressed)  XVelocity = -BALL_VELOCITY;
    */
    m_enemyVelocity = QPoint(XVelocity, YVelocity);
}

