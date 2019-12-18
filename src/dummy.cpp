/**
  \file
  \brief    Déclaration de la classe enemy.
  \author   Thibaud Nussbaumer
  \date     décembre 2019
*/

#include "dummy.h"
#include "resources.h"
#include <QDebug>
#include <ctime>
#include "bullet.h"
#include "gamecore.h"
#include "gamescene.h"
#include "sprite.h"
#include "player.h"

const int FRAME_WIDTH = 79;
const int FRAME_HEIGHT = 121;
const int FRAME_COUNT = 16;
const int COLUMN_COUNT = 4;
const float SCALE_RATIO = 0.5F;

//! Construit et initialise un Dummy.
//! \param pParent
Dummy::Dummy(QGraphicsItem* pParent) : Enemy(pParent){
    m_enemyVelocity = QPointF(0,0);
    m_spriteType = Sprite::SpriteType_e::ST_ENEMY;
    //Initialisation des variables pour le tir automatique
    m_timeBeforeShoot = 0;
    qDebug() << "m_timeBeforeShoot" << m_timeBeforeShoot;
    m_counterMillisecondsEnemy = 0;
    configureAnimation();
}


//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void Dummy::tick(int elapsedTimeInMilliseconds) {

    Sprite::tick(elapsedTimeInMilliseconds);

    // Calcul de la distance parcourue par la balle, selon sa vitesse et le temps écoulé.
    QPointF enemyDistance = elapsedTimeInMilliseconds * m_enemyVelocity / 1000.;

    // Positionne la bounding box de la balle à sa prochaine position.
    QRectF nextRect = this->globalBoundingBox().translated(enemyDistance);

    // Si la prochaine position reste dans les limites de la scène, l'ennemi
    // y est positionnée. Sinon, elle reste sur place.
    if (isInsideScene(nextRect)) {
        this->setPos(this->pos() + enemyDistance);
    }else{
        this->setPos(this->pos() - enemyDistance);
    }

    //qDebug() << "elapsedTimeInMilliseconds" << elapsedTimeInMilliseconds;
    //qDebug() << "compteur milliseconds" << counterMilliseconds;
    if(this->m_counterMillisecondsEnemy == this->m_timeBeforeShoot){
        shoot();
        this->m_counterMillisecondsEnemy = 0;
    }
    this->m_counterMillisecondsEnemy++;

    // Vérification du contact de l'ennemi avec une bullet émise par le joueur
    // Si l'ennemi est touchée par une de ces balles, il meurt
    if(!this->collidingSprites().isEmpty()){
        //On enlève l'ennemi de sa liste de collision
        this->collidingSprites().removeAll(this);
        if(!this->collidingSprites().isEmpty()){
            if(this->collidingSprites().first()->getType() == Sprite::SpriteType_e::ST_BULLET
                && this->collidingSprites().first()->getEmitter() == Sprite::Emitter_e::EM_PLAYER){
                this->collidingSprites().first()->deleteLater();
                //Envoit le signal de sa mort
                emit notifyEnemyDeath(this);
            }
        }
    }

}

//! Charge les différentes images qui composent l'animation de l'ennemi et
//! les ajoute à ce sprite.
void Dummy::configureAnimation() {
    // Chargement de la spritesheet
    QImage spriteSheet(GameFramework::enemyImagesPath() + "Dummy_resized.png");

    // Découpage de la spritesheet
    for (int frameIndex = 0; frameIndex < FRAME_COUNT; frameIndex++) {
        QImage sprite = spriteSheet.copy((frameIndex % COLUMN_COUNT) * FRAME_WIDTH,
                                         (frameIndex / COLUMN_COUNT) * FRAME_HEIGHT,
                                         FRAME_WIDTH, FRAME_HEIGHT);

        this->addAnimationFrame(QPixmap::fromImage(sprite.scaled(FRAME_WIDTH * SCALE_RATIO,
                                                                 FRAME_HEIGHT * SCALE_RATIO,
                                                                 Qt::IgnoreAspectRatio,
                                                                 Qt::SmoothTransformation)));
    }

    this->setAnimationSpeed(100);
    updateAnimationState();
}

//! Active ou désactive l'animation selon si la roue est visible ou non.
void Dummy::updateAnimationState() {

    if (this->isVisible()) {
        this->startAnimation();
    } else {
        this->stopAnimation();
    }

}

//! Met à jour le vecteur de vitesse de l'ennemi en fonction des touches appuyées.
void Dummy::updateVelocity()  {
    int XVelocity = 0;
    int YVelocity = 0;
    m_enemyVelocity = QPoint(XVelocity, YVelocity);
}

//! Créer un objet de type Bullet et l'envoi dans une direction
void Dummy::shoot(){
    Bullet* pBullet = new Bullet;
    pBullet->setEmitter(Sprite::Emitter_e::EM_ENEMY);
    pBullet->setPos(QPointF(this->x()+(this->width()/2),this->bottom()));
    pBullet->setZValue(1);
    this->parentScene()->addSpriteToScene(pBullet);
    pBullet->updateVelocity(0,800);
}

void Dummy::setTimeBeforeShoot(int timeBeforeShoot){
    this->m_timeBeforeShoot = timeBeforeShoot;
}
