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

    // Affichage des touches actives :
    QString texte = "w : Se déplacer en haut\n"
                    "s : Se déplacer en bas\n"
                    "d : Se déplacer à droite\n"
                    "a : Se déplacer à gauche\n"
                    "space : Tirer\n"
                    "ctrl+shift+i : affiche des informations sur la cadence.";
    m_pScene->createText(QPointF(10,750), texte, 15);

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

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {
    emit notifyKeyPressed(key);
    //BlueBall::onKeyPressed(key);
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);
    //BlueBall::onKeyReleased(key);
}

//! Cadence.
//! Gère le déplacement de la Terre qui tourne en cercle.
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
    m_pEnemy = pEnemy;
}
