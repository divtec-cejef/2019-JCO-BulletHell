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
//! \brief Classe qui gère le joueur.
//!
//! Cette classe démontre les fonctionnalités suivantes :
//!
//! - Spécialiser la classe Player pour y ajouter une gestion des touches du clavier.
//! - Réagir aux événements clavier pour mémoriser l'état des touches, afin de
//!   déplacer correctement le joueur.
//!   Pour que les fonctions onKeyPressed() et onKeyReleased() soient appelées lorsqu'une
//!   touche est appuyée, il faut que l'objet qui crée une instance de joueur se
//!   charge de les connecter aux signaux GameCore::notifyKeyPressed() et
//!   GameCore::notifyKeyReleased().
//! - Lors du tick(), déterminer la nouvelle position du joueur selon les touches
//!   du clavier qui sont appuyées.
//! - Récupérer la bounding box (boîte d'encombrement) du sprite dans le système de
//!   coordonnées global avec globalBoundingBox() et déplacement à la future position
//!   du joueur (avec translated()) pour vérifier qu'elle soit toujours
//!   incluse dans les limites de la surface de la scène.
//! - Gestion de l'animation du Sprite du joueur
//! - Créer un objet de classe bullet pour l'envoyer dans une direction afin
//!   de tuer les ennemis
//!
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
