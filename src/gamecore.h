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
    //Constructeur
    explicit GameCore(GameCanvas* pGameCanvas, QObject *parent = nullptr);
    //Destructeur
    ~GameCore();

    //Fonction permettant le traitement des touches appuyées/relâchées
    void keyPressed(int key);
    void keyReleased(int key);

    void tick(int elapsedTimeInMilliseconds);
    void gameOver();
    void exitGame();

    //Tableau permettant de stocker les différentes options du menu
    QGraphicsSimpleTextItem* m_pMenuItems[3];
    QGraphicsSimpleTextItem* m_pGameOverItems[2];
    //Variable qui stockent l'option choisie
    int m_menuChoosenItem = 0;
    int m_gameOverChoosenItem = 0;

    // Les scènes sont en public pour que le joueur puisse intéragir avec elles
    GameScene* m_pSceneGame;
    GameScene* m_pOldGameScene;
    GameScene* m_pSceneGameOver;
    GameScene* m_pSceneControl;
    GameScene* m_pSceneMenu;

signals:
    void notifyKeyPressed(int key);
    void notifyKeyReleased(int key);

public slots:
    void onPlayerDeath(bool playerDead);
    void onEnemyDeath(Enemy* enemy);

private:

    void manageWaves();
    void setupPlayer();
    void setupEnemy();
    void clearWave();

    void setupSceneGameOver();
    void setupSceneGameScene();
    void setupSceneMenu();
    void setupSceneControl();

    GameCanvas* m_pGameCanvas;

    Player* m_pPlayer;
    Enemy* m_pEnemy;
    QList<Enemy*> m_ennemyWave;
    bool m_keySpacePressed;


private slots:

};


#endif // GAMECORE_H
