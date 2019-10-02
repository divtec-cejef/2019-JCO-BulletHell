/**
  \file
  \brief    Définition de la classe AutomaticWalkingHandler.
  \author   JCO
  \date     septembre 2018
*/
#include "automaticwalkinghandler.h"

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "sprite.h"

const int DISTANCE_TO_WALK = 800;

//! Constructeur.
//! \param pParentSprite Sprite dont le déplacement doit être géré.
AutomaticWalkingHandler::AutomaticWalkingHandler(Sprite* pParentSprite) : SpriteTickHandler (pParentSprite) {

}

//! Initialise l'animation du sprite au moyen d'une objet QPropertyAnimation qui va
//! agir sur la propriété x du sprite.
void AutomaticWalkingHandler::init() {
    QPropertyAnimation* pAnimation = new QPropertyAnimation(m_pParentSprite, "x"); // La propriété à animer est x (la position x)
    pAnimation->setDuration(7000); // L'animation doit durer 7 secondes
    pAnimation->setStartValue(m_pParentSprite->pos().x());
    pAnimation->setEndValue(m_pParentSprite->pos().x() + DISTANCE_TO_WALK);

    // Lorsque l'animation sera terminée, notre slot onWalkFinished() sera automatiquement appelé.
    connect(pAnimation, &QPropertyAnimation::finished, this, &AutomaticWalkingHandler::onWalkFinished);

    // Démarrage de l'animation : la propriété x va varier de StartValue à EndValue en 7 secondes.
    // Lorsque cette animation est terminée, l'objet pointé par pAnimation est automatiquement effacé.
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

//! Cadence : inutilisée.
void AutomaticWalkingHandler::tick(int elapsedTime) {
    Q_UNUSED(elapsedTime);
}

//! Traite la fin de l'animation automatique du sprite : l'animation
//! de l'apparence est également stoppée.
void AutomaticWalkingHandler::onWalkFinished()  {
    m_pParentSprite->stopAnimation();
}

