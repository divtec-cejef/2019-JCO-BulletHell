/**
  \file
  \brief    Déclaration de la classe enemy.
  \author   Thibaud Nussbaumer
  \date     octobre 2019
*/
#include "enemy.h"

#include "resources.h"

#include <QDebug>
#include <ctime>
#include "bullet.h"
#include "gamecore.h"
#include "gamescene.h"
#include "sprite.h"

const int ENEMY_VELOCITY = 500; // pixels par seconde


//! Construit et initialise un ennemi.
//! \param pParent  Objet propiétaire de cet objet.
Enemy::Enemy(QGraphicsItem* pParent) : LivingEntity(GameFramework::enemyImagesPath() + "Eye_of_Chaos.png", pParent) {
    m_enemyVelocity = QPointF(0,0);
    spriteType = Sprite::SpriteType_e::ST_ENEMY;

    health = 0;
    // Initialise le générateur aléatoire pour la cadence de tir des ennemis
    //std::srand(std::time(0));
    //Initialisation des variables pour le tir automatique
    timeBeforeShoot = 0;
    qDebug() << "timeBeforeShoot" << timeBeforeShoot;
    counterMillisecondsEnemy = 0;
    //configureAnimation();
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void Enemy::tick(int elapsedTimeInMilliseconds) {

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
    if(this->counterMillisecondsEnemy == this->timeBeforeShoot){
        shoot();
        this->counterMillisecondsEnemy = 0;
    }
    this->counterMillisecondsEnemy++;

    // Vérification du contact de l'ennemi avec une bullet émise par le joueur
    // Si l'ennemi est touchée par une de ces balles, il meurt
    if(!this->collidingSprites().isEmpty()){
        //On enlève l'ennemi de sa liste de collision
        this->collidingSprites().removeAll(this);
        if(!this->collidingSprites().isEmpty()){
            if(this->collidingSprites().first()->getType() == Sprite::SpriteType_e::ST_BULLET
                && this->collidingSprites().first()->getEmitter() == Sprite::Emitter_e::EM_PLAYER){
                //Envoit le signal de sa mort
                emit enemyDeath(this);
            }
        }
    }

}

//! Charge les différentes images qui composent l'animation de l'ennemi et
//! les ajoute à ce sprite.
void Enemy::configureAnimation() {
    /*
    for (int FrameNumber = 1; FrameNumber <= 2; ++FrameNumber)  {
        this->addAnimationFrame(QString(GameFramework::enemyImagesPath() + "Eye_Enemy%1.png").arg(FrameNumber));
    }
    this->setAnimationSpeed(100);  // Passe à la prochaine image de la marche toutes les 100 ms
    this->startAnimation();
    */
}

//! Met à jour le vecteur de vitesse de l'ennemi en fonction des touches appuyées.
void Enemy::updateVelocity()  {
    int XVelocity = 0;
    int YVelocity = 0;
    m_enemyVelocity = QPoint(XVelocity, YVelocity);
}

//! Créer un objet de type Bullet et l'envoi dans une direction
void Enemy::shoot(){
    Bullet* pBullet = new Bullet;
    pBullet->setEmitter(Sprite::Emitter_e::EM_ENEMY);
    pBullet->setPos(QPointF(this->x()+(this->width()/2),this->bottom()));
    pBullet->setZValue(1);          // Passe devant tous les autres sprites
    this->parentScene()->addSpriteToScene(pBullet);
    pBullet->updateVelocity(0,800);
    //pBullet = nullptr;
}

void Enemy::setTimeBeforeShoot(int timeBeforeShoot){
    this->timeBeforeShoot = timeBeforeShoot;
}
