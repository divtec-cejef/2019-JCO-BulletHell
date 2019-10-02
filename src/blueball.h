/**
  \file
  \brief    Déclaration de la classe BlueBall.
  \author   JCO
  \date     septembre 2018
*/
#ifndef BLUEBALL_H
#define BLUEBALL_H

#include "sprite.h"

//! \brief Classe qui gère la balle bleue.
//!
//! Cette classe démontre les fonctionnalités suivantes :
//!
//! - Spécialiser la classe Sprite pour y ajouter une gestion des touches du clavier.
//! - Réagir aux événements clavier pour mémoriser l'état des touches, afin de
//!   déplacer correctement la balle.
//!   Pour que les fonctions onKeyPressed() et onKeyReleased() soient appelées lorsqu'une
//!   touche est appuyée, il faut que l'objet qui crée une instance de BlueBall se
//!   charge de les connecter aux signaux GameCore::notifyKeyPressed() et
//!   GameCore::notifyKeyReleased().
//! - Lors du tick(), déterminer la nouvelle position de la boule selon les touches
//!   du clavier qui sont appuyées.
//! - Récupérer la bounding box (boîte d'encombrement) du sprite dans le système de
//!   coordonnées global avec globalBoundingBox() et déplacement à la future position
//!   de la balle (avec translated()) pour vérifier qu'elle soit toujours
//!   incluse dans les limites de la surface de la scène.
//!
class BlueBall : public Sprite
{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT

public:
    BlueBall(QGraphicsItem* pParent = nullptr);

    virtual void tick(int elapsedTimeInMilliseconds);

public slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);

private:
    void updateBallVelocity();

    QPointF m_ballVelocity;
    bool m_keyUpPressed;
    bool m_keyDownPressed;
    bool m_keyLeftPressed;
    bool m_keyRightPressed;

};

#endif // BLUEBALL_H
