/**
  Déclaration de la classe GameView, qui affiche une scène.

  @author   JCO
  @date     Janvier 2015
 */
#include "gameview.h"

#include <QDebug>
#include <QMouseEvent>

//! Construit une fenêtre de visualisation de la scène de jeu.
//! \param pParent  Widget parent.
GameView::GameView(QWidget* pParent) : QGraphicsView(pParent) {
    init();
}

//! Construit une fenêtre de visualisation de la scène de jeu.
//! \param pScene   Scène affichée par cette vue.
//! \param pParent  Widget parent.
GameView::GameView(QGraphicsScene* pScene, QWidget* pParent) : QGraphicsView(pScene, pParent){
    init();
}

//! Enclenche ou déclenche l'ajustage automatique de l'affichage de la scène à
//! la taille de la fenêtre.
//! \param fitToScreenEnabled  Indique si l'ajustage automatique est enclenché (true) ou
//!                            déclanché (false).
void GameView::setFitToScreenEnabled(bool fitToScreenEnabled) {
    m_fitToScreen = fitToScreenEnabled;
}

//! \return un booléen indiquant si l'ajustage automatique de l'affichage est
//!         enclenché (true) ou non (false).
bool GameView::isFitToScreenEnabled() const {
    return m_fitToScreen;
}

//! Enclenche ou déclenche le clipping de la scène.
//! \param clipSceneEnabled  Indique si le clipping est enclenché (true) ou
//!                          déclanché (false).
void GameView::setClipSceneEnabled(bool clipSceneEnabled) {
    m_clipScene = clipSceneEnabled;
}

//! \return un booléen indiquant si le clipping de la scène est enclenché ou non.
bool GameView::isClipSceneEnabled() const {
    return m_clipScene;
}

//! Gère le redimensionnement de l'affichage.
//! \param pEvent   Evénement de redimensionnement reçu.
void GameView::resizeEvent(QResizeEvent* pEvent) {
    QGraphicsView::resizeEvent(pEvent);
    m_clippingRectUpToDate = false;
    if (m_fitToScreen) {
        fitInView(sceneRect(), Qt::KeepAspectRatio);
    }

}

//! Si la scène doit être clippée, dessine en avant-plan des rectangles permettant
//! de cacher les marges de la scène, car il n'y a pas de méthodes propres à Qt le permettant,
//! étant donné que chaque QGraphicsItem est responsable de se dessiner.
//! \param pPainter     Painter à utiliser pour dessiner.
//! \param rRect        Zone à dessiner.
void GameView::drawForeground(QPainter* pPainter, const QRectF& rRect) {
    if (!m_clipScene)
        return;

    if (!m_clippingRectUpToDate) {
        m_clippingRect[0] = QRectF(rRect.left(), rRect.top(), rRect.width(), sceneRect().top() - rRect.top());
        m_clippingRect[1] = QRectF(rRect.left(), sceneRect().top(), sceneRect().left() - rRect.left(), sceneRect().height());
        m_clippingRect[2] = QRectF(sceneRect().right(), sceneRect().top(), rRect.right() - sceneRect().right(), sceneRect().height());
        m_clippingRect[3] = QRectF(rRect.left(), sceneRect().bottom(), rRect.width(), rRect.bottom() - sceneRect().bottom());
        m_clippingRectUpToDate = true;
    }

    for (int i = 0; i < 4; ++i)
        pPainter->fillRect(m_clippingRect[i], Qt::black);
}

//! Initialise cette affichage.
void GameView::init() {
    m_fitToScreen = false;
    m_clipScene = false;
    m_clippingRectUpToDate = false;

    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}
