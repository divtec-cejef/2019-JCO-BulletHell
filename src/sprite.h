/**
\file
\brief    Déclaration de la classe Sprite.
\author   JCO
\date     janvier 2014
*/
#ifndef SPRITE_H
#define SPRITE_H

// décommenter pour rendre la boundingbox de tous les sprites visible.
#define DEBUG_BBOX
// décommenter pour rendre la shape de tous les sprites visible.
#define DEBUG_SHAPE
// décommenter pour afficher dans la sortie de debug le nombre de sprites existants
//#define DEBUG_SPRITE_COUNT

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QTimer>

class GameScene;
class SpriteTickHandler;

//! \brief Classe qui représente un élément d'animation graphique 2D.
//!
//! Cette classe met à disposition différentes méthodes permettant de gérer simplement un sprite animé.
//! Un sprite est un élément graphique qui affiche une image bitmap. Cet élément graphique doit appartenir à une scène.
//! Il possède un certain nombre de propriétés, associées à des méthodes, influençant son apparence et sa position :
//! - `pos` : la position 2D  du sprite
//! - `x` : la position sur l'axe horizontal du sprite
//! - `y` : la position sur l'axe vertical du sprite
//! - `z` : l'ordre z du sprite
//! - `rotation` : l'angle de rotation du sprite
//! - `scale` : un facteur de grossissement/rappetissment du sprite
//! - `opacity` : l'opacité du sprite, valeur décimal comprise en 0 et 1, 0 étant transparent et 1 opaque.
//!
//! Toutes ces propriétés peuvent être animées de façon très simple au moyen d'un objet QPropertyAnimation.
//!
//! D'autres méthodes permettent de connaître la géométrie et le placement du sprite : width(), height(), posX(), posY(), left(), right(), top(), bottom().
//! Il est possible d'obtenir le rectangle dans lequel le sprite est inscrit avec globalBoundingBox().
//! Il est possible d'obtenir la forme exacte du sprite avec globalShape().
//!
//! L'apparence du sprite n'est déterminée que par une seule image. Toutefois, il est possible d'en mémoriser plusieurs, afin de changer facilement d'apparence. Il est également possible de faire changer automatiquement ces images dans le but d'obtenir un sprite animé.
//!
//! La méthode addAnimationFrame() permet d'ajouter une image au sprite. Si plusieurs images sont ajoutées, elles sont conservées dans une liste qui préserve l'ordre d'ajout des images.
//!
//! La méthode setCurrentAnimationFrame() permet de spécifier quelle image doit être affichée (l'indice de la première image est 0). La méthode currentAnimationFrame() indique quelle image est actuellement affichée par le sprite.
//!
//! La méthode startAnimation() permet de démarrer l'animation des images. La méthode stopAnimation() permet de stopper l'animation des images. La vitesse d'animation peut être réglée avec setAnimationSpeed() ou au moment de démarrer l'animation.
//!
//! Il est également possible de demander au sprite d'émettre un signal chaque fois que l'animation est terminée, avec la méthode setEmitSignalEndOfAnimationEnabled().
//!
//!
//! Comme la classe Sprite spécialise la classe QGraphicsPixmapItem, qui elle-même spécialise la class QGraphicsItem, toutes les méthodes de QGraphicsItem sont accessibles au sprite.
//!
//! \section sprite_on_scene Intéger un sprite à une scène
//! Lorsqu'un sprite est créé, il faut le placer sur une scène pour qu'il apparaisse, au
//! moyen de la méthode GameScene::addSpriteToScene(). Celle-ci en prend alors la
//! propriété et se chargera de la détruire lorsque la scène sera détruite.
//!
//! Une fois ajouté à la scène, le sprite reçoit automatiquement la cadence du jeu
//! au moyen de la méthode virtuelle tick().
//!
//! \section tick_handler Le gestionnaire de cadence
//!
//! Un sprite peut être déplacé de plusieurs façons différents au sein d'une scène.
//!
//! Une première solution est qu'un élément externe se charge de la position d'un
//! sprite en utilisant les propriétés publiques du sprite, telle que x (setX()),
//! y (setY()) et z (setZ()) ou la fonction setPos().
//!
//! Une autre solution est de laisser le sprite gérer seul son déplacement en se
//! basant sur la cadence du jeu.
//!
//! Pour faire cela, il suffit d'attribuer au sprite un gestionnaire de cadence (SpriteTickHandler())
//! qui contiendra le code nécessaire pour faire bouger le sprite.
//!
//! Si un gestionnaire de cadence est attribué au sprite, à chaque tick, le gestionnaire
//! de cadence est appelé (tick()).
//!
//! Une dernière solution est  de spécialiser la classe Sprite afin de surcharger
//! la méthode tick().
//!
class Sprite : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged FINAL)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos FINAL)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged FINAL)
    Q_PROPERTY(qreal z READ zValue WRITE setZValue NOTIFY zChanged FINAL)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QPointF transformOriginPoint READ transformOriginPoint WRITE setTransformOriginPoint)
    Q_PROPERTY(int currentAnimationFrame READ currentAnimationFrame WRITE setCurrentAnimationFrame)

