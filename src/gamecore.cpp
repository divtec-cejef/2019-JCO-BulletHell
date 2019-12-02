/**
  Fichier qui contient toute la logique du jeu.

  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"

#include <cmath>
#include <ctime>

#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QGraphicsScale>
#include <QGraphicsSvgItem>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QSettings>
#include <QTimer>

#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "sprite.h"
#include "utilities.h"
#include "manualwalkinghandler.h"
#include "livingentity.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "bullet.h"


const int SCENE_WIDTH = 700;
const int SCENE_HEIGHT = 880;
int compteurWave = 0;
int ennemyPerWave = 2;

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {


    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;

    //Création et dispositon de la scène Control
    // Crée la scène pour afficher les contrôles
    m_pSceneControl = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre Contrôle
    m_pSceneControl->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2-200), "Contrôle", 70);
    // Affichage des touches actives :
    QString texteControl = "w : Se déplacer en haut\n"
                    "s : Se déplacer en bas\n"
                    "d : Se déplacer à droite\n"
                    "a : Se déplacer à gauche\n"
                    "space : Tirer\n"
                    "ctrl+shift+i : infos sur la cadence.";
    m_pSceneControl->createText(QPointF(SCENE_WIDTH-600,SCENE_HEIGHT/2), texteControl, 30);

    // Création et disposition de la scène GameOver
    // Crée la scène GameOver
    m_pSceneGameOver = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre GameOver
    m_pSceneGameOver->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2-200), "Game Over", 70);
    // Affichage des différentes options du menu
    m_pGameOverItems[0] = m_pSceneGameOver->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2), "Rejouer", 50, Qt::white);
    m_pGameOverItems[1] = m_pSceneGameOver->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2 + 50), "Menu", 50, Qt::white);
    gameOverChoosenItem = 0;

    // Création et disposition de la scène Menu
    // Crée la scène Menu
    m_pSceneMenu = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre Menu
    m_pSceneMenu->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2-200), "Menu", 70);
    // Affichage des différentes options du menu
    m_pMenuItems[0] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2), "Jouer", 50, Qt::white);
    m_pMenuItems[1] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2 + 50), "Contrôle", 50, Qt::white);
    m_pMenuItems[2] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2 + 100), "Quitter", 50, Qt::white);
    menuChoosenItem = 0;


    // Initialise le joueur
    m_pPlayer = nullptr;

    // Création d'une liste d'ennemis
    QList<Enemy> ennemyWave;
    // Initialise le générateur aléatoire pour la position des ennemis
    std::srand(std::time(0));

    //Initialisation de la scène Game (zone de jeu)
    m_pSceneGame = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    QImage img(GameFramework::imagesPath() + "background.png");
    m_pSceneGame->setBackgroundImage(img);
    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pSceneGame->addRect(m_pSceneGame->sceneRect(), QPen(Qt::white));
    // Création du joueur
    setupPlayer();
    // Affichage d'un texte
    QGraphicsSimpleTextItem* pText = m_pSceneGame->createText(QPointF(0,0), "BulletHell", 70);
    pText->setOpacity(0.5);

    compteurWave = 1;
    m_pGameCanvas->setCurrentScene(m_pSceneMenu);

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pSceneGame;
    m_pSceneGame = nullptr;
}


//! Déconnecte les signaux en lien avec les joueurs et détruit ce dernier
//! Affiche le GameOver
//! \param  playerDead Booléen pour indiquer si le joueur est mort
void GameCore::onPlayerDeath(bool playerDead){
    if(playerDead){
        clearWave();
        //qDebug() << "mort";
        disconnect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::onKeyPressed);
        disconnect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::onKeyReleased);
        disconnect(m_pPlayer, &Player::playerDeath, this, &GameCore::onPlayerDeath);
        m_pPlayer->deleteLater();
        m_pPlayer = nullptr;
        m_pGameCanvas->setCurrentScene(m_pSceneGameOver);
    }
}

//! Déconnecte le signal en lien avec l'ennemi et détruit ce dernier
//! \param  enemyDead Booléen pour indiquer si l'ennemi est mort
void GameCore::onEnemyDeath(Enemy *enemy){
    if(ennemyWave.contains(enemy)){
        Enemy* pEnemy = ennemyWave[ennemyWave.indexOf(enemy)];
        disconnect(pEnemy, &Enemy::enemyDeath, this, &GameCore::onEnemyDeath);
        pEnemy->deleteLater();
        pEnemy = nullptr;
        ennemyWave.removeAt(ennemyWave.indexOf(enemy));
        //qDebug() << "mort";
        //qDebug() << ennemyWave.length();
        if(ennemyWave.length() == 0){
            ennemyWave.clear();
            compteurWave+= 1;
            manageWaves();
        }
    }
}

void GameCore::clearWave(){
    for(int i = ennemyPerWave-1; i > 0; i--){
        Enemy* pEnemy = ennemyWave[i];
        disconnect(pEnemy, &Enemy::enemyDeath, this, &GameCore::onEnemyDeath);
        pEnemy->deleteLater();
        pEnemy = nullptr;
        ennemyWave.removeAt(i);
        qDebug() << "mort";
        qDebug() << ennemyWave.length();
        if(ennemyWave.length() == 0){
            ennemyWave.clear();
            compteurWave+= 1;
            manageWaves();
        }
    }
}

//! Traite la pression d'une touche lorsqu'on est dans un menu
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {
    emit notifyKeyPressed(key);
    switch (key)  {
        case Qt::Key_W :
            if (m_pGameCanvas->currentScene() == m_pSceneMenu) {
                m_pMenuItems[menuChoosenItem]->setBrush(Qt::white);
                if (menuChoosenItem > 0) {
                    menuChoosenItem--;
                    m_pMenuItems[menuChoosenItem]->setBrush(Qt::red);
                } else {
                    menuChoosenItem = 2;
                    m_pMenuItems[menuChoosenItem]->setBrush(Qt::red);
                }
            }else if(m_pGameCanvas->currentScene() == m_pSceneGameOver){
                m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::white);
                if (gameOverChoosenItem > 0) {
                    gameOverChoosenItem--;
                    m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::red);
                } else {
                    gameOverChoosenItem = 1;
                    m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::red);
                } /*else if(gameOverChoosenItem = 0){
                    gameOverChoosenItem = 1;
                    m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::red);
                }*/
            }
            break;
        case Qt::Key_S :
            if (m_pGameCanvas->currentScene() == m_pSceneMenu) {
                m_pMenuItems[menuChoosenItem]->setBrush(Qt::white);
                if (menuChoosenItem < 2) {
                    menuChoosenItem++;
                    m_pMenuItems[menuChoosenItem]->setBrush(Qt::red);
                } else {
                    menuChoosenItem = 0;
                    m_pMenuItems[menuChoosenItem]->setBrush(Qt::red);
                }
            }else if(m_pGameCanvas->currentScene() == m_pSceneGameOver){
                m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::white);
                if (gameOverChoosenItem < 1) {
                    gameOverChoosenItem++;
                    m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::red);
                } else {
                    gameOverChoosenItem = 0;
                    m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::red);
                }
            }
            break;
        case Qt::Key_Escape:
            if (m_pGameCanvas->currentScene() != m_pSceneMenu && m_pGameCanvas->currentScene() != m_pSceneGameOver) {
                menuChoosenItem = 0;
                if(m_pGameCanvas->currentScene() != m_pSceneControl){
                    m_pOldGameScene = m_pGameCanvas->currentScene();
                }
                m_pGameCanvas->setCurrentScene(m_pSceneMenu);
            }
            else{
                menuChoosenItem = 0;
                m_pGameCanvas->setCurrentScene(m_pOldGameScene);
            }
            break;
        case Qt::Key_Space:
            if (m_pGameCanvas->currentScene() == m_pSceneMenu) {
                switch (menuChoosenItem) {
                case 0:
                    if(m_pOldGameScene){
                        m_pGameCanvas->setCurrentScene(m_pOldGameScene);
                    }else{
                        m_pGameCanvas->setCurrentScene(m_pSceneGame);
                        compteurWave = 1;
                        manageWaves();
                    }

                    break;
                case 1:
                    m_pGameCanvas->setCurrentScene(m_pSceneControl);
                    break;
                case 2:
                    exitGame();
                }
            }else if (m_pGameCanvas->currentScene() == m_pSceneGameOver){
                switch(gameOverChoosenItem){
                case 0:
                    m_pGameCanvas->setCurrentScene(m_pSceneGame);
                    setupPlayer();
                    compteurWave = 1;
                    manageWaves();
                    break;
                case 1:
                    m_pGameCanvas->setCurrentScene(m_pSceneMenu);
                }
            }

        }
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);
}

