/**
  \file
  \brief    Déclaration de la classe WalkingMan.
  \author   JCO
  \date     septembre 2018
*/
#include "walkingman.h"

#include "resources.h"
const int WALKINGMAN_VELOCITY = 400; // pixels par seconde
//! Construit un marcheur et l'initialise.
//! \param pParent  Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
WalkingMan::WalkingMan(QGraphicsItem* pParent) : Sprite(pParent) {
    m_keyUpPressed    = false;
    m_keyDownPressed  = false;
    m_keyLeftPressed  = false;
    m_keyRightPressed = false;
    m_WalkingManVelocity = QPointF(0,0);
    configureAnimation();
}

/*
 * //! Construit et initialise une balle bleue.
//! \param pParent  Objet propiétaire de cet objet.
BlueBall::BlueBall(QGraphicsItem* pParent) : Sprite(GameFramework::imagesPath() + "ball.png", pParent) {
    m_keyUpPressed    = false;
    m_keyDownPressed  = false;
    m_keyLeftPressed  = false;
    m_keyRightPressed = false;
    m_ballVelocity = QPointF(0,0);

}
*/
//! Charge les différentes images qui composent l'animation du marcheur et
//! les ajoute à ce sprite.
void WalkingMan::configureAnimation() {
    for (int FrameNumber = 1; FrameNumber <= 8; ++FrameNumber)  {
        this->addAnimationFrame(QString(GameFramework::imagesPath() + "marche%1.png").arg(FrameNumber));
    }
    this->setAnimationSpeed(100);  // Passe à la prochaine image de la marche toutes les 100 ms
    this->startAnimation();
}



//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void WalkingMan::tick(int elapsedTimeInMilliseconds) {

    // Calcul de la distance parcourue par la balle, selon sa vitesse et le temps écoulé.
    QPointF WalkingManDistance = elapsedTimeInMilliseconds * m_WalkingManVelocity / 1000.;

    // Positionne la bounding box de la balle à sa prochaine position.
    QRectF nextRect = this->globalBoundingBox().translated(WalkingManDistance);

    // Si la prochaine position reste dans les limites de la scène, la balle
    // y est positionnée. Sinon, elle reste sur place.
    if (isInsideScene(nextRect)) {
        this->setPos(this->pos() + WalkingManDistance);
    }
}

//! Une touche a été appuyée.
//! \param key Code de la touche appuyée.
void WalkingMan::onKeyPressed(int key) {
    switch (key)  {
    case Qt::Key_W:    m_keyUpPressed    = true;  updateWalkingManVelocity(); break;
    case Qt::Key_S:  m_keyDownPressed  = true;  updateWalkingManVelocity(); break;
    case Qt::Key_D: m_keyRightPressed = true;  updateWalkingManVelocity(); break;
    case Qt::Key_A:  m_keyLeftPressed  = true;  updateWalkingManVelocity(); break;
    }
}

//! Une touche a été relâchée.
//! \param key Code de la touche relâchée.
void WalkingMan::onKeyReleased(int key) {
    switch (key)  {
    case Qt::Key_W:    m_keyUpPressed    = false;  updateWalkingManVelocity(); break;
    case Qt::Key_S:  m_keyDownPressed  = false;  updateWalkingManVelocity(); break;
    case Qt::Key_D: m_keyRightPressed = false;  updateWalkingManVelocity(); break;
    case Qt::Key_A:  m_keyLeftPressed  = false;  updateWalkingManVelocity(); break;
    }

}

//! Met à jour le vecteur de vitesse de la balle en fonction des touches appuyées.
void WalkingMan::updateWalkingManVelocity()  {
    int XVelocity = 0;
    int YVelocity = 0;
    if (m_keyUpPressed)    YVelocity = -WALKINGMAN_VELOCITY; configureAnimation();
    if (m_keyDownPressed)  YVelocity = WALKINGMAN_VELOCITY; configureAnimation();
    if (m_keyRightPressed) XVelocity = WALKINGMAN_VELOCITY; configureAnimation();
    if (m_keyLeftPressed)  XVelocity = -WALKINGMAN_VELOCITY; configureAnimation();

    m_WalkingManVelocity = QPoint(XVelocity, YVelocity);
}

