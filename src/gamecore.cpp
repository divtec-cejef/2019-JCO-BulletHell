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

#include "automaticwalkinghandler.h"
#include "blueball.h"
#include "bouncingspritehandler.h"
#include "gamescene.h"
#include "gamecanvas.h"
#include "manualwalkinghandler.h"
#include "resources.h"
#include "spinningpinwheel.h"
#include "sprite.h"
#include "utilities.h"
#include "walkingman.h"

const int SCENE_WIDTH = 1280;
const int SCENE_HEIGHT = 1080;

const QPointF BOUNCING_AREA_POS(700,300);
const int BOUNCING_AREA_SIZE = 20;

enum EarthSpriteDataKey {
    EARTH_ANGLE_KEY = 0,
    EARTH_CENTER_KEY,
    EARTH_RADIUS_KEY
};

const float EARTH_SPEED = 2; // radian/seconde

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {
    
    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;
    
    // Initialisation des variables membres de la classe.
    m_footballClipRunning = false;
    
    m_pBall = nullptr;
    m_pWalkingMan = nullptr;
    m_pEarth = nullptr;
    
    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pScene);
    
    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));
    
    // Créé une scène supplémentaire, pour l'instant invisible.
    createAlternativeScene();
    
    // Création des marcheurs
    setupWalkingMen();

    // Création de la zone de rebond
    setupBouncingArea();

    // Création de la balle
    setupBlueBall();

    // Création du walkin man
    setupWalkingMan();

    // Création de la spinning wheel
    setupSpinningWheel();

    // Création de la terre tournante
    setupCirclingEarth();

    // Création des murs
    Sprite* pRightWall = new Sprite(GameFramework::imagesPath() + "brique.jpg");
    pRightWall->setScale(0.5);
    pRightWall->setPos(400, 300);
    
    Sprite* pLeftWall = new Sprite(GameFramework::imagesPath() + "brique.jpg");
    pLeftWall->setScale(0.5);
    pLeftWall->setPos(-250, 300);

    m_pScene->addSpriteToScene(pRightWall);
    m_pScene->addSpriteToScene(pLeftWall);

    // Affichage d'un texte
    QGraphicsSimpleTextItem* pText = m_pScene->createText(QPointF(300,600), "L'homme qui marche", 70);
    pText->setOpacity(0.5);

    // Affichage des touches actives :
    QString texte = "<space> : change l'opacité de l'homme qui marche\n"
                    "q : supprime la balle bleue\n"
                    "x : change de scène\n"
                    "m : démarre le suivi de souris\n"
                    "flèches : déplace la balle bleue\n"
                    "clic gauche : animation de balle\n"
                    "i : affiche dans la console des informations sur le sprite sous la souris\n"
                    "ctrl+shift+i : affiche des informations sur la cadence.";
    m_pScene->createText(QPointF(10,600), texte, 10);

    
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
    
    delete m_pAlternativeScene;
    m_pAlternativeScene = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {
    emit notifyKeyPressed(key);

    switch (key)  {
    case Qt::Key_Space:
        reduceWalkingManOpacity();
        break;
    case Qt::Key_Q:
        removeBall();
        break;
    case Qt::Key_X:
        swapScenes();
        break;
    case Qt::Key_I:
        displaySpriteDebugInfos();
        break;
    case Qt::Key_M:
        toggleSpinningWheel();
    }
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);

}

//! Cadence.
//! Gère le déplacement de la Terre qui tourne en cercle.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(int elapsedTimeInMilliseconds) {
    Q_ASSERT(m_pEarth != nullptr);

    float radian = EARTH_SPEED * elapsedTimeInMilliseconds / 1000.F;

    float newangle = m_pEarth->data(EARTH_ANGLE_KEY).toFloat() + radian;
    float radius = m_pEarth->data(EARTH_RADIUS_KEY).toFloat();
    QPointF newPos(std::cos(newangle)*radius, std::sin(newangle)*radius);

    m_pEarth->setPos(m_pEarth->data(EARTH_CENTER_KEY).toPointF() + newPos);
    m_pEarth->setData(EARTH_ANGLE_KEY, newangle);
}

