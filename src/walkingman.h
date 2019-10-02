/**
  \file
  \brief    Déclaration de la classe WalkingMan.
  \author   JCO
  \date     septembre 2018
*/
#ifndef WALKINGMAN_H
#define WALKINGMAN_H

#include "sprite.h"

//! \brief Classe qui gère un homme qui marche.
//!
//! Classe qui démontre la spécialisation de la classe Sprite afin de mettre
//! en place le chargement des images d'animation et l'utilisation de la macro
//! Q_OBJECT afin de pouvoir retrouver un sprite donné en fonction de son type
//! et au moyen de qobject_cast.
//!
//! - Changer automatiquement l'apparence d'un sprite (dans ce cas, les différentes étapes d'une marche) avec les
//!   méthodes Sprite::addAnimationFrame(), et Sprite::stopAnimation(). Cela est fait dans la fonction configureWalking().
//!
//! - Déplacer un sprite manuellement, en calculant à chaque tick() quelle distance il a parcouru (en tenant compte de
//!   sa vitesse de déplacement et du temps qui s'est écoulé) et en lui changeant sa position : Dans la méthode tick(),
//!   appelée automatiquement, le marcheur est déplacé et une détection de collision est réalisée afin de le faire
//!   changer de sens de marche s'il touche un obstacle.
//!
//! - Déplacer un sprite automatiquement, en utilisant un objet QPropertyAnimation, qui se charge d'animer automatiquement
//!   une propriété d'un objet, en l'occurence la propriété Sprite::x() du sprite représentant un marcheur. Cela est fait
//!   dans la fonction initAutomaticWalkingMan().
//!   Cette fonction connecte le slot onWalkFinished() au signal QPropertyAnimation::finished() (émis par QPropertyAnimation lorsque
//!   l'animation est terminée) afin de stopper l'animation du marcheur.
//!   Au moment de démarrer l'animation (avec start()), on précise que celle-ci doit être détruite (QAbstractAnimation::DeleteWhenStopped)
//!   une fois terminée.
//!
class WalkingMan : public Sprite
{
    Q_OBJECT
public:

    WalkingMan(QGraphicsItem* pParent = nullptr);
    virtual void tick(int elapsedTimeInMilliseconds);

private:
    void configureAnimation();


public slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);

private:
    void updateWalkingManVelocity();

    QPointF m_WalkingManVelocity;
    bool m_keyUpPressed;
    bool m_keyDownPressed;
    bool m_keyLeftPressed;
    bool m_keyRightPressed;
};
#endif // WALKINGMAN_H
