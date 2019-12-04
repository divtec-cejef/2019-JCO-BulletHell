/**
  \file
  \brief    Déclaration de la classe enemy.
  \author   Thibaud Nussbaumer
  \date     octobre 2019
*/
#ifndef ENEMY_H
#define ENEMY_H

#include "livingentity.h"
#include "sprite.h"

//! \brief Classe qui gère l'ennemi.
//!
//! Cette classe démontre les fonctionnalités suivantes :
//!
//! - Spécialiser la classe Sprite pour y ajouter une gestion des touches du clavier.
//! - Réagir aux événements clavier pour mémoriser l'état des touches, afin de
//!   déplacer correctement l'ennemi.
//!   Pour que les fonctions onKeyPressed() et onKeyReleased() soient appelées lorsqu'une
//!   touche est appuyée, il faut que l'objet qui crée une instance de ennemi se
//!   charge de les connecter aux signaux GameCore::notifyKeyPressed() et
//!   GameCore::notifyKeyReleased().
//! - Lors du tick(), déterminer la nouvelle position de la boule selon les touches
//!   du clavier qui sont appuyées.
//! - Récupérer la bounding box (boîte d'encombrement) du sprite dans le système de
//!   coordonnées global avec globalBoundingBox() et déplacement à la future position
//!   de l'ennemi (avec translated()) pour vérifier qu'elle soit toujours
//!   incluse dans les limites de la surface de la scène.
//!
class Enemy : public LivingEntity
{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT

    public:
        Enemy(QGraphicsItem* pParent = nullptr);
        virtual void tick(int elapsedTimeInMilliseconds);
    signals:
        void enemyDeath(Enemy* enemy);
    public slots:


    private:
        void updateEnemyVelocity();
        virtual void configureAnimation();
        virtual void updateVelocity();
        virtual void shoot();

        QPointF m_enemyVelocity;
        int health;
        int timeBeforeShoot; // temps à attendre avant de shoot
        int counterMillisecondsEnemy;

};

#endif // enemy_H
