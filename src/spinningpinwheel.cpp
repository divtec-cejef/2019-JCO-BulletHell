/**
  \file
  \brief    Déclaration de la classe SpinningPinwheel.
  \author   JCO
  \date     septembre 2018
*/
#include "spinningpinwheel.h"

#include "resources.h"

const int FRAME_WIDTH = 164;
const int FRAME_HEIGHT = 164;
const int FRAME_COUNT = 40;
const int COLUMN_COUNT = 5;
const float SCALE_RATIO = 0.5F;


//! Construit un marcheur et l'initialise.
//! \param pParent  Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
SpinningPinwheel::SpinningPinwheel(QGraphicsItem* pParent): Sprite(pParent) {
    configureAnimation();

    // Calcul du décalage par rapport aux coordonnées de la souris pour que
    // le centre de la roue se trouve sous le pointeur.
    m_wheelShifting = QPointF(FRAME_WIDTH * SCALE_RATIO / 2.,
                              FRAME_HEIGHT * SCALE_RATIO / 2.);
}

//! Traite le déplacement de la souris.
void SpinningPinwheel::onMouseMove(QPointF newMousePosition) {
    if (this->isVisible())
        this->setPos(newMousePosition - m_wheelShifting);
}

//! Découpe la spritesheet pour en extraire les étapes d'animation et
//! les ajouter au sprite.
void SpinningPinwheel::configureAnimation() {
    // Chargement de la spritesheet
    QImage spriteSheet(GameFramework::imagesPath() + "spinningwheel.png");

    // Découpage de la spritesheet
    for (int frameIndex = 0; frameIndex < FRAME_COUNT; frameIndex++) {
        QImage sprite = spriteSheet.copy((frameIndex % COLUMN_COUNT) * FRAME_WIDTH,
                                         (frameIndex / COLUMN_COUNT) * FRAME_HEIGHT,
                                         FRAME_WIDTH, FRAME_HEIGHT);

        this->addAnimationFrame(QPixmap::fromImage(sprite.scaled(FRAME_WIDTH * SCALE_RATIO,
                                                                 FRAME_HEIGHT * SCALE_RATIO,
                                                                 Qt::IgnoreAspectRatio,
                                                                 Qt::SmoothTransformation)));
    }

    this->setAnimationSpeed(15);
    updateAnimationState();
}

//! Active ou désactive l'animation selon si la roue est visible ou non.
void SpinningPinwheel::updateAnimationState() {
    if (this->isVisible()) {
        this->startAnimation();
    } else {
        this->stopAnimation();
    }
}

//! Traite le changement concernant ce sprite.
//! Si le changement concerne sa visibilité, on le traite.
QVariant SpinningPinwheel::itemChange(GraphicsItemChange change, const QVariant& rValue) {
    if (change == ItemVisibleHasChanged) {
        // rValue indique s'il est visible ou pas.
        updateAnimationState();
    }
    return Sprite::itemChange(change, rValue);
}
