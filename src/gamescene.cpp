/**
  \file
  \brief    Définition de la classe GameScene.
  \author   JCO
  \date     janvier 2014
*/
#include "gamescene.h"

#include <cstdlib>
#include <QApplication>
#include <QBrush>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>

#include "gamecore.h"
#include "resources.h"
#include "sprite.h"

//! Construit la scène de jeu avec une taille par défaut et un fond noir.
//! \param pParent  Objet propriétaire de cette scène.
GameScene::GameScene(QObject* pParent) : QGraphicsScene(pParent) {
    init();
}

//! Construit la scène de jeu avec la taille donnée et un fond noir.
//! \param rSceneRect   Taille de la scène.
//! \param pParent      Objet propriétaire de cette scène.
GameScene::GameScene(const QRectF& rSceneRect, QObject* pParent) : QGraphicsScene(rSceneRect, pParent) {
    init();
}

//! Construit la scène de jeu avec la taille donnée et un fond noir.
//! \param X        Position X de la scène.
//! \param Y        Position Y de la scène.
//! \param width    Largeur de la scène.
//! \param height   Hauteur de la scène.
//! \param pParent  Objet propriétaire de cette scène.
GameScene::GameScene(qreal X, qreal Y, qreal width, qreal height, QObject* pParent) : QGraphicsScene(X, Y, width, height, pParent) {
    init();
}

//! Destruction de la scène.
GameScene::~GameScene()  {
    delete m_pBackgroundImage;
    m_pBackgroundImage = nullptr;
}

//! Ajoute le sprite à la scène.
//! La scène prend possession du sprite et se chargera de l'effacer.
//! \param pSprite Pointeur sur le sprite à ajouter à la scène.
void GameScene::addSpriteToScene(Sprite* pSprite)
{
    Q_ASSERT(pSprite != nullptr);

    this->addItem(pSprite);
    m_spriteList.append(pSprite);
    pSprite->setParentScene(this);

    connect(pSprite, SIGNAL(destroyed(QObject*)), this, SLOT(onSpriteDestroyed(QObject*)));
}

//! Construit la liste de tous les sprites en collision avec le sprite donné en
//! paramètre.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param pSprite Sprite pour lequel les collisions doivent être vérifiées.
//! \return une liste de sprites en collision. Si aucun autre sprite ne collisionne
//! le sprite donné, la liste retournée est vide.
QList<Sprite*> GameScene::collidingSprites(const Sprite* pSprite) const {
    QList<Sprite*> spriteList;
    const auto collidingItems = pSprite->collidingItems();
    for(QGraphicsItem* pItem : collidingItems) {
        if (pItem->type() == Sprite::SpriteItemType)
            spriteList << static_cast<Sprite*>(pItem);
    }
    return spriteList;
}

//! Construit la liste de tous les sprites en collision avec le rectangle donné
//! en paramètre.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param rRect Rectangle avec lequel il faut tester les collisions.
//! \return une liste de sprites en collision.
QList<Sprite*> GameScene::collidingSprites(const QRectF &rRect) const  {
    QList<Sprite*> collidingSpriteList;
    for(Sprite* pSprite : sprites())  {
        QRectF globalBBox = pSprite->globalBoundingBox();
        if (globalBBox.intersects(rRect)) {
            collidingSpriteList << pSprite;
        }
    }
    return collidingSpriteList;
}

//! Construit la liste de tous les sprites en collision avec la forme donnée
//! en paramètre.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param rShape Forme avec laquelle il faut tester les collisions.
//! \return une liste de sprites en collision.
QList<Sprite*> GameScene::collidingSprites(const QPainterPath& rShape) const {
    QList<Sprite*> collidingSpriteList;
    auto spriteList = collidingSprites(rShape.boundingRect());
    for(Sprite* pSprite : spriteList)  {
        if (pSprite->globalShape().intersects(rShape)) {
            collidingSpriteList << pSprite;
        }
    }
    return collidingSpriteList;
}