//! Met en place la démo des deux marcheurs.
//! Un marcheur (WalkingMan) hérite de Sprite afin d'y placer la responsabilité
//! de gérer les images d'animation.
//! Le déplacement se fait au moyen d'un gestionnaire de tick (SpriteTickHandler).
//! Deux gestionnaires de tick ont été créés :
//! - ManualWalkingHandler : déplacement classique en se basant sur la cadence.
//! - AutomatiqueWalkingHandler : déplacement en préprogrammant une séquence d'animation
//!   avec QPropertyAnimation.
void GameCore::setupWalkingMen() {
    WalkingMan* pAutoWalkingMan = new WalkingMan;
    pAutoWalkingMan->setPos(10, 100);
    m_pScene->addSpriteToScene(pAutoWalkingMan);
    pAutoWalkingMan->setTickHandler(new AutomaticWalkingHandler);

    WalkingMan* pManualWalkingMan = new WalkingMan;
    pManualWalkingMan ->setPos(30,300);
    m_pScene->addSpriteToScene(pManualWalkingMan );
    pManualWalkingMan->setTickHandler(new ManualWalkingHandler);
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

    // Si le bouton gauche est cliqué et que le clip n'a pas déjà démarré,
    // on démarre le clip de la balle qui tombe et rebondit sur le sol.
    if (buttons.testFlag(Qt::LeftButton) && !m_footballClipRunning) {
        m_footballClipRunning = true;
        createFootBallClip(mousePosition);
    }
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}

//! Construit la zone de rebond, ainsi que la balle de tennis qui va s'y déplacer.
void GameCore::setupBouncingArea() {
    
    // Création des briques de délimitation de la zone et placement
    QPixmap SmallBrick(GameFramework::imagesPath() + "brique_small.png");
    SmallBrick = SmallBrick.scaled(15,15);
    for (int Row = 0; Row < BOUNCING_AREA_SIZE; ++Row)  {
        for (int Col = 0; Col < BOUNCING_AREA_SIZE; ++Col)  {
            if (Row == 0 || Row == BOUNCING_AREA_SIZE-1 || Col == 0 || Col == BOUNCING_AREA_SIZE-1)  {
                Sprite* pSprite = new Sprite(SmallBrick);
                pSprite->setPos(QPointF(Col * SmallBrick.width(), Row * SmallBrick.height())+ BOUNCING_AREA_POS);
                m_pScene->addSpriteToScene(pSprite);
            }
        }
    }
    
    // Création de la balle de tennis qui rebondi
    Sprite* pTennisBall = new Sprite(GameFramework::imagesPath() + "tennisball.png");
    pTennisBall->setTickHandler(new BouncingSpriteHandler);
    pTennisBall->setPos(BOUNCING_AREA_POS + QPointF(25,50));
    m_pScene->addSpriteToScene(pTennisBall);
}

//! Met en place la démo de la balle bleue.
void GameCore::setupBlueBall() {
    BlueBall* pBall = new BlueBall;
    pBall->setPos(0, 470);
    pBall->setZValue(1);          // Passe devant tous les autres sprites
    m_pScene->addSpriteToScene(pBall);
    connect(this, &GameCore::notifyKeyPressed, pBall, &BlueBall::onKeyPressed);
    connect(this, &GameCore::notifyKeyReleased, pBall, &BlueBall::onKeyReleased);
    m_pBall = pBall;
}

//! Met en place la démo du Walking Man
void GameCore::setupWalkingMan() {
    WalkingMan* pWLKM = new WalkingMan;
    pWLKM->setPos(0, 470);
    pWLKM->setZValue(1);          // Passe devant tous les autres sprites
    m_pScene->addSpriteToScene(pWLKM);
    connect(this, &GameCore::notifyKeyPressed, pWLKM, &WalkingMan::onKeyPressed);
    connect(this, &GameCore::notifyKeyReleased, pWLKM, &WalkingMan::onKeyReleased);
    m_pWalkingMan = pWLKM;
}

//! Met en place la roue tournante.
void GameCore::setupSpinningWheel() {
    SpinningPinwheel* pSpinningWheel = new SpinningPinwheel;
    pSpinningWheel->setVisible(false);
    m_pScene->addSpriteToScene(pSpinningWheel);
    connect(this, &GameCore::notifyMouseMoved, pSpinningWheel, &SpinningPinwheel::onMouseMove);
    m_pSpinningWheel = pSpinningWheel;
}

