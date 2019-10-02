/**
  \file
  \brief    Déclaration de la classe SpinningPinwheel.
  \author   JCO
  \date     septembre 2018
*/
#ifndef SPINNINGPINWHEEL_H
#define SPINNINGPINWHEEL_H

#include "sprite.h"

//! \brief Démontre le suivi de la souris.
//!
//! Cette classe spécialise Sprite pour gérer le déplacement
//! d'une roue tournante, qui suit en permanence le pointeur de la souris.
//!
//! Cette classe démontre également les fonctionnalités suivantes :
//! - Détecter les changements d'état d'un item (itemChange(), mécanisme hérité
//!   de Sprite qui lui-même l'hérite de QGraphicsItem) et plus particulièrement
//!   dans ce cas un changement de visibilité pour stopper l'animation de la roue
//!   si celle-ci est invisible.
//! - Générer une animation à partir d'une spritesheet (dans configureAnimation()).
class SpinningPinwheel : public Sprite
{
    Q_OBJECT

public:
    SpinningPinwheel(QGraphicsItem* pParent = nullptr);

public slots:
    void onMouseMove(QPointF newMousePosition);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& rValue);

private:
    void configureAnimation();
    void updateAnimationState();

    QPointF m_wheelShifting;

};

#endif // SPINNINGPINWHEEL_H
