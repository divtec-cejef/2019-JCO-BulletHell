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
#include <QMediaPlayer>
#include <QMediaPlaylist>

class QAbstractAnimation;
class QGraphicsTransform;


class GameCanvas;
class GameScene;
class Sprite;
class Enemy;
class Player;
class Bullet;
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
    void exitGame();

    //Tableau permettant de stocker les différentes options des menu
    QGraphicsSimpleTextItem* m_pMenuItems[3];
    QGraphicsSimpleTextItem* m_pGameOverItems[2];
    QGraphicsSimpleTextItem* m_pYouWinItems[1];
    //Variable qui stockent l'option choisie
    int m_menuChoosenItem;
    int m_gameOverChoosenItem;
    int m_youWinChoosenItem;

    // Les scènes sont en public pour que le joueur puisse intéragir avec elles
    GameScene* m_pSceneGame;
    GameScene* m_pOldGameScene;
    GameScene* m_pSceneGameOver;
    GameScene* m_pSceneControl;
    GameScene* m_pSceneMenu;
    GameScene* m_pSceneYouWin;

signals:
    //Signaux
    void notifyKeyPressed(int key);
    void notifyKeyReleased(int key);

public slots:
    //Slot pour gérer la mort d'un ennemi/joueur
    void onPlayerDeath(bool playerDead);
    void onEnemyDeath(Enemy* enemy);

private:


    //Fonctions pour l'appui des touches dans les menus
    void whenKeyUpPressedMenus();
    void whenKeyDownPressedMenus();
    void whenKeySpacePressedMenus();
    void whenKeyEscapePressedMenus();

    void setupPlayer();

    //Fonctions pour gérer les vagues d'ennemis ou un ennemi
    void manageWaves();
    void setupEnemy();
    void clearWave();

    //Fonctions pour initialiser les scènes
    void setupSceneGameOver();
    void setupSceneGameScene();
    void setupSceneMenu();
    void setupSceneControl();
    void setupSceneYouWin();

    GameCanvas* m_pGameCanvas;

    //Variable membre
    Player* m_pPlayer;
    Enemy* m_pEnemy;
    QList<Enemy*> m_ennemyWave;
    QMediaPlayer* m_musicPlayer;
    bool m_keySpacePressed;
    int m_compteurWave;
    int m_ennemyPerWave;
    int m_maxWave;


private slots:

};


#endif // GAMECORE_H