//! Cadence.
//! Gère la mort du joueur dès la collision avec un ennemi
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(int elapsedTimeInMilliseconds) {
    if (m_pPlayer == nullptr)
        return;

    // Vérification du contact du joueur avec un ennemi
    // Si le joueur est touchée par un ennemi, Game Over
    if(!m_pPlayer->collidingSprites().isEmpty()){
        //On enlève le joueur de sa liste de collision
        m_pPlayer->collidingSprites().removeAll(m_pPlayer);
        if(!m_pPlayer->collidingSprites().isEmpty()){
            if(m_pPlayer->collidingSprites().first()->getType() == Sprite::SpriteType_e::ST_ENEMY){
                this->onPlayerDeath(true);
            }
        }
    }

}

//! Met en place le joueur
void GameCore::setupPlayer() {
    m_pPlayer = new Player;
    m_pPlayer->setPos(0,0);//(350, 470);
    m_pPlayer->setZValue(1);          // Passe devant tous les autres sprites
    m_pSceneGame->addSpriteToScene(m_pPlayer);
    connect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::onKeyPressed);
    connect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::onKeyReleased);
    connect(m_pPlayer, &Player::playerDeath, this, &GameCore::onPlayerDeath);
}

//! Place sur la scène la vague d'ennemis
void GameCore::setupEnemy() {
    for(int i = 0; i < ennemyPerWave; i++){
        Enemy* pEnemy = new Enemy;
        ennemyWave.append(pEnemy);
        pEnemy->setPos(std::rand() % (SCENE_WIDTH-pEnemy->width()) + 1,100);
        pEnemy->setZValue(1);
        m_pSceneGame->addSpriteToScene(pEnemy);
        pEnemy->setTickHandler(new ManualWalkingHandler);
        connect(pEnemy, &Enemy::enemyDeath, this, &GameCore::onEnemyDeath);
        qDebug() << "length" << ennemyWave.length();
    }
}

//! Gère les vagues d'ennemis
void GameCore::manageWaves(){
    qDebug() << ennemyPerWave;
    qDebug() << compteurWave;
    switch(compteurWave){
        case 0: compteurWave=0; break;
        case 1:
            //Première vague
            qDebug() << "firstWave";
            setupEnemy();
            break;
        case 2:
            //Deuxième vague
            qDebug() << "secondWave";
            ennemyPerWave += 2;
            setupEnemy();
            break;
        case 3:
            //Troisième vague
            qDebug() << "thirdWave";
            ennemyPerWave += 2;
            setupEnemy();
    }
}

//! Disposition de différents éléments dans la scène GameOver
void GameCore::gameOver(){
    //supprime l'ancienne scène afin qu'elle ne tourne pas en fond
    delete m_pSceneGame;
    m_pSceneGame = nullptr;

}

//! Quitte le jeu
void GameCore::exitGame(){
    qDebug() << "Quitter";
    //exit (EXIT_SUCCESS);
}