public:
    enum SpriteType {
        ST_PLAYER,
        ST_ENEMY,
        ST_BULLET
    };
    enum Emitter {
        EM_PLAYER,
        EM_ENEMY,
        EM_NONE
    };
    void setEmitter(Sprite::Emitter emitter);
    Emitter getEmitter();
    SpriteType getType();
    Sprite(QGraphicsItem* pParent = nullptr);
    Sprite(const QPixmap& rPixmap, QGraphicsItem* pParent = nullptr);
    virtual ~Sprite();

    void addAnimationFrame(const QPixmap& rPixmap);
    void setCurrentAnimationFrame(int frameIndex);
    int currentAnimationFrame() const;
    void clearAnimationFrames();
    void showNextAnimationFrame();
    void setAnimationSpeed(int frameDuration);
    void stopAnimation();
    void startAnimation();
    void startAnimation(int frameDuration);
    bool isAnimationRunning() const;

    void setEmitSignalEndOfAnimationEnabled(bool enabled);
    bool isEmitSignalEndOfAnimationEnabled() const;

    QRectF globalBoundingBox() const { return mapRectToScene(boundingRect());  }
    QPainterPath globalShape() const { return mapToScene(shape()); }
    int width() const { return static_cast<int>(globalBoundingBox().width()); }
    int height() const { return static_cast<int>(globalBoundingBox().height()); }
    int left() const { return static_cast<int>(globalBoundingBox().left()); }
    int top() const { return static_cast<int>(globalBoundingBox().top()); }
    int right() const { return static_cast<int>(globalBoundingBox().right()); }
    int bottom() const { return static_cast<int>(globalBoundingBox().bottom()); }

    void setParentScene(GameScene *pScene);

    enum { SpriteItemType = UserType + 1 };
    virtual int type() const { return SpriteItemType; }

    virtual void tick(int elapsedTimeInMilliseconds);

    void setTickHandler(SpriteTickHandler* pTickHandler);
    SpriteTickHandler* tickHandler() const;
    void removeTickHandler();

    GameScene *parentScene() const;
    QList<Sprite*> collidingSprites() const;

#if defined(DEBUG_BBOX) || defined(DEBUG_SHAPE)
    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);
#endif

signals:
    void animationFinished();
    void opacityChanged();
    void visibleChanged();
    void enabledChanged();
    void xChanged();
    void yChanged();
    void zChanged();
    void rotationChanged();
    void scaleChanged();

protected:
    //QList<Sprite*> collidingSprites() const;
    QList<Sprite*> collidingSprites(const QRectF& rRect) const;
    QList<Sprite*> collidingSprites(const QPainterPath& rShape) const;
    bool isInsideScene(const QPointF& rPosition) const;
    bool isInsideScene(const QRectF& rRect) const;
    GameScene* m_pParentScene;
    SpriteType spriteType;
    Emitter emitter;

private:
    static int s_spriteCount;
    static void displaySpriteCount();

    void init();

    SpriteTickHandler* m_pTickHandler;

    QTimer m_animationTimer;

    bool m_emitSignalEOA;

    QList<QPixmap> m_animationFrames;
    int m_frameDuration;
    int m_currentAnimationFrame;

    int m_customType;

private slots:
    void onNextAnimationFrame();

};

#endif // SPRITE_H
