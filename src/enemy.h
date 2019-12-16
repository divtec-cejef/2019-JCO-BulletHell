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
//! - Spécialiser la classe Enemy pour y ajouter une gestion des touches du clavier.
//! - Réagir aux événements clavier pour mémoriser l'état des touches, afin de
//!   déplacer correctement l'ennemi.
//! - Lors du tick(), déterminer la nouvelle position de l'ennemi
//! - Récupérer la bounding box (boîte d'encombrement) du sprite dans le système de
//!   coordonnées global avec globalBoundingBox() et déplacement à la future position
//!   de l'ennemi (avec translated()) pour vérifier qu'elle soit toujours
//!   incluse dans les limites de la surface de la scène.
//! - Tir un objet de classe bullet
//! - Ûn setter qui définit la cadence de tir de l'ennemi
//! - Une fonction pour gérer l'animation de l'ennemi
//!
class Enemy : public LivingEntity
{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT

    public:
        Enemy(QGraphicsItem* pParent = nullptr);
        virtual void tick(int elapsedTimeInMilliseconds);
        void setTimeBeforeShoot(int timeBeforeShoot);
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
