/**
  \file
  \brief    Déclaration de la classe BlueBall.
  \author   JCO
  \date     septembre 2018
*/
#include "blueball.h"

#include "resources.h"

#include <QDebug>

const int BALL_VELOCITY = 400; // pixels par seconde

//! Construit et initialise une balle bleue.
//! \param pParent  Objet propiétaire de cet objet.
BlueBall::BlueBall(QGraphicsItem* pParent) : Sprite(GameFramework::imagesPath() + "ball.png", pParent) {
    m_keyUpPressed    = false;
    m_keyDownPressed  = false;
    m_keyLeftPressed  = false;
    m_keyRightPressed = false;
    m_ballVelocity = QPointF(0,0);

}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void BlueBall::tick(int elapsedTimeInMilliseconds) {

    // Calcul de la distance parcourue par la balle, selon sa vitesse et le temps écoulé.
    QPointF ballDistance = elapsedTimeInMilliseconds * m_ballVelocity / 1000.;

    // Positionne la bounding box de la balle à sa prochaine position.
    QRectF nextRect = this->globalBoundingBox().translated(ballDistance);

    // Si la prochaine position reste dans les limites de la scène, la balle
    // y est positionnée. Sinon, elle reste sur place.
    if (isInsideScene(nextRect)) {
        this->setPos(this->pos() + ballDistance);
    }
}

//! Une touche a été appuyée.
//! \param key Code de la touche appuyée.
void BlueBall::onKeyPressed(int key) {
    switch (key)  {
    case Qt::Key_W:    m_keyUpPressed    = true;  updateBallVelocity(); break;
    case Qt::Key_S:  m_keyDownPressed  = true;  updateBallVelocity(); break;
    case Qt::Key_D: m_keyRightPressed = true;  updateBallVelocity(); break;
    case Qt::Key_A:  m_keyLeftPressed  = true;  updateBallVelocity(); break;
    }
}

//! Une touche a été relâchée.
//! \param key Code de la touche relâchée.
void BlueBall::onKeyReleased(int key) {
    switch (key)  {
    case Qt::Key_W:    m_keyUpPressed    = false;  updateBallVelocity(); break;
    case Qt::Key_S:  m_keyDownPressed  = false;  updateBallVelocity(); break;
    case Qt::Key_D: m_keyRightPressed = false;  updateBallVelocity(); break;
    case Qt::Key_A:  m_keyLeftPressed  = false;  updateBallVelocity(); break;
    }

}

//! Met à jour le vecteur de vitesse de la balle en fonction des touches appuyées.
void BlueBall::updateBallVelocity()  {
    int XVelocity = 0;
    int YVelocity = 0;
    if (m_keyUpPressed)    YVelocity = -BALL_VELOCITY;
    if (m_keyDownPressed)  YVelocity = BALL_VELOCITY;
    if (m_keyRightPressed) XVelocity = BALL_VELOCITY;
    if (m_keyLeftPressed)  XVelocity = -BALL_VELOCITY;

    m_ballVelocity = QPoint(XVelocity, YVelocity);
}

