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

    //Initialisation de la scène OldGameScene
    m_pOldGameScene = nullptr;

    // Initialise le joueur
    m_pPlayer = nullptr;

    // Création d'une liste d'ennemis
    QList<Enemy> m_ennemyWave;
    // Initialise le générateur aléatoire pour la position des ennemis
    std::srand(std::time(0));

    //On initialise à quelle vague d'ennemi le joueur commencera
    compteurWave = 1;

    //Initialisation des différents scènes
    // et des variables qui leurs sont spécifiques
    m_pSceneControl = nullptr;
    m_pSceneGame = nullptr;
    m_pSceneGameOver = nullptr;
    m_pSceneMenu = nullptr;
    setupSceneControl();
    setupSceneGameOver();
    setupSceneMenu();
    m_gameOverChoosenItem = 0;
    m_menuChoosenItem = 0;

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
        //qDebug() << "mort";
        disconnect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::onKeyPressed);
        disconnect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::onKeyReleased);
        disconnect(m_pPlayer, &Player::playerDeath, this, &GameCore::onPlayerDeath);
        m_pPlayer->deleteLater();
        m_pPlayer = nullptr;
        m_pSceneGame->deleteLater();
        m_pSceneGame = nullptr;
        m_pGameCanvas->setCurrentScene(m_pSceneGameOver);
        ennemyPerWave=2;
    }
}

//! Déconnecte le signal en lien avec l'ennemi et détruit ce dernier
//! \param  enemyDead Booléen pour indiquer si l'ennemi est mort
void GameCore::onEnemyDeath(Enemy *enemy){
    qDebug() << "onEnemyDeath";
    if(m_ennemyWave.contains(enemy)){
        Enemy* pEnemy = m_ennemyWave[m_ennemyWave.indexOf(enemy)];
        disconnect(pEnemy, &Enemy::enemyDeath, this, &GameCore::onEnemyDeath);
        pEnemy->deleteLater();
        pEnemy = nullptr;
        m_ennemyWave.removeAt(m_ennemyWave.indexOf(enemy));
        //qDebug() << "mort";
        //qDebug() << m_ennemyWave.length();
        if(m_ennemyWave.length() == 0){
            m_ennemyWave.clear();
            compteurWave+= 1;
            manageWaves();
        }
    }
}

