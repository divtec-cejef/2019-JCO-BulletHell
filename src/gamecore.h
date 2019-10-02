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

class QAbstractAnimation;
class QGraphicsTransform;

class GameCanvas;
class GameScene;
class Sprite;

//! \brief Classe qui gère la logique du jeu.
//!
//! Dans son état actuel, cette classe met en place différents éléments de démonstration
//! des différentes possibilités à disposition pour animer et interagir avec la zone de jeu.
//!
//! \section earth La Terre
//!
//! Démontre une façon (la moins "orientée-objet") de déplacer un sprite, tout en utilisant
//! le mécanisme de mémorisation de données de QGraphicsItem (dont hérite la classe Sprite).
//!
//! Il y a principalement deux autres façons de déplacer un sprite :
//! - Créer une nouvelle classe qui hérite de Sprite, comme démontré avec la roue tournante (\ref mouse_move_detection).
//! - Affecter à une sprite un gestionnaire de tick, comme démontré avec la balle qui rebondit (\ref tennis_ball) ou
//!   l'homme qui marche (\ref walking_man).
//!
//! \section walking_man L'homme qui marche
//!
//! Démontre les fonctionnalités suivantes :
//!
//! - Spécialisation de la classe Sprite avec la classe WalkingMan afin d'y placer la responsabilité
//! de gérer les images d'animation.
//! - Déplacer un sprite au moyen d'un gestionnaire de tick (SpriteTickHandler) en se basant
//!   sur la cadence (ManualWalkingHandler) ou en préprogrammant une séquence d'animation
//!   avec QPropertyAnimation (AutomaticWalkingHandler).
//! - Retrouver parmi tous les sprites présents sur la scène, les sprites d'un type spécifique (reduceWalkingManOpacity()).
//!
//! \section blue_orb La sphère bleue
//!
//! Démontre les fonctionnalités suivantes :
//!
//! - Au moyen de la classe BlueBall, comment spécialiser la classe Sprite pour réagir
//!   aux événements clavier.
//! - Gérer l'ordre d'affichage des éléments graphiques lorsqu'ils sont superposés en utilisant
//!   la méthode Sprite::setZValue(). Le sprite qui a la Z value la plus élevée apparaît devant les autres.
//!
//! \section tennis_ball La balle qui rebondi
//!
//! Démontre les fonctionnalités suivantes :
//!
//! - Attribuer à un sprite un gestionnaire de mouvement (SpriteTickHandler) pour gérer le déplacement
//!   d'un sprite sur chaque tick.
//! - Détecter une collision entre un sprite et d'autres sprites *avant* de déplacer le sprite,
//!   en calculant la boîte d'encombrement (BoundingBox) qui contiendra la balle lorqu'elle
//!   se *sera* déplacée, et de tester si cette surface entre en collision.
//!   Si c'est le cas, un rebond est calculé et la balle est déplacée dans une autre direction.
//!   Tout ce mécanisme est implémenté dans la classe BouncingSpriteHandler, un gestionnaire qui
//!   spécialise la classe SpriteTickHandler.
//!
//!   Donc, contrairement à la détection de collision réalisée pour le marcheur (ManualWalkingHandler),
//!   où la détection est faite *après* le déplacement du marcheur, ici on vérifie si le déplacement
//!   de la balle provoque une collision *avant* de la déplacer.
//!
//! \section mouse_click_detection La balle de foot qui tombe
//!
//! Démontre les fonctionnalités suivantes :
//!
//! - Détecter le clic sur la souris (fonction mouseButtonPressed()).
//! - Créer une animation avec des courbes de mouvement (QEasingCurve), en particulier celle provoquant un rebond (QEasingCurve::OutBounce),
//!   dans la fonction createFootBallClip().
//! - Créer une animation d'un effet de flou (QGraphicsBlurEffect) et appliquer cet effet à un sprite, dans la fonction createVanishAnimation().
//! - Créer une animation de disparition en faisant varier l'opacité d'un sprite, dans la fonction createVanishAnimation().
//! - Combiner différentes animations en les séquençant (QSequentialAnimationGroup) ou en les parallélisant (QParallelAnimationGroup), dans la
//!   fonction createFootBallClip().
//!
//! À noter que le fichier utilities.h met à disposition des fonctions utilitaires permettant de cacher
//! (GameFramework::hideMouseCursor() le curseur de la souris.
//!
//! \section mouse_move_detection La roue tournante
//!
//! Démontre les fonctionnalités suivantes :
//!
//! - Démarrer/arrêter le suivi de la souris avec GameCanvas::startMouseTracking() et
//!   GameCanvas::stopMouseTracking()
//! - Au moyen de la classe SpinningPinwheel, comment spécialiser la classe Sprite pour réagir
//!   aux événements de la souris.
//! - Comment savoir si un sprite est visible ou non (avec SpinningPinwheel::itemChange()), afin
//!   d'éviter de faire des traitements inutiles si un sprite n'est pas visible.
//!
//! \section text Texte simple
//!
//! Démontre comment afficher un texte simple sur une scène, en utilisant la fonction GameScene::createText().
//!
//! \section multi_scene Plusieurs scènes
//!
//! L'appui sur la touche X permet d'alterner la scène visible, en vérifiant quelle scène est actuellement visible (GameCanvas::currentScene())
//! puis on affichant l'autre (GameCanvas::setCurrentScene()). Cela est fait dans la fonction swapScenes().
//!
//! \section others Autres
//! - La touche Q supprime la sphère bleue.
//! - La touche "espace" rend le marcheur de plus en plus transparent.
//! - La touche I affiche des informations de debug concernant le sprite se trouvant sous le curseur de la souris.
//!
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

signals:
    void notifyMouseMoved(QPointF newMousePosition);
    void notifyMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyKeyPressed(int key);
    void notifyKeyReleased(int key);

private:

    void setupWalkingMen();
    void setupBouncingArea();
    void setupBlueBall();
    void setupWalkingMan();
    void setupSpinningWheel();
    void setupCirclingEarth();

    void createAlternativeScene();
    void configureWalking(Sprite* pSprite);
    void reduceWalkingManOpacity();
    void removeBall();
    void swapScenes();
    void displaySpriteDebugInfos();

    void createFootBallClip(QPointF position);
    QAbstractAnimation* createVanishAnimation(Sprite* pSprite);
    void toggleSpinningWheel();

    GameCanvas* m_pGameCanvas;
    GameScene* m_pScene;
    GameScene* m_pAlternativeScene;

    Sprite* m_pBall;
    Sprite* m_pWalkingMan;
    Sprite* m_pSpinningWheel;
    Sprite* m_pEarth;

    QList<Sprite*> m_footballClipSpriteList;

    bool m_footballClipRunning;

private slots:

    void onClipVanishFinished();
};


#endif // GAMECORE_H
