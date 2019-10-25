/**
  \file
  \brief    Déclaration de la classe Player.
  \author   Thibaud Nussbaumer
  \date     octobre 2019
*/
#include "player.h"
#include "resources.h"

const int PLAYER_VELOCITY = 400; // pixels par seconde
const int FRAME_WIDTH = 63;
const int FRAME_HEIGHT = 56;
const int FRAME_COUNT = 5;
const int COLUMN_COUNT = 5;
const float SCALE_RATIO = 1.5F;

//! Construit et initialise une balle bleue.
//! \param pParent  Objet propiétaire de cet objet.
Player::Player(QGraphicsItem* pParent) : LivingEntity(pParent)/*LivingEntity(GameFramework::imagesPath() + "player.png", pParent)*/ {
    m_keyUpPressed    = false;
    m_keyDownPressed  = false;
    m_keyLeftPressed  = false;
    m_keyRightPressed = false;
    m_playerVelocity = QPointF(0,0);
    configureAnimation();
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void Player::tick(int elapsedTimeInMilliseconds) {

    // Calcul de la distance parcourue par la balle, selon sa vitesse et le temps écoulé.
    QPointF playerDistance = elapsedTimeInMilliseconds * m_playerVelocity / 1000.;

    // Positionne la bounding box de la balle à sa prochaine position.
    QRectF nextRect = this->globalBoundingBox().translated(playerDistance);

    // Si la prochaine position reste dans les limites de la scène, la balle
    // y est positionnée. Sinon, elle reste sur place.
    if (isInsideScene(nextRect)) {
        this->setPos(this->pos() + playerDistance);
    }
}

//! Découpe la spritesheet pour en extraire les étapes d'animation et
//! les ajouter au sprite.
void Player::configureAnimation() {

    // Chargement de la spritesheet
    QImage spriteSheet(GameFramework::imagesPath() + "player-flight.png");

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
void Player::updateAnimationState() {

    if (this->isVisible()) {
        this->startAnimation();
    } else {
        this->stopAnimation();
    }

}

//! Une touche a été appuyée.
//! \param key Code de la touche appuyée.
void Player::onKeyPressed(int key) {
    switch (key)  {
    case Qt::Key_W:    m_keyUpPressed    = true;  updatePlayerVelocity(); break;
    case Qt::Key_S:  m_keyDownPressed  = true;  updatePlayerVelocity(); break;
    case Qt::Key_D: m_keyRightPressed = true;  updatePlayerVelocity(); break;
    case Qt::Key_A:  m_keyLeftPressed  = true;  updatePlayerVelocity(); break;
    }
}

//! Une touche a été relâchée.
//! \param key Code de la touche relâchée.
void Player::onKeyReleased(int key) {
    switch (key)  {
    case Qt::Key_W:    m_keyUpPressed    = false;  updatePlayerVelocity(); break;
    case Qt::Key_S:  m_keyDownPressed  = false;  updatePlayerVelocity(); break;
    case Qt::Key_D: m_keyRightPressed = false;  updatePlayerVelocity(); break;
    case Qt::Key_A:  m_keyLeftPressed  = false;  updatePlayerVelocity(); break;
    }
}

//! Met à jour le vecteur de vitesse de la balle en fonction des touches appuyées.
void Player::updatePlayerVelocity()  {
    int XVelocity = 0;
    int YVelocity = 0;
    if (m_keyUpPressed)    YVelocity = -PLAYER_VELOCITY;
    if (m_keyDownPressed)  YVelocity = PLAYER_VELOCITY;
    if (m_keyRightPressed) XVelocity = PLAYER_VELOCITY;
    if (m_keyLeftPressed)  XVelocity = -PLAYER_VELOCITY;

    m_playerVelocity = QPoint(XVelocity, YVelocity);
}
