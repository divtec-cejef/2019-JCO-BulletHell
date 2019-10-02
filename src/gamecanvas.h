/**
  \file
  \brief    Déclaration de la classe GameCanvas.
  \author   JCO
  \date     janvier 2015
*/
#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include <QObject>
#include <QPointer>
#include <QTime>
#include <QTimer>

class GameCore;
class GameScene;
class GameView;
class QGraphicsSceneMouseEvent;
class QGraphicsTextItem;
class QKeyEvent;

//! \brief Classe de gestion des scènes du jeu et de sa cadence.
//!
//! GameCanvas prend en charge les différentes scènes constituant le jeu.
//! Il est possible, au sein de GameCore, de créer et gérer plusieurs scènes (des instances de GameScene). Toutefois, une
//! seule scène ne peut être affichée à la fois.
//! Pour créer une scène, il faut utiliser la méthode createScene() de cette classe.
//!
//! La méthode setCurrentScene() permet d'indiquer quelle scène doit être affichée.
//! La méthode currentScene() retourne la scène qui est actuellement affichée.
//!
//! Par défaut, aucune scène n'est affichée. La méthode setCurrentScene() doit être au moins appelée une fois par GameCore.
//!
//! Cette classe implémente également le mécanisme de cadence du jeu (le tick).
//!
//! Pour démarrer le tick, utiliser la commande startTick(). Dès que le tick est démarré, la méthode GameCore::tick() est
//! appelée régulièrement, toutes les 20 millisecondes par défaut.
//!
//! Elle se charge alors d'appeler la méthode GameCore::tick() et GameScene::tick() de façon
//! à ce que ces classes puissent réagir à la cadence.
//!
//! Pour stopper le tick, utiliser la commande stopTick().
//!
//! GameCanvas permet également d'enclencher le suivi des déplacements de la souris (startMouseTracking() et de
//! le stopper (stopMouseTracking()).
class GameCanvas : public QObject
{
    Q_OBJECT
public:
    enum { KEEP_PREVIOUS_TICK_INTERVAL = -1  };

    explicit GameCanvas(GameView* pView, QObject* pParent = nullptr);
    ~GameCanvas();


    GameScene* createScene();
    GameScene* createScene(const QRectF& rSceneRect);
    GameScene* createScene(qreal x, qreal y, qreal width, qreal height);

    void setCurrentScene(GameScene* pScene);
    GameScene* currentScene() const;

    void startTick(int tickInterval = KEEP_PREVIOUS_TICK_INTERVAL);
    void stopTick();

    void startMouseTracking();
    void stopMouseTracking();
    QPointF currentMousePosition() const;

signals:

public slots:

protected:
    bool eventFilter(QObject* pObject, QEvent* pEvent);

private:
    void initDetailedInfos();

    void keyPressed(QKeyEvent* pKeyEvent);
    void keyReleased(QKeyEvent* pKeyEvent);

    void mouseMoved(QGraphicsSceneMouseEvent* pMouseEvent);
    void mouseButtonPressed(QGraphicsSceneMouseEvent* pMouseEvent);
    void mouseButtonReleased(QGraphicsSceneMouseEvent* pMouseEvent);

    GameView* m_pView;
    GameCore* m_pGameCore;
    QPointer<QGraphicsTextItem> m_pDetailedInfosItem; // Smart Pointer pour qu'il soit mis à zéro au cas où l'item est effacé par GameScene::clear()

    bool m_keepTicking;

    QTime m_lastUpdateTime;
    QTimer m_tickTimer;

private slots:
    void onInit();
    void onTick();

};

#endif // GAMECANVAS_H