void GameCore::clearWave(){
    for(int i = m_ennemyWave.length()-1; i > 0; i--){
        if(i>=0){
            Enemy* pEnemy = m_ennemyWave[i];
            disconnect(pEnemy, &Enemy::enemyDeath, this, &GameCore::onEnemyDeath);
            pEnemy->deleteLater();
            pEnemy = nullptr;
            m_ennemyWave.removeAt(i);
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
        //Si on est dans le menu
            if (m_pGameCanvas->currentScene() == m_pSceneMenu) {
                m_pMenuItems[m_menuChoosenItem]->setBrush(Qt::white);
                //Peint en rouge la sélection
                if (m_menuChoosenItem > 0) {
                    m_menuChoosenItem--;
                    m_pMenuItems[m_menuChoosenItem]->setBrush(Qt::red);
                } else {
                    m_menuChoosenItem = 2;
                    m_pMenuItems[m_menuChoosenItem]->setBrush(Qt::red);
                }
        //Si on est sur le GameOver
            }else if(m_pGameCanvas->currentScene() == m_pSceneGameOver){
                m_pGameOverItems[m_gameOverChoosenItem]->setBrush(Qt::white);
                //Peint en rouge la sélection
                if (m_gameOverChoosenItem > 0) {
                    m_gameOverChoosenItem--;
                    m_pGameOverItems[m_gameOverChoosenItem]->setBrush(Qt::red);
                } else {
                    m_gameOverChoosenItem = 1;
                    m_pGameOverItems[m_gameOverChoosenItem]->setBrush(Qt::red);
                }
            }
            break;
        case Qt::Key_S :
        //Si on est dans le menu
            if (m_pGameCanvas->currentScene() == m_pSceneMenu) {
                m_pMenuItems[m_menuChoosenItem]->setBrush(Qt::white);
                //Peint en rouge la sélection
                if (m_menuChoosenItem < 2) {
                    m_menuChoosenItem++;
                    m_pMenuItems[m_menuChoosenItem]->setBrush(Qt::red);
                } else {
                    m_menuChoosenItem = 0;
                    m_pMenuItems[m_menuChoosenItem]->setBrush(Qt::red);
                }
        //Si on est sur le GameOver
            }else if(m_pGameCanvas->currentScene() == m_pSceneGameOver){
                m_pGameOverItems[m_gameOverChoosenItem]->setBrush(Qt::white);
                //Peint en rouge la sélection
                if (m_gameOverChoosenItem < 1) {
                    m_gameOverChoosenItem++;
                    m_pGameOverItems[m_gameOverChoosenItem]->setBrush(Qt::red);
                } else {
                    m_gameOverChoosenItem = 0;
                    m_pGameOverItems[m_gameOverChoosenItem]->setBrush(Qt::red);
                }
            }
            break;
        case Qt::Key_Escape:
            //Affiche le menu si on est dans la scène "SceneGame"
            if (m_pGameCanvas->currentScene() != m_pSceneMenu && m_pGameCanvas->currentScene() != m_pSceneGameOver) {
                m_menuChoosenItem = 0;
                if(m_pGameCanvas->currentScene() != m_pSceneControl){
                    m_pOldGameScene = m_pGameCanvas->currentScene();
                }
                m_pGameCanvas->setCurrentScene(m_pSceneMenu);
            }
            //Affiche la scène "SceneGame"
            else{
                m_menuChoosenItem = 0;
                m_pGameCanvas->setCurrentScene(m_pOldGameScene);
            }
            break;
        case Qt::Key_Space:
            if (m_pGameCanvas->currentScene() == m_pSceneMenu) {
                switch (m_menuChoosenItem) {
                //0.Jouer -
                case 0:
                    //Affiche la zone de jeu où on s'était arrêté
                    if(m_pOldGameScene){
                        m_pGameCanvas->setCurrentScene(m_pOldGameScene);
                    //Affiche la zone de jeu à la première vague
                    }else{
                        setupSceneGameScene();
                        m_pGameCanvas->setCurrentScene(m_pSceneGame);
                        compteurWave = 1;
                        //manageWaves();
                    }
                    break;
                //1. Contrôles - Affiche la scène avec les contrôles du jeu
                case 1:
                    m_pGameCanvas->setCurrentScene(m_pSceneControl);
                    break;
                //2. Quitter - Quitte le jeu
                case 2:
                    exitGame();
                }
            //Si on est sur le GameOver
            }else if (m_pGameCanvas->currentScene() == m_pSceneGameOver){
                switch(m_gameOverChoosenItem){
                //0. Jouer - Relance une partie à la première vague
                case 0:
                    clearWave();
                    setupSceneGameScene();
                    m_pGameCanvas->setCurrentScene(m_pSceneGame);
                    compteurWave = 1;
                    ennemyPerWave = 2;
                    //manageWaves();
                    break;
                //1. Quitter - Retour au menu
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
    // Si le joueur est touché par un ennemi, Game Over
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
        qDebug() << "Ajout ennemi" << i;
        Enemy* pEnemy = new Enemy;
        m_ennemyWave.append(pEnemy);
        pEnemy->setPos(std::rand() % (SCENE_WIDTH-pEnemy->width()) + 1,100);
        pEnemy->setZValue(1);
        m_pSceneGame->addSpriteToScene(pEnemy);
        pEnemy->setTickHandler(new ManualWalkingHandler);
        connect(pEnemy, &Enemy::enemyDeath, this, &GameCore::onEnemyDeath);
    }
    qDebug() << "length" << m_ennemyWave.length();
}

//! Gère les vagues d'ennemis
void GameCore::manageWaves(){
    qDebug() << "ennemyPerWave" << ennemyPerWave;
    qDebug() << "compteurWave" << compteurWave;
    switch(compteurWave){
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
            break;
        case 4:
            qDebug() << "You win";
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

//! Crée la scène GameOver et y ajoute différent éléments;
void GameCore::setupSceneGameOver(){
    // Création et disposition de la scène GameOver
    // Crée la scène GameOver
    m_pSceneGameOver = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre GameOver
    m_pSceneGameOver->createText(QPointF(SCENE_WIDTH-515,SCENE_HEIGHT/2-200), "Game Over", 70);
    // Affichage des différentes options du menu
    m_pGameOverItems[0] = m_pSceneGameOver->createText(QPointF(SCENE_WIDTH-425,SCENE_HEIGHT/2), "Rejouer", 50, Qt::red);
    m_pGameOverItems[1] = m_pSceneGameOver->createText(QPointF(SCENE_WIDTH-425,SCENE_HEIGHT/2 + 50), "Menu", 50, Qt::white);
    m_gameOverChoosenItem = 0;
}

//! Crée la scène Menu et y ajoute différent éléments
void GameCore::setupSceneMenu(){
    // Création et disposition de la scène Menu
    // Crée la scène Menu
    m_pSceneMenu = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre Menu
    m_pSceneMenu->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2-200), "Menu", 70);
    // Affichage des différentes options du menu
    m_pMenuItems[0] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2), "Jouer", 50, Qt::red);
    m_pMenuItems[1] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2 + 50), "Contrôles", 50, Qt::white);
    m_pMenuItems[2] = m_pSceneMenu->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2 + 100), "Quitter", 50, Qt::white);
    m_menuChoosenItem = 0;