//! Met en place la Terre qui tourne en cercle.
//! Démontre comment un sprite peut être mis en mouvement en le déplaçant dans
//! la fonction tick().
//! Démontre également comment mémoriser des données spécifiques à un sprite
//! en utilisant QGraphicsItem::setData() et QGraphicsItem::data().
void GameCore::setupCirclingEarth() {
    QImage earthImage(GameFramework::imagesPath() + "earth.png");

    Sprite* pEarth = new Sprite(QPixmap::fromImage(earthImage.scaledToHeight(50,
                                                   Qt::SmoothTransformation)));
    pEarth->setData(EARTH_ANGLE_KEY, 0);
    pEarth->setData(EARTH_CENTER_KEY, QPointF(100,100));
    pEarth->setData(EARTH_RADIUS_KEY, 50);
    m_pScene->addSpriteToScene(pEarth);
    m_pEarth = pEarth;
}

//! Construit une scène alternative qui contiendra un élément SVG.
void GameCore::createAlternativeScene() {
    // Création de la scène alternative
    m_pAlternativeScene = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    
    // Création d'un élément affichange une image SVG (image verctorielle, incluant une animation)
    QGraphicsSvgItem* pSvgItem = new QGraphicsSvgItem(GameFramework::imagesPath() + "bubbles.svg");
    QRectF ItemRect = pSvgItem->boundingRect();
    
    // Ajoute l'image à la scène et la positionne au centre
    m_pAlternativeScene->addItem(pSvgItem);
    pSvgItem->setPos(m_pAlternativeScene->sceneRect().center().x() - ItemRect.width()/2, m_pAlternativeScene->sceneRect().center().y() - ItemRect.height()/2);
}

//! Réduit l'opacité du marcheur.
//! Si celle-ci devient négative, le marcheur redevient opaque.
//! Cette fonction démontre la façon de retrouver des sprites
//! d'une classe spécifique.
//! Pour que qobject_cast fonctionne, la classe dérivée
//! recherchée *doit* implémenter la macro Q_OBJECT.
void GameCore::reduceWalkingManOpacity()  {
    for(Sprite* pSprite : m_pScene->sprites()) {
        WalkingMan* pWalkingMan = qobject_cast<WalkingMan*>(pSprite);
        if (pWalkingMan) {
            double opacity = pWalkingMan->opacity() - 0.1;
            if (opacity < 0) opacity = 1;

            pWalkingMan->setOpacity(opacity);
        }
    }
}

//! Détruit la balle et affiche un texte qui indique que cela a été fait.
void GameCore::removeBall()  {
    if (!m_pBall)
        return;
    
    delete m_pBall;
    m_pBall = nullptr;
    m_pScene->createText(QPointF(40,40), "La balle est détruite");
}

//! Echange la scène affichée avec l'autre scène.
void GameCore::swapScenes() {
    if (m_pGameCanvas->currentScene() == m_pScene)
        m_pGameCanvas->setCurrentScene(m_pAlternativeScene);
    else
        m_pGameCanvas->setCurrentScene(m_pScene);
}

//! Affiche en tant qu'informations de déboguage des informations sur le sprite
//! pointé par la souris.
void GameCore::displaySpriteDebugInfos() {
    QPointF MousePos(m_pGameCanvas->currentMousePosition());
    qDebug() << "Current mouse pos : " << MousePos;
    Sprite* pHoverSprite = m_pGameCanvas->currentScene()->spriteAt(MousePos);
    if (pHoverSprite) {
        qDebug() << "Sprite class : " << pHoverSprite->metaObject()->className();
        qDebug() << "Pos : " << pHoverSprite->pos();
        qDebug() << "X, Y : " << pHoverSprite->x() << pHoverSprite->y();
    }
}

