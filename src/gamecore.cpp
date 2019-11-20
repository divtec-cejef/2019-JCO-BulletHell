/**
  Fichier qui contient toute la logique du jeu.

  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"

#include <cmath>

#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QGraphicsScale>
#include <QGraphicsSvgItem>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QSettings>

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

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {


    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;

    //Initialisation des scene
    m_pSceneControl = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    m_pSceneGameOver = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    m_pSceneMenu = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    gameOver();
    control();
    menu();


    // Initialise le joueur
    m_pPlayer = nullptr;
    // Initialise l'ennemi
    m_pEnemy = nullptr;

    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT /*/ GameFramework::screenRatio()*/);
    QImage img(GameFramework::imagesPath() + "background.png");
    m_pScene->setBackgroundImage(img);
    pGameCanvas->setCurrentScene(m_pScene);

    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));

    // Création du joueur
    setupPlayer();
    // Création de l'ennemi
    setupEnemy();

    // Affichage d'un texte
    QGraphicsSimpleTextItem* pText = m_pScene->createText(QPointF(0,0), "BulletHell", 70);
    pText->setOpacity(0.5);

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pScene;
    m_pScene = nullptr;
}

void GameCore::onPlayerDeath(bool playerDead){
    if(playerDead){
        qDebug() << "mort";
        disconnect(this, &GameCore::notifyKeyPressed, pPlayer, &Player::onKeyPressed);
        disconnect(this, &GameCore::notifyKeyReleased, pPlayer, &Player::onKeyReleased);
        disconnect(pPlayer, &Player::playerDeath, this, &GameCore::onPlayerDeath);
        delete pPlayer;
        m_pPlayer = nullptr;
        m_pGameCanvas->setCurrentScene(m_pSceneGameOver);
    }
}

void GameCore::onEnemyDeath(bool enemyDead){
    if(enemyDead){
        //delete pPlayer;
        disconnect(pEnemy, &Enemy::enemyDeath, this, &GameCore::onEnemyDeath);
        delete pEnemy;
        m_pEnemy = nullptr;
    }
}

//! Traite la pression d'une touche.
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
                    menuChoosenItem--;
                    m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::red);
                } else {
                    menuChoosenItem = 1;
                    m_pGameOverItems[gameOverChoosenItem]->setBrush(Qt::red);
                }
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
            else
                m_pGameCanvas->setCurrentScene(m_pOldGameScene);
            break;
        case Qt::Key_Space:
            if (m_pGameCanvas->currentScene() == m_pSceneMenu) {
                switch (menuChoosenItem) {
                case 0:
                    m_pGameCanvas->setCurrentScene(m_pOldGameScene);
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
                    m_pGameCanvas->setCurrentScene(m_pOldGameScene);
                    break;
                case 1:
                    exitGame();
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
    if(pPlayer->collidingSprites().contains(pEnemy)){
        qDebug() << "mort";
        pPlayer->death();
        m_pPlayer = nullptr;
    }
}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition) {
    emit notifyMouseMoved(newMousePosition);
}

//! Traite l'appui sur un bouton de la souris.
//! Fait apparaître le clip de la balle de foot qui tombe et rebondi.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}


//! Met en place le joueur
void GameCore::setupPlayer() {
    pPlayer = new Player;
    pPlayer->setPos(350, 470);
    pPlayer->setZValue(1);          // Passe devant tous les autres sprites
    m_pScene->addSpriteToScene(pPlayer);
    connect(this, &GameCore::notifyKeyPressed, pPlayer, &Player::onKeyPressed);
    connect(this, &GameCore::notifyKeyReleased, pPlayer, &Player::onKeyReleased);
    connect(pPlayer, &Player::playerDeath, this, &GameCore::onPlayerDeath);
    m_pPlayer = pPlayer;
}

//! Met en place l'ennemi
void GameCore::setupEnemy() {
    pEnemy = new Enemy;
    pEnemy->setPos(350, 100);
    pEnemy->setZValue(1);          // Passe devant tous les autres sprites
    m_pScene->addSpriteToScene(pEnemy);
    //Ajoute le déplacement manuel tirer du walkingman
    pEnemy->setTickHandler(new ManualWalkingHandler);
    connect(pEnemy, &Enemy::enemyDeath, this, &GameCore::onEnemyDeath);
    m_pEnemy = pEnemy;
}

//! Disposition de différents éléments dans la scène menu
void GameCore::gameOver(){
    // Crée la scène GameOver
    //m_pSceneGameOver = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre GameOver
    QGraphicsSimpleTextItem* pTextGameOver = m_pSceneGameOver->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2-200), "Game Over", 70);
    m_pGameOverItems[0] = m_pSceneGameOver->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2), "Rejouer", 50, Qt::white);
    m_pGameOverItems[1] = m_pSceneGameOver->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2 + 50), "Menu", 50, Qt::white);
    gameOverChoosenItem = 0;
}

//! Disposition de différents éléments dans la scène menu
void GameCore::menu(){
    //Crée le menu
    //m_pSceneMenu = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre Menu
    QGraphicsSimpleTextItem* pTextMenu = m_pSceneMenu->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2-200), "Menu", 70);
    // Affichage des différentes options du menu
    m_pMenuItems[0] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2), "Jouer", 50, Qt::white);
    m_pMenuItems[1] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2 + 50), "Contrôle", 50, Qt::white);
    m_pMenuItems[2] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2 + 100), "Quitter", 50, Qt::white);
    menuChoosenItem = 0;
}

//! Quitte le jeu
void GameCore::exitGame(){
    qDebug() << "Quitter";
    //exit (EXIT_SUCCESS);
}

//! Disposition de différents éléments dans la scène control
void GameCore::control(){
    // Crée la scène pour afficher les contrôles
    //m_pSceneControl = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre Contrôle
    QGraphicsSimpleTextItem* pTextControl = m_pSceneControl->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2-200), "Contrôle", 70);
    // Affichage des touches actives :
    QString texteControl = "w : Se déplacer en haut\n"
                    "s : Se déplacer en bas\n"
                    "d : Se déplacer à droite\n"
                    "a : Se déplacer à gauche\n"
                    "space : Tirer\n"
                    "ctrl+shift+i : affiche des informations sur la cadence.";
    m_pSceneControl->createText(QPointF(SCENE_WIDTH/2,SCENE_HEIGHT/2), texteControl, 30);
}
