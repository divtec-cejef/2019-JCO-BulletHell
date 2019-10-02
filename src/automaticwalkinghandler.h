/**
  \file
  \brief    Déclaration de la classe AutomaticWalkingHandler.
  \author   JCO
  \date     septembre 2018
*/
#ifndef AUTOMATICWALKINGHANDLER_H
#define AUTOMATICWALKINGHANDLER_H
#include <QObject>

#include "spritetickhandler.h"


//! \brief Gestionnaire de déplacement manuel.
//!
//! Gestionnaire de déplacement qui se base sur l'animation des propriétés d'un sprite.
//!
//! Plus spécifiquement, un objet QPropertyAnimation est utilisé, afin d'animer
//! automatiquement la propriété Sprite::x() du sprite à déplacer. Cela est fait
//! dans la fonction init().
//!
//! Cette fonction connecte le slot onWalkFinished() au signal QPropertyAnimation::finished() (émis par QPropertyAnimation lorsque
//! l'animation est terminée) afin de stopper l'animation du sprite.
//!
//! Au moment de démarrer l'animation (à la fin de init()), on précise que celle-ci doit être détruite (QAbstractAnimation::DeleteWhenStopped)
//! une fois terminée.
//!
//! Ce type d'animation n'utilise pas la cadence. La fonction tick() reste donc vide.
//!
class AutomaticWalkingHandler : public QObject, public SpriteTickHandler
{
    Q_OBJECT

public:

    AutomaticWalkingHandler(Sprite* pParentSprite = nullptr);

    virtual void init();
    void tick(int elapsedTimeInMilliseconds);

private slots:
    void onWalkFinished();

};

#endif // AUTOMATICWALKINGHANDLER_H
