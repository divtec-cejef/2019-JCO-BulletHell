/**
  \file
  \brief    Déclaration de la classe Player.
  \author   Thibaud Nussbaumer
  \date     octobre 2019
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "livingentity.h"

class Player : public LivingEntity
{
public:
    Player(QGraphicsItem* pParent = nullptr);
    virtual void tick(int elapsedTimeInMilliseconds);

public slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);

private:
    void updatePlayerVelocity();
    virtual void configureAnimation();
    void updateAnimationState();

    QPointF m_playerVelocity;
    bool m_keyUpPressed;
    bool m_keyDownPressed;
    bool m_keyLeftPressed;
    bool m_keyRightPressed;
};

#endif // PLAYER_H