#ifdef QT_DEBUG
    // Bricolage pour forcer le chargement de tous les plugins dès le lancement
    // de l'application. Si on ne fait pas ça, impossible de débogguer l'app, car
    // c'est au moment de la création des ennemis que les formats d'images sont chargés,
    // ce qui est trop tard.
    Enemy* pEnemy = new Enemy;
    pEnemy->setPos(std::rand() % (SCENE_WIDTH-pEnemy->width()) + 1,100);
    pEnemy->setZValue(1);
    m_pSceneMenu->addSpriteToScene(pEnemy);
#endif
}

//! Crée la scène Control et y ajoute différent éléments
void GameCore::setupSceneControl(){
    //Création et dispositon de la scène Control
    // Crée la scène pour afficher les contrôles
    m_pSceneControl = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre Contrôle
    m_pSceneControl->createText(QPointF(SCENE_WIDTH-500,SCENE_HEIGHT/2-200), "Contrôles", 70);
    // Affichage des touches actives :
    QString texteControl = "w : Se déplacer en haut\n"
                    "s : Se déplacer en bas\n"
                    "d : Se déplacer à droite\n"
                    "a : Se déplacer à gauche\n"
                    "space : Tirer\n"
                    "ctrl+shift+i : infos sur la cadence.";
    m_pSceneControl->createText(QPointF(SCENE_WIDTH-525,SCENE_HEIGHT/2), texteControl, 30);
}

//! Crée la scène GameScene et y ajoute différent éléments
void GameCore::setupSceneGameScene(){
    //Initialisation de la GameScene (zone de jeu)
    m_pSceneGame = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    QImage img(GameFramework::imagesPath() + "background.png");
    m_pSceneGame->setBackgroundImage(img);
    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pSceneGame->addRect(m_pSceneGame->sceneRect(), QPen(Qt::white));
    // Création du joueur
    setupPlayer();
    manageWaves();
    // Affichage d'un texte
    QGraphicsSimpleTextItem* pText = m_pSceneGame->createText(QPointF(0,0), "BulletHell", 70);
    pText->setOpacity(0.5);
}