//! Créé le petit clip de la balle de football, qui démontre l'utilisation d'une animation
//! (QPropertyAnimation) avec courbes de vitesse, ainsi que le Z-Order.
//! \param position  Position où le clip doit être créé.
void GameCore::createFootBallClip(QPointF position) {
    // Créé la balle de foot
    Sprite* pFootBall = new Sprite(GameFramework::imagesPath() + "football.png");
    pFootBall->setScale(0.1);
    pFootBall->setZValue(0.2);
    
    // Créé l'herbe
    Sprite* pGrass = new Sprite(GameFramework::imagesPath() + "grass.png");
    pGrass->setScale(0.1);
    pGrass->setZValue(0.4);
    
    // Met en place le clip
    QRectF ClipRect(position, QSizeF(pGrass->width(), pGrass->width()*2));
    
    QPixmap SkyPixmap(static_cast<int>(ClipRect.width()), static_cast<int>(ClipRect.height()));
    SkyPixmap.fill(Qt::cyan);
    Sprite* pSky = new Sprite(SkyPixmap);
    pSky->setPos(ClipRect.topLeft());

    QPointF BallStartPos(ClipRect.center().x() - pFootBall->width()/2, ClipRect.top());
    
    pFootBall->setPos(BallStartPos);
    pGrass->setPos(position.x(), ClipRect.bottom() - pGrass->height()/2);
    
    m_footballClipSpriteList << pFootBall << pGrass << pSky;
    
    QPropertyAnimation* pBallFallDownAnimation = new QPropertyAnimation(pFootBall, "y"); // La propriété à animer est y (la position y)
    pBallFallDownAnimation->setDuration(1500);
    pBallFallDownAnimation->setStartValue(BallStartPos.y());
    pBallFallDownAnimation->setEndValue(BallStartPos.y() + ClipRect.height() - pFootBall->height());
    pBallFallDownAnimation->setEasingCurve(QEasingCurve::OutBounce);
    
    QParallelAnimationGroup* pAllVanishAnimation = new QParallelAnimationGroup;
    pAllVanishAnimation->addAnimation(createVanishAnimation(pFootBall));
    pAllVanishAnimation->addAnimation(createVanishAnimation(pGrass));
    pAllVanishAnimation->addAnimation(createVanishAnimation(pSky));
    
    QSequentialAnimationGroup* pClipAnimation = new QSequentialAnimationGroup;
    pClipAnimation->addAnimation(pBallFallDownAnimation);
    pClipAnimation->addAnimation(pAllVanishAnimation);
    
    m_pScene->addSpriteToScene(pFootBall);
    m_pScene->addSpriteToScene(pGrass);
    m_pScene->addSpriteToScene(pSky);

    connect(pClipAnimation, SIGNAL(finished()), this, SLOT(onClipVanishFinished()));
    
    pClipAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

//! Construit une animation qui fait disparaitre le sprite donné, en jouant
//! sur son opacité et un flou de plus en plus marqué.
QAbstractAnimation* GameCore::createVanishAnimation(Sprite* pSprite) {
    
    QGraphicsBlurEffect* pBlurEffect = new QGraphicsBlurEffect;
    pSprite->setGraphicsEffect(pBlurEffect);
    pBlurEffect->setBlurRadius(0.);
    QPropertyAnimation* pClipVanishBlurAnimation = new QPropertyAnimation(pBlurEffect, "blurRadius");
    pClipVanishBlurAnimation->setDuration(1000);
    pClipVanishBlurAnimation->setStartValue(0.);
    pClipVanishBlurAnimation->setEndValue(10);
    
    QPropertyAnimation* pClipVanishOpacityAnimation = new QPropertyAnimation(pSprite, "opacity");
    pClipVanishOpacityAnimation->setDuration(1000);
    pClipVanishOpacityAnimation->setStartValue(1.);
    pClipVanishOpacityAnimation->setEndValue(0.);
    
    QParallelAnimationGroup* pClipVanishAnimation = new QParallelAnimationGroup;
    pClipVanishAnimation->addAnimation(pClipVanishBlurAnimation);
    pClipVanishAnimation->addAnimation(pClipVanishOpacityAnimation);
    
    return pClipVanishAnimation;
}

//! Montre ou cache la roue tournant qui suit la souris.
//! Si elle est cachée, le suivi de la souris est stoppé (GameCanvas::stopMouseTracking(),
//! afin de limiter l'impact sur les performances.
//! Si elle est montrée, le suivi de la souris est enclenché (GameCanvas::startMouseTracking())
//! et un signal de suivi de souris est émis, afin de pouvoir mettra à jour la position
//! de la roue tournante par rapport à la position actuelle de la souris.
void GameCore::toggleSpinningWheel() {
    bool isSpinningWheelVisible = m_pSpinningWheel->isVisible();

    if (isSpinningWheelVisible) {
        m_pSpinningWheel->setVisible(false);
        m_pGameCanvas->stopMouseTracking();
    } else {
        m_pSpinningWheel->setVisible(true);
        m_pGameCanvas->startMouseTracking();
        emit notifyMouseMoved(m_pGameCanvas->currentMousePosition());
    }
}

//! Le clip de la balle de foot qui tombe est terminé : les sprites sont effacés.
void GameCore::onClipVanishFinished() {
    qDeleteAll(m_footballClipSpriteList);
    m_footballClipSpriteList.clear();
    m_footballClipRunning = false;
}
