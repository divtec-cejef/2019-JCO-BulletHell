/**
  \file
  \brief    Définition de la classe ManualWalkingHandler.
  \author   JCO
  \date     septembre 2018
*/
#include "manualwalkinghandler.h"

#include <QGraphicsScale>

#include "gamescene.h"
#include "sprite.h"
#include "enemy.h"


const int DEFAULT_WALKING_SPEED = 100; // pixels par seconde

//! Constructeur.
//! \param pParentSprite Sprite dont le déplacement doit être géré.
ManualWalkingHandler::ManualWalkingHandler(Sprite* pParentSprite) : SpriteTickHandler (pParentSprite) {
    m_walkingSpeed = DEFAULT_WALKING_SPEED;
}

//! Initialise le déplacement.
void ManualWalkingHandler::init() {
    configureTransformationMatrix();
}

//! Cadence.
//! Déplace le sprite en fonction de sa vitesse et du temps écoulé.
//! Si après l'avoir déplacé, on constate une collision avec un autre sprite,
//! celui-ci change de direction afin de poursuivre son déplacement dans l'autre sens.
void ManualWalkingHandler::tick(int elapsedTime) {
    int walkingVelocity = m_walkingSpeed;
    if (m_walkingDirection == WALKING_LEFT)
        walkingVelocity *= -1;

    // Calcule la distance parcourue et positionne le sprite à sa prochaine position
    double distance = elapsedTime * walkingVelocity / 1000.;
    QPointF currentPos = m_pParentSprite->pos();
    currentPos.setX(currentPos.x() + distance);
    m_pParentSprite->setPos(currentPos);

    // Le sprite touche-t-il un mur ?
    if (/*!(static_cast<GameScene*>(m_pParentSprite->scene())->collidingSprites(m_pParentSprite).isEmpty()) ||*/
        !(m_pParentSprite->parentScene()->isInsideScene(m_pParentSprite->globalBoundingBox()))) {
            /*
            if (m_walkingDirection == WALKING_RIGHT)  {
                // Il se déplaçait à droite : on le fait tourner à gauche
                m_pParentSprite->setTransformations(m_transformsForFlip); // Flip horizontal
            } else {
                // Il se déplaçait à gauche : on le fait tourner à droite
                m_pParentSprite->setTransformations(QList<QGraphicsTransform*>());
            }
            */
            changeWalkingDirection();
    }
}


//! Change la vitesse de marche.
//! \param walkingSpeed     Nouvelle vitesse de déplacement.
void ManualWalkingHandler::setWalkingSpeed(int walkingSpeed) {
    m_walkingSpeed = walkingSpeed;
}

//! \return la vitesse de marche.
int ManualWalkingHandler::walkingSpeed() const {
    return m_walkingSpeed;
}

//! \return la direction de marche.
void ManualWalkingHandler::changeWalkingDirection() {
    setWalkingDirection(m_walkingDirection == WALKING_RIGHT ? WALKING_LEFT : WALKING_RIGHT);
}

//! Change la direction de marche.
//! \param newWalkingDirection  Nouvelle direction de marche.
void ManualWalkingHandler::setWalkingDirection(WalkingDirection newWalkingDirection) {
    if (m_walkingDirection != newWalkingDirection) {
        m_walkingDirection = newWalkingDirection;
    }
}

//! Construit une matrice de transformation en fonction du sprite afin de pouvoir
//! faire un miroir vertical de l'image lorsque le sprite change de direction.
void ManualWalkingHandler::configureTransformationMatrix() {
    // Préparation d'une matrice de transformation pour faire un miroir du marcheur
    QGraphicsScale* pHorizontalFlip = new QGraphicsScale(m_pParentSprite);
    pHorizontalFlip->setOrigin(QVector3D(m_pParentSprite->width()/2,0,0));
    pHorizontalFlip->setXScale(-1);
    m_transformsForFlip << pHorizontalFlip;
}

