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

    //On initialise les variables en lien avec les vagues d'ennemis
    m_compteurWave = 1;
    m_ennemyPerWave = 0;
    m_maxWave = 5;

    //Initialisation des différents scènes
    // et des variables qui leurs sont spécifiques
    m_pSceneControl = nullptr;
    m_pSceneGame = nullptr;
    m_pSceneGameOver = nullptr;
    m_pSceneMenu = nullptr;
    m_pSceneYouWin = nullptr;
    setupSceneControl();
    setupSceneGameOver();
    setupSceneMenu();
    setupSceneYouWin();
    m_gameOverChoosenItem = 0;
    m_menuChoosenItem = 0;
    m_youWinChoosenItem = 0;

    //On définit la scène qui sera affichée
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
    delete m_pSceneControl;
    delete m_pSceneGameOver;
    delete m_pSceneMenu;
    m_pSceneGame = nullptr;
    m_pSceneControl = nullptr;
    m_pSceneGameOver = nullptr;
    m_pSceneMenu = nullptr;
}


//! Déconnecte les signaux en lien avec les joueurs et détruit ce dernier
//! Affiche le GameOver
//! \param  playerDead Booléen pour indiquer si le joueur est mort
void GameCore::onPlayerDeath(bool playerDead){
    if(playerDead){
        //On déconnecte le signal et le slot
        disconnect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::onKeyPressed);
        disconnect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::onKeyReleased);
        disconnect(m_pPlayer, &Player::notifyPlayerDeath, this, &GameCore::onPlayerDeath);
        //On le "supprime plus tard" quand on ne fait
        //plus appel à celui-ci
        m_pPlayer->deleteLater();
        //Le pointeur pointe sur nullptr
        m_pPlayer = nullptr;
        //On affiche la scène GameOver
        m_pGameCanvas->setCurrentScene(m_pSceneGameOver);
        //On réinitialise le nombre d'ennemi et la vague
        // pour éviter que le joueur se retrouve à une vague
        // plus élevée que la première lorsqu'il relance une partie
        m_ennemyPerWave = 0;
        m_compteurWave = 1;
    }
}

//! Déconnecte le signal en lien avec la bullet et détruit cette dernière
//! \param bullet
void GameCore::onBulletDestroyed(Bullet *bullet){
    if(bullet != nullptr){
        disconnect(bullet, &Bullet::notifyBulletDestroyed, this, &GameCore::onBulletDestroyed);
        m_pPlayer->deleteLater();
        m_pPlayer = nullptr;
    }
}

//! Déconnecte le signal en lien avec l'ennemi et détruit ce dernier
//! \param  enemyDead Booléen pour indiquer si l'ennemi est mort
void GameCore::onEnemyDeath(Enemy *enemy){
    if(m_ennemyWave.contains(enemy)){
        //On récupère l'index de l'ennemi passé en paramètre
        // pour l'utiliser dans une variable pointeur
        Enemy* pEnemy = m_ennemyWave[m_ennemyWave.indexOf(enemy)];
        //On déconnecte le signal et le slot
        disconnect(pEnemy, &Enemy::notifyEnemyDeath, this, &GameCore::onEnemyDeath);
        //On enlève l'ennemi de la liste
        m_ennemyWave.removeAt(m_ennemyWave.indexOf(pEnemy));
        //On le "supprime plus tard" quand on ne fait
        // plus appel à celui-ci
        pEnemy->deleteLater();
        //Le pointeur pointe sur nullptr
        pEnemy = nullptr;

        //Si la vague d'ennemi est vide et que le joueur
        // est en vie on passe à la prochaine vague
        if(m_ennemyWave.length() == 0 && m_pPlayer != nullptr){
            m_compteurWave+= 1;
            manageWaves();
        }
    }
}

//! Efface la vague d'ennemi afin d'éviter que les
//! anciens ennemis soient encore présent dans la nouvelle vague.
void GameCore::clearWave(){
    for(int i = m_ennemyWave.length()-1; i >= 0; i--){
        if(i>=0){
            Enemy* pEnemyClearWave = m_ennemyWave.at(i);
            onEnemyDeath(pEnemyClearWave);
        }
    }
}

//! Traite la pression d'une touche lorsqu'on est dans un menu
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyPressed(int key) {
    emit notifyKeyPressed(key);
    switch (key)  {
    case Qt::Key_W :
        whenKeyUpPressedMenus();
        break;
    case Qt::Key_S :
        whenKeyDownPressedMenus();
        break;
    case Qt::Key_Escape:
        whenKeyEscapePressedMenus();
        break;
    case Qt::Key_Space:
        whenKeySpacePressedMenus();
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
    //Affecte l'objet Player à la variable membre pointeur
    m_pPlayer = new Player;
    //Connecte les signaux du GameCore avec les slots du joueur
    connect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::onKeyPressed);
    connect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::onKeyReleased);
    connect(m_pPlayer, &Player::notifyPlayerDeath, this, &GameCore::onPlayerDeath);
    //Définit la position initiale du joueur sur les scènes
    m_pPlayer->setPos(0,0);//(350, 470);
    //Définit la couche où est le joueur
    m_pPlayer->setZValue(1);
    //On ajoute le joueur sur la scène de jeu
    m_pSceneGame->addSpriteToScene(m_pPlayer);
}

