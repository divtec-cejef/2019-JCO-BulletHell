/**
  \file
  \brief    Déclaration de la classe Player.
  \author   Thibaud Nussbaumer
  \date     octobre 2019
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "livingentity.h"
#include "sprite.h"
#include <QObject>

class Player : public LivingEntity
{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT
public:
    Player(QGraphicsItem* pParent = nullptr);
    virtual void tick(int elapsedTimeInMilliseconds);
signals:
    void playerDeath(bool playerDead);
public slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);
private:
    void updatePlayerVelocity();
    virtual void configureAnimation();
    virtual void updateVelocity();
    virtual void shoot();


    void updateAnimationState();

    QPointF m_playerVelocity;
    bool m_keyUpPressed;
    bool m_keyDownPressed;
    bool m_keyLeftPressed;
    bool m_keyRightPressed;
    bool m_keySpacePressed;
};

#endif // PLAYER_H
