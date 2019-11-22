/**
  \file
  \brief    Déclaration de la classe GameCore.
  \author   JCO
  \date     septembre 2018
*/
#ifndef GAMECORE_H
#define GAMECORE_H

#include <QList>
#include <QObject>
#include <QPointF>
#include <QGraphicsItem>

class QAbstractAnimation;
class QGraphicsTransform;


class GameCanvas;
class GameScene;
class Sprite;
class Enemy;
class Player;
//! \brief Classe qui gère la logique du jeu.
//!
//! Dans son état actuel, cette classe crée une scène vide, délimite
//! sa surface avec une ligne blanche puis démarre la cadence du jeu.
class GameCore : public QObject
{
    Q_OBJECT
public:
    explicit GameCore(GameCanvas* pGameCanvas, QObject *parent = nullptr);
    ~GameCore();

    void keyPressed(int key);
    void keyReleased(int key);

    void mouseMoved(QPointF newMousePosition);
    void mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);

    void tick(int elapsedTimeInMilliseconds);

    void menu();
    void control();
    void gameOver();
    void exitGame();

    QGraphicsSimpleTextItem* m_pMenuItems[3];
    QGraphicsSimpleTextItem* m_pGameOverItems[2];
    int menuChoosenItem = 0;
    int gameOverChoosenItem = 0;
    //bool playerDead = false; // Création d'un booléen pour gérer la mort du joueur


    // Les scene sont en public pour que le joueur puisse intéragir avec elles
    GameScene* m_pScene;
    GameScene* m_pOldGameScene;
    GameScene* m_pSceneGameOver;
    GameScene* m_pSceneControl;
    GameScene* m_pSceneMenu;

signals:
    void notifyMouseMoved(QPointF newMousePosition);
    void notifyMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyKeyPressed(int key);
    void notifyKeyReleased(int key);

public slots:
    void onPlayerDeath(bool playerDead);
    void onEnemyDeath(bool enemyDead);

private:

    void setupPlayer();
    void setupEnemy();

    GameCanvas* m_pGameCanvas;

    Player* m_pPlayer;
    Enemy* m_pEnemy;
    bool m_keySpacePressed;


private slots:

};


#endif // GAMECORE_H
