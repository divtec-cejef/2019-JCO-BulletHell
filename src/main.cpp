/*! \mainpage Game Framework
 *
 * \author JCO
 * \section intro_sec Introduction
 *
 * Ce projet est une architecture de base en vue de la création d'un jeu simple en 2D.
 * Il est plutôt orienté jeu d'action.
 *
 * Il prend en charge l'organisation des classes nécessaires à l'affichage
 * d'une surface de jeu en deux dimensions.
 *
 * En plus de fournir l'architecture de base, ce projet démontre quelques
 * fonctionnalités rudimentaires :
 *
 * - Déplacement d'un sprite animé (un marcheur) en modifiant sa position x grâce
 *   à QPropertyAnimation (voir \ref walking_man).
 * - Déplacement d'un sprite animé (un marcheur) sur la base d'une cadence (voir \ref walking_man).
 * - Déplacement d'un sprite (une balle) avec les touches du clavier (voir \ref blue_orb).
 * - Recherche d'un sprite en fonction de son type exact et changement de
 *   son opacité (QGameCore::reduceWalkingManOpacity()).
 * - Destruction d'un sprite (GameCore::removeBall()).
 * - Détection de collisions (voir par exemple BouncingSpriteHandler et ManualWalkingHandler).
 * - Utilisation de plusieurs scènes (surfaces de jeu), au moyen de GameCanvas::createScene()
 *   et GameCanvas::setCurrentScene().
 *
 * Ces différentes fonctionnalités sont décrites dans GameCore.
 *
 * Ce projet devra être complètement renommé avec un nom adapté au projet final développé par l'apprenant.
 *
 * \section architecture_sec Architecture
 * La classe MainFrm se charge d'afficher la fenêtre de l'application. Elle utilise le
 * fichier `mainfrm.ui` pour construire l'interface utilisateur.
 * Par défaut, cette interface utilisateur intègre un élément de type GameView
 * qui se charge d'afficher la scene graphique (qui elle est de type GameScene).
 *
 * Au moment de la construction (MainFrm()) d'une instance MainFrm, un cadre
 * de jeu (GameCanvas) est créé.
 * Ce cadre de jeu gère la cadence du jeu (le tick) et délègue toute la logique
 * du jeu à un objet de type GameCore, qu'il crée, et qui doit être codé par l'apprenant.
 *
 * Voici un diagramme de séquence qui montre la séquence de démarrage du jeu et
 * la création des instances principales chargées de mettre en place les éléments de
 * ce jeu.
 *
 * \image html UML_sequence.png "Diagramme de séquence"
 *
 * GameCore est en charge de créer un objet de type GameScene, qui représente la
 * surface du jeu, avec la méthode GameCanvas::createScene() et de
 * l'afficher en appelant la méthode GameCanvas::setCurrentScene() de GameCanvas.
 *
 * Il est possible de créer plusieurs scènes et de spécifier à GameCanvas d'en
 * afficher une plutôt qu'une autre.
 *
 * La plupart des jeux d'action ont besoin d'un timing régulier, permettant de
 * déplacer les sprites, détecter les collisions et analyser l'état du jeu.
 * C'est la cadence du jeu.
 *
 * La classe GameCanvas est capable de générer un tel timing, appelé tick. Chaque fois
 * qu'un tick survient, la méthode GameCore::tick() est automatiquement appelée par GameCanvas.
 * Ensuite, ce tick est propagé aux différents sprites de la scène, afin qu'ils puissent
 * y réagir.
 *
 * Par défaut, le tick survient toutes les 20 ms (environ). L'intégralité du code qui
 * est exécuté à chaque tick ne devrait donc pas durer plus de 20 ms.
 * Avec une cadence de 20 ms, on obtient 50 images par secondes.
 *
 * Les méthodes suivantes permettent de manipuler ce tick :
 * - GameCanvas::startTick() : Cette fonction démarre le timing, ce qui a pour effet
 * d'appeler la fonction GameCore::tick() de façon régulière.
 * - GameCanvas::stopTick() : Cette fonction interrompt le timing. La fonction GameCore::tick() n'est plus appelée.
 *
 * Voici un diagramme de classes simplifié qui offre une vue globale des classes qui
 * compose de projet.
 * \image html UML_classes.png "Diagramme de classe simplifié"
 *
 * \section afaire_sec Travail à réaliser
 * Pour repartir de ce projet afin de réaliser un jeu, il convient tout d'abord de le renommer et de lui donner un nom
 * adapté.
 *
 * Ensuite, développer le jeu au sein de la classe GameCore et en spécialisant la classe Sprite.
 *
 * Les initialisations peuvent être faites dans le constructeur de GameCore : GameCore::GameCore().
 *
 * Si l'initialisation du jeu implique de nombreuses instructions, ajouter à GameCore
 * des fonctions privées d'initialisation qui seront appelées depuis le constructeur.
 *
 * Au sein de la classe GameCore, la variable membre m_pCanvas permet d'accéder
 * aux fonctions de la classe GameCanvas.
 *
 * La classe GameScene, qui représente un espace de jeu en deux dimensions, met
 * à disposition plusieurs fonctions utiles pour le jeu :
 *
 * - GameScene::setWidth() : Permet de déterminer la largeur, en pixels, de la surface de jeu. La fonction GameScene::width() permet de la relire.
 * - GameScene::setHeight() : Permet de déterminer la hauteur, en pixels, de la surface de jeu. La fonction GameScene::height() permet de la relire.
 * - GameScene::addSpriteToScene() : Cette fonction intègre le sprite donné à la scène. La scène en prend la propriété
 *   et se chargera donc de le détruire lorsque l'application se termine.
 * - GameScene::collidingSprites() : Cette fonction permet d'obtenir une liste de sprites en collision avec un sprite donné.
 * - GameScene::spriteAt() : Cette fonction permet de récupérer le sprite se trouvant à la position donnée.
 * - GameScene::createText() : Cette fonction ajoute à la scène un texte et retourne un pointeur permettant de manipuler ce texte.
 *
 * La classe GameCanvas intercepte les événements produits par le clavier (appui et
 * relâche d'une touche, voir \ref input_keyboard) et la souris (voir \ref input_mouse).
 *
 * \section sprite_sec Les objets animés
 * Un jeu est essentiellement constitué d'élément graphiques animés qui se déplacent sur
 * la surface de jeu.
 * Qt met à disposition plusieurs classes et fonctionnalités permettant de gérer de
 * nombreux éléments graphiques. Ces éléments graphiques sont de type QGraphicsItem.
 *
 * Pour simplifier le développement, la classe Sprite spécialise QGraphicsItem et
 * met à disposition quelques fonctionnalités de base telles que des méthodes de
 * détection de collision (Sprite::collidingSprites()) ou d'animation (Sprite::addAnimationFrame(),
 * Sprite::startAnimation() et Sprite::stopAnimation()).
 *
 * \section input Evénements du clavier et de la souris
 *
 * \subsection input_keyboard Clavier
 * Lorsqu'une touche du clavier est appuyée, la méthode GameCore::keyPressed() est
 * automatiquement appelée. Si la touche reste appuyée, cette méthode n'est pas appelée
 * continuellement (ce comportement peut être modifié dans GameCanvas::keyPressed() et
 * GameCanvas::keyReleased()).
 *
 * Lorsque la touche du clavier est relâchée, la méthode GameCore::keyReleased() est automatiquement appelée.
 *
 * Ces deux méthodes reçoivent le paramètre `Key` qui représente le code numérique de la touche appuyée.
 *
 * GameCore se charge d'émettre le signal `notifyKeyPressed()` lorsqu'une touche est appuyée
 * et le signal `notifyKeyReleased()` lorsqu'une touche est relâchée, ce qui permet d'y
 * connecter les objets intéressés par cette information.
 *
 * \subsection input_mouse Souris
 * Lorsqu'un bouton de la souris est appuyé, la méthode GameCore::mouseButtonPressed() est automatiquement appelée. La méthode
 * reçoit alors la position de la souris en paramètre et la liste des boutons appuyés.
 *
 * Lorsqu'un bouton de la souris est relâché, la méthode GameCore::mouseButtonReleased() est automatiquement appelée. La méthode
 * reçoit alors la position de la souris en paramètre et la liste des boutons appuyés.
 *
 * Par défaut, les événements de déplacement de la souris ne sont pas détectés, pour des raisons de performance.
 * Pour les détecter, il faut enclencher le suivi de la souris avec la méthode GameCanvas::startMouseTracking(). Le suivi peut être
 * stoppé avec la méthode GameCanvas::stopMouseTracking().
 *
 * Lorsque le suivi de la souris est enclenché, la méthode GameCore::mouseMoved() est appelée chaque fois que la souris est déplacée.
 *
 * GameCore se charge d'émettre les signaux `notifyMouseButtonPressed()`,
 * `notifyMouseButtonReleased()` et `notifyMouseMoved()`, ce qui permet d'y connecter
 * les objets intéressés par les informations concernant la souris.
 *
 * \section res_sec Ressources
 * Pour fonctionner correctement, le jeu a besoin d'un certain nombre de ressources, telles que des images, des sons,
 * des fichiers de configuration, etc.
 * Ces ressources sont placées dans un répertoire `res`.
 *
 * Le fichier resources.h met à disposition des fonctions utilitaires (dans l'espace de nommage GameFramework) permettant d'accéder
 * à ce répertoire `res`.
 *
 * Toutefois, si une de ces ressources venait à être introuvable, il est important que le programme ne plante pas lamentablement et
 * que l'erreur soit gérée de la façon la plus propre possible.

 * \section images_sec Les images
 * Qt met différentes classes à disposition pour manipuler des images.
 *
 * Pour ce projet, ce sont des images bitmap qui seront utilisées. La classe QPixmap permet de les manipuler facilement.
 *
 * Il est très facile de créer un objet QPixmap à partir d'un fichier (png ou jpeg) : il suffit de passer le chemin du fichier au constructeur :
 *
 *     QPixmap monImage("chemin_du_fichier/image.png");
 *
 * Il est également possible d'instancier une image sur le tas :
 *
 *     QPixmap* pMonImage = new QPixmap("chemin_du_fichier/image.png");
 *
 * \section screen_mode Les modes d'affichage
 * Par défaut, la zone de jeu est affichée au sein d'une fenêtre classique Windows et peut cohabiter avec des composants graphiques classiques
 * comme des boutons, des champs de texte et autres.
 *
 * Il est possible de faire en sorte que le jeu s'affiche en plein écran, selon différents modes :
 *
 * - Mode plein écran fenêtré : la fenêtre de jeu prend toute la surface de l'écran, mais garde sa bordure. L'utilisateur peut modifier sa taille.
 * Pour obtenir ce mode, dans la fonction main(), choisir le ligne de code `w.showMaximized()` et commenter les autres.
 * - Mode plein écran total : la fenêtre de jeu prend toute la surface de l'écran, ses bordures et sa barre de titre disparaissent. Pour obtenir
 * ce mode, dans la fonction main(), choisir la ligne de code `w.showFullScreen()` et commenter les autres.
 * - Mode fenêtré : c'est le mode par défaut : le jeu s'affiche dans une fenêtre classique de windows. Pour obtenir ce mode, dans la fonction
 * main(), choisir la ligne de code `w.show()` et commenter les autres.
 *
 * Indépendamment de ces modes, deux réglages peuvent également être faits :
 *
 * - Faire que la surface de jeu affichée se dimensionne automatiquement en fonction de la taille de l'écran (la taille de le scène n'est pas modifiée).
 * Pour cela, ajouter dans MainFrm::MainFrm() la ligne de code `ui->grvGame->setFitToScreenEnabled(true);`.
 * - Supprimer les marges de l'affichage de la surface de jeu. Pour cela, ajouter dans MainFrm::MainFrm() la ligne de code `ui->verticalLayout->setContentsMargins(QMargins(0,0,0,0));`.
 *
 * \section utilities Les fonctions utilitaires
 * En plus des fonctions utilitaires liées aux resources (\ref res_sec), le fichier utilities.h met à disposition des fonctions
 * utiliaires diverses, en particulier des fonctions permettant de connaître les dimensions de l'écran et le rapport largeur/hauteur.
 *
 * Les fonctions GameFramework::hideMouseCursor() et GameFramework::showMouseCursor() permettent de cacher, respectivement afficher le
 * curseur de la souris.
 *
 */

#include "mainfrm.h"

#include <QApplication>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainFrm w;
    w.show();

    // Pour un mode d'affichage fenêtré, plein écran
    // w.showMaximized();

    // Pour un mode d'affichage non-fenêtré, plein écran
    // w.showFullScreen();

    return a.exec();
}
