/**
  \file
  \brief    Déclaration de la classe GameScene.
  \author   JCO
  \date     janvier 2014
*/
#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "gamecanvas.h"

#include <QGraphicsScene>

class Sprite;
class QGraphicsSimpleTextItem;
class QPainter;

//! \brief Représente l'espace 2D du jeu.
//!
//! GameScene représente une scène utilisée par l'interface graphique pour afficher une espace 2D du jeu dans un objet QGameView.
//!
//! La taille de l'espace de jeu (appelé une *scene*) peut être spécifié avec les méthodes setWidth() et setHeight().
//!
//! Cette classe met à disposition différentes méthodes pour simplifier le travail de développement d'un jeu :
//! - Gestion de sprites (Sprite) avec la méthode addSpriteToScene()
//! - Détection de collisions avec la méthode collidingSprites()
//! - Détection du sprite à une position donnée avec spriteAt()
//! - Affichage de textes avec la méthode createText()
//!
//! Cette classe ne gère pas la logique du jeu.
//!
//! Lorsque la cadence à lieu (tick()), elle se charge d'appeler la fonction Sprite::tick()
//! pour chaque sprite présent sur cette scène.
//!
//! Les événements de clavier et de la souris qu'elle reçoit sont interceptés par GameCanvas (au moyen d'un filtre à événements) et
//! retransmis à GameCore.
//!
//! Si plusieurs scènes co-existent, c'est celle qui est affichée qui reçoit les événements clavier et souris.
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    ~GameScene();

    void addSpriteToScene(Sprite* pSprite);
    QList<Sprite*> collidingSprites(const Sprite* pSprite) const;
    QList<Sprite*> collidingSprites(const QRectF& rRect) const;
    QList<Sprite*> collidingSprites(const QPainterPath& rShape) const;
    QList<Sprite*> sprites() const;
    Sprite* spriteAt(const QPointF& rPosition) const;

    QGraphicsSimpleTextItem* createText(QPointF initialPosition, const QString& rText, int size = 10, QColor color=Qt::white);

    void setBackgroundImage(const QImage& rImage);

    void setWidth(int sceneWidth);
    int width() const { return static_cast<int>(sceneRect().width()); }
    void setHeight(int sceneHeight);
    int height() const { return static_cast<int>(sceneRect().height()); }

    virtual void tick(int elapsedTimeInMilliseconds);

    bool isInsideScene(const QPointF& rPosition) const;
    bool isInsideScene(const QRectF& rRect) const;

protected:
    virtual void drawBackground(QPainter* pPainter, const QRectF& rRect);

private:
    // Seul GameCanvas est autorisé à instancier un GameScene, afin de garantir que
    // l'instanciation soit faite correctement.
    friend GameScene* GameCanvas::createScene();
    friend GameScene* GameCanvas::createScene(const QRectF& rSceneRect);
    friend GameScene* GameCanvas::createScene(qreal x, qreal y, qreal width, qreal height);

    explicit GameScene(QObject* pParent = nullptr);
    explicit GameScene(const QRectF& rSceneRect, QObject* pParent = nullptr);
    explicit GameScene(qreal x, qreal y, qreal width, qreal height, QObject* pParent = nullptr);

    void init();

    QImage* m_pBackgroundImage;
    QList<Sprite*> m_spriteList;

private slots:
    void onSpriteDestroyed(QObject* pSprite);
};

#endif // GAMESCENE_H