//! Place sur la scène la vague d'ennemis
void GameCore::setupEnemy() {
    for(int i = 0; i < m_ennemyPerWave; i++){
        Enemy* pEnemy = new Enemy;
        connect(pEnemy, &Enemy::notifyEnemyDeath, this, &GameCore::onEnemyDeath);
        //On ajoute l'ennemi à la liste d'ennemi.
        m_ennemyWave.append(pEnemy);
        //On le positionne aléatoirement sur un axe X et Y
        pEnemy->setPos(std::rand() % (SCENE_WIDTH-pEnemy->width()) + 1, std::rand() % (SCENE_HEIGHT-(SCENE_HEIGHT/2)) + 1);
        //On définit la même couche Z pour que le joueur
        // puisse intéragir avec l'ennemi et vice-versa
        pEnemy->setZValue(1);
        //On initialise la cadence de tir de l'ennemi aléatoirement dans une plage de 50 à 100. (1/2s à 1s).
        pEnemy->setTimeBeforeShoot(std::rand() % 100 + 51);
        m_pSceneGame->addSpriteToScene(pEnemy);
        //On crée l'objet qui gérera le tick de l'ennemi
        ManualWalkingHandler* pEnnemyWalkingHandler = new ManualWalkingHandler;
        //On définit l'objet qui gère le tick de l'ennemi
        pEnemy->setTickHandler(pEnnemyWalkingHandler);
        //On définit aléatoirement la direction que prendra l'ennemi
        pEnnemyWalkingHandler->changeWalkingDirection();
    }
}

//! Gère les vagues d'ennemis
void GameCore::manageWaves(){
    qDebug() << "m_ennemyPerWave" << m_ennemyPerWave;
    qDebug() << "m_compteurWave" << m_compteurWave;

    if(m_compteurWave <= m_maxWave){
        qDebug() << "vague n° : " << m_compteurWave;
        m_ennemyPerWave+=2;
        setupEnemy();
    }else if(m_compteurWave > m_maxWave){
        qDebug() << "You win";
        m_pGameCanvas->setCurrentScene(m_pSceneYouWin);
    }
}

//! Quitte le jeu
void GameCore::exitGame(){
    qDebug() << "Quitter";
    exit (EXIT_SUCCESS);
}

//! Crée la scène GameOver et y ajoute différent éléments;
void GameCore::setupSceneGameOver(){
    //Affectation de la scène GameOver
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
    //Affectation de la scène Menu
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
    delete pEnemy;
    pEnemy = nullptr;
#endif
}

//! Crée la scène Control et y ajoute différent éléments
void GameCore::setupSceneControl(){
    //Affectation de la scène Control
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
    //Affectation de la GameScene (zone de jeu)
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

//! Crée la scène YouWin et y ajoute différents éléments
void GameCore::setupSceneYouWin(){
    //Affectation de la scène YouWiN
    m_pSceneYouWin = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Affichage du titre Contrôle
    m_pSceneYouWin->createText(QPointF(SCENE_WIDTH-500,SCENE_HEIGHT/2-200), "You win !", 70);
    // Affichage des différentes options du menu
    m_pYouWinItems[0] = m_pSceneYouWin->createText(QPointF(SCENE_WIDTH-400,SCENE_HEIGHT/2), "Menu", 50, Qt::red);
}

//! Fonction gérant l'appui de la touche W
void GameCore::whenKeyUpPressedMenus(){
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
}

//! Fonction gérant l'appui de la touche S
void GameCore::whenKeyDownPressedMenus(){
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
}

//! Fonction gérant l'appui de la touche Esc
void GameCore::whenKeyEscapePressedMenus(){
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
}

//! Fonction gérant l'appui de la touche Space
void GameCore::whenKeySpacePressedMenus(){
    if (m_pGameCanvas->currentScene() == m_pSceneMenu) {
        switch (m_menuChoosenItem) {
        case 0: //0.Jouer -
            //Affiche la zone de jeu où on s'était arrêté
            if(m_pOldGameScene){
                m_pGameCanvas->setCurrentScene(m_pOldGameScene);
                //Affiche la zone de jeu à la première vague
            }else{
                setupSceneGameScene();
                m_pGameCanvas->setCurrentScene(m_pSceneGame);
                m_compteurWave = 1;
            }
            break;
        case 1: //1. Contrôles - Affiche la scène avec les contrôles du jeu
            m_pGameCanvas->setCurrentScene(m_pSceneControl);
            break;
        case 2: //2. Quitter - Quitte le jeu
            exitGame();
        }
        //Si on est sur le GameOver
    }else if (m_pGameCanvas->currentScene() == m_pSceneGameOver){
        switch(m_gameOverChoosenItem){
        case 0: //0. Jouer - Relance une partie à la première vague
            clearWave();
            setupSceneGameScene();
            m_pGameCanvas->setCurrentScene(m_pSceneGame);
            m_compteurWave = 1;
            m_ennemyPerWave = 0;
            break;
        case 1: //1. Quitter - Retour au menu
            m_pGameCanvas->setCurrentScene(m_pSceneMenu);
        }
    }else if(m_pGameCanvas->currentScene() == m_pSceneYouWin){
            m_pGameCanvas->setCurrentScene(m_pSceneMenu);
            m_compteurWave = 1;
            m_ennemyPerWave = 0;
    }
}

GameCore* GameCore::getGameCore(){
    return this;
}

