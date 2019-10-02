/**
  Déclaration de la classe GameView, qui affiche une scène.

  @author   JCO
  @date     Janvier 2015
 */
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>

//! \brief Classe de visualisation d'un espace 2D de jeu.
//!
//! GameView représente la vue utilisée par l'application pour afficher une scène (QGraphicsScene), généralement plus
//! spécifiquement la scène de jeu (GameScene)..
//!
//! Cette classe implémente les fonctionnalités suivantes :
//! - Possibilité d'adapter l'affichage de la scène à la taille de la fenêtre. Cette possibilité est déclanchée par défaut
//!   et peut être enclenchée avec setFitToScreenEnabled().
//! - Possibilité de "clipper" l'affichage de la scène, afin que tout élment en dehors de la surface de la scène soit
//!   caché. Cette possibilité est déclanchée par défaut et peut être enclenchée avec setClipSceneEnabled().
//!
class GameView : public QGraphicsView
{
public:
    GameView(QWidget* pParent = nullptr);
    GameView(QGraphicsScene* pScene, QWidget* pParent = nullptr);

    void setFitToScreenEnabled(bool fitToScreenEnabled);
    bool isFitToScreenEnabled() const;

    void setClipSceneEnabled(bool clipSceneEnabled);
    bool isClipSceneEnabled() const;

protected:
    virtual void resizeEvent(QResizeEvent* pEvent);
    virtual void drawForeground(QPainter* pPainter, const QRectF& rRect);

private:
    void init();

    bool m_fitToScreen;
    bool m_clipScene;

    bool m_clippingRectUpToDate;
    QRectF m_clippingRect[4];
};

#endif // GAMEVIEW_H
