/**
  \file
  \brief    Déclaration de la classe bullet.
  \author   Thibaud Nussbaumer
  \date    octobre 2019
*/
#ifndef BULLET_H
#define BULLET_H

#include "item.h"

//! \brief Classe qui gère item.
//!
//! Cette classe démontre les fonctionnalités suivantes :
//!
//! - Spécialiser la classe Sprite pour y ajouter une gestion des touches du clavier.
//! - Réagir aux événements clavier pour mémoriser l'état des touches, afin de
//!   déplacer correctement l'item.
//!   Pour que les fonctions onKeyPressed() et onKeyReleased() soient appelées lorsqu'une
//!   touche est appuyée, il faut que l'objet qui crée une instance de l'item se
//!   charge de les connecter aux signaux GameCore::notifyKeyPressed() et
//!   GameCore::notifyKeyReleased().
//! - Lors du tick(), déterminer la nouvelle position de la boule selon les touches
//!   du clavier qui sont appuyées.
//! - Récupérer la bounding box (boîte d'encombrement) du sprite dans le système de
//!   coordonnées global avec globalBoundingBox() et déplacement à la future position
//!   de l'item (avec translated()) pour vérifier qu'elle soit toujours
//!   incluse dans les limites de la surface de la scène.
//!
class Bullet : public Item
{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT

    public:
        Bullet(QGraphicsItem* pParent = nullptr);
        virtual void tick(int elapsedTimeInMilliseconds);
        virtual void updateVelocity(int x, int y);

    protected:
        virtual void configureAnimation();
        virtual void updateAnimationState();

        QPointF m_bulletVelocity;

};

#endif // BULLET_H