//!
//! \return la liste des sprites de cette scène (y compris ceux qui ne sont pas visibles).
//!
QList<Sprite*> GameScene::sprites() const  {
    QList<Sprite*> spriteList;
    auto allItems = this->items();
    for(QGraphicsItem* pItem : allItems) {
        if (pItem->type() == Sprite::SpriteItemType)
            spriteList << static_cast<Sprite*>(pItem);
    }
    return spriteList;
}

//! Récupère le sprite visible le plus en avant se trouvant à la position donnée.
//! \return un pointeur sur le sprite trouvé, ou null si aucun sprite ne se trouve à cette position.
Sprite* GameScene::spriteAt(const QPointF& rPosition) const {
    QGraphicsItem* pTopMostVisibleItem = this->itemAt(rPosition, QTransform());
    if (pTopMostVisibleItem && pTopMostVisibleItem->type() == Sprite::SpriteItemType)
        return static_cast<Sprite*>(pTopMostVisibleItem);

    return nullptr;
}

//!
//! Affiche un texte sur la scène.
//! Si ce texte n'est pas détruit manuellement, il le sera automatiquement
//! lorsque la scène sera détruite.
//! \param initialPosition  Position initiale du texte.
//! \param rText            Texte à afficher.
//! \param size             Taille (en pixels) du texte.
//! \param color            Couleur du texte.
//! \return un pointeur sur l'élément graphique textuel.
//!
QGraphicsSimpleTextItem* GameScene::createText(QPointF initialPosition, const QString& rText, int size, QColor color) {
    QGraphicsSimpleTextItem* pText = this->addSimpleText(rText);
    pText->setPos(initialPosition);
    QFont textFont = pText->font();
    textFont.setPixelSize(size);
    pText->setFont(textFont);
    pText->setBrush(QBrush(color));
    return pText;
}

//! Défini l'image de fond à utiliser pour cette scène.
void GameScene::setBackgroundImage(const QImage& rImage)  {
    if (m_pBackgroundImage)
        delete m_pBackgroundImage;
    m_pBackgroundImage = new QImage(rImage);
}

//! Change la largeur de la scène.
//! \param sceneWidth   Largeur de la scène en pixels.
void GameScene::setWidth(int sceneWidth)  {
    setSceneRect(0,0, sceneWidth, height());
}

//! Change la hauteur de la scène.
//! \param sceneHeight   Hauteur de la scène, en pixels.
void GameScene::setHeight(int sceneHeight)  {
    setSceneRect(0,0, width(), sceneHeight);
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le tick précédent.
void GameScene::tick(int elapsedTimeInMilliseconds) {
    for(Sprite* pSprite : m_spriteList) {
        pSprite->tick(elapsedTimeInMilliseconds);
    }
}

//! Dessine le fond d'écran de la scène.
//! Si une image à été définie avec setBackgroundImage(), celle-ci est affichée.
//! Une autre méthode permet de définir une image de fond :
//! QGraphicsScene::setBackgroundBrush(QBrush(QPixmap(...))).
//! Cette deuxième méthode affiche cependant l'image comme un motif de tuile.
//! \see setBackgroundImage()
void GameScene::drawBackground(QPainter* pPainter, const QRectF& rRect)  {
    QGraphicsScene::drawBackground(pPainter, rRect);
    if (m_pBackgroundImage)
        pPainter->drawImage(0,0, *m_pBackgroundImage);
}

//! Initialise la scène
void GameScene::init() {
    m_pBackgroundImage = nullptr;

    this->setBackgroundBrush(QBrush(Qt::black));
    //setBackgroundImage(QImage(GameFramework::imagesPath() + "space.jpg"));

}

//! Retire de la liste des sprite le sprite qui va être détruit.
void GameScene::onSpriteDestroyed(QObject* pSprite) {
    Sprite* pSpriteDestroyed = static_cast<Sprite*>(pSprite);
    m_spriteList.removeAll(pSpriteDestroyed);
}
