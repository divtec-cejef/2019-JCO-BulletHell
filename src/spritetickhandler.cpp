/**
  \file
  \brief    Définition de la classe SpriteTickHandler.
  \author   JCO
  \date     septembre 2018
*/
#include "spritetickhandler.h"

class Sprite;

//! Construit un gestionnaire de tick pour le Sprite donné.
//! \param pParentSprite  Sprite qui sera géré par ce gestionnaire.
SpriteTickHandler::SpriteTickHandler(Sprite* pParentSprite) {
    setParentSprite(pParentSprite);
}

//! Destructeur
SpriteTickHandler::~SpriteTickHandler() {

}

//! Indique quel sprite sera géré par ce gestionnaire.
void SpriteTickHandler::setParentSprite(Sprite* pParentSprite) {
    m_pParentSprite = pParentSprite;
}
