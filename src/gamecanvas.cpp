/**
  \file
  \brief    Déclaration de la classe GameCanvas.
  \author   JCO
  \date     janvier 2015
*/
#include "gamecanvas.h"

#include "gamecore.h"
#include "gamescene.h"
#include "gameview.h"

#include <limits>

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>

const int DEFAULT_TICK_INTERVAL = 20;

//!
//! Construit le canvas de jeu, qui se charge de faire l'interface entre GameView, GameScene et GameCore.
//! \param pView    La vue qui affiche les scènes du jeu.
//! \param pParent  Objet parent.
GameCanvas::GameCanvas(GameView* pView, QObject* pParent) : QObject(pParent) {
    m_pView = pView;
    m_pGameCore = nullptr;
    m_pDetailedInfosItem = nullptr;

    m_keepTicking = false;

    m_tickTimer.setSingleShot(true);
    m_tickTimer.setInterval(DEFAULT_TICK_INTERVAL);
    m_tickTimer.setTimerType(Qt::PreciseTimer); // Important pour avoir un précision suffisante sous Windows
    connect(&m_tickTimer, SIGNAL(timeout()), this, SLOT(onTick()));

    initDetailedInfos();

    QTimer::singleShot(0, this, SLOT(onInit()));
}

//!
//! Destructeur.
//!
GameCanvas::~GameCanvas()
{
    delete m_pGameCore;
    m_pGameCore = nullptr;
}

//! Construit une scène de jeu et lui installe un filtre d'événements pour intercepter les
//! événements clavier et souris.
//! La scène créée n'est pas automatiquement affichée.
//! \see setCurrentScene()
GameScene* GameCanvas::createScene() {
    GameScene* pScene = new GameScene(this);
    pScene->installEventFilter(this);
    return pScene;
}

//! Construit une scène de jeu et lui installe un filtre d'événements pour intercepter les
//! événements clavier et souris.
//! La scène créée n'est pas automatiquement affichée.
//! \see setCurrentScene()
GameScene* GameCanvas::createScene(const QRectF& rSceneRect) {
    GameScene* pScene = new GameScene(rSceneRect, this);
    pScene->installEventFilter(this);
    return pScene;
}

//! Construit une scène de jeu et lui installe un filtre d'événements pour intercepter les
//! événements clavier et souris.
//! La scène créée n'est pas automatiquement affichée.
//! \see setCurrentScene()
GameScene* GameCanvas::createScene(qreal x, qreal y, qreal width, qreal height) {
    GameScene* pScene = new GameScene(x, y, width, height, this);
    pScene->installEventFilter(this);
    return pScene;
}

//! Change la scène de jeu actuellement affichée.
void GameCanvas::setCurrentScene(GameScene* pScene) {
    if (m_pView->scene())
        m_pView->scene()->removeItem(m_pDetailedInfosItem);

    m_pView->setScene(pScene);
    m_pView->scene()->addItem(m_pDetailedInfosItem);
}

//! \return un pointeur sur la scène qui est actuellement affichée par GameView.
GameScene* GameCanvas::currentScene() const {
    return static_cast<GameScene*>(m_pView->scene());
}

//!
//! Démarre la génération d'un tick sur une base de temps régulière,
//! donnée en paramètre.
//! \param tickInterval  Intervalle de temps (en millisecondes) entre chaque tick. Si cette valeur est
//! inférieure à zéro, l'intervalle de temps précédent est utilisé.
//!
void GameCanvas::startTick(int tickInterval)  {
    if (tickInterval != KEEP_PREVIOUS_TICK_INTERVAL)
        m_tickTimer.setInterval(tickInterval);

    m_keepTicking = true;
    m_lastUpdateTime.start();
    m_tickTimer.start();
}

//!
//! Arrête la génération du tick.
//!
void GameCanvas::stopTick()  {
    m_keepTicking = false;
    m_tickTimer.stop();
}

//! Enclenche le suivi du déplacement de la souris.
void GameCanvas::startMouseTracking() {
    m_pView->setMouseTracking(true);
}

//! Déclenche le suivi du déplacement de la souris.
void GameCanvas::stopMouseTracking() {
    m_pView->setMouseTracking(false);
}

//!
//! \return la position actuelle de la souris, dans le système de coordonnées de la scène actuelle.
//!
QPointF GameCanvas::currentMousePosition() const
{
    return m_pView->mapToScene(m_pView->mapFromGlobal(QCursor::pos()));
}

//! Filtre et dispatch les événements intéressant de la scène.
bool GameCanvas::eventFilter(QObject* pObject, QEvent* pEvent)
{
    switch (pEvent->type())  {
    case QEvent::KeyPress:                  this->keyPressed(static_cast<QKeyEvent*>(pEvent));                         return true;
    case QEvent::KeyRelease:                this->keyReleased(static_cast<QKeyEvent *>(pEvent));                       return true;
    case QEvent::GraphicsSceneMouseMove:    this->mouseMoved(static_cast<QGraphicsSceneMouseEvent*>(pEvent));          return true;
    case QEvent::GraphicsSceneMousePress:   this->mouseButtonPressed(static_cast<QGraphicsSceneMouseEvent*>(pEvent));  return true;
    case QEvent::GraphicsSceneMouseRelease: this->mouseButtonReleased(static_cast<QGraphicsSceneMouseEvent*>(pEvent)); return true;
    default : return QObject::eventFilter(pObject, pEvent);
    }
}

//! Initialise l'affichage des informations détaillées.
void GameCanvas::initDetailedInfos()
{
    m_pDetailedInfosItem = new QGraphicsTextItem("");
    m_pDetailedInfosItem->setDefaultTextColor(Qt::white);
    m_pDetailedInfosItem->setPos(0,20);
    m_pDetailedInfosItem->setZValue(std::numeric_limits<qreal>::max()); // Toujours devant les autres items
    m_pDetailedInfosItem->hide();
}

//! Gère l'appui sur une touche du clavier.
//! Les répétitions automatiques sont ignorées.
void GameCanvas::keyPressed(QKeyEvent* pKeyEvent) {
    // Supprimer ce premier test si la répétition de touche doit être signalée.
    if (pKeyEvent->isAutoRepeat())
        pKeyEvent->ignore();
    else {
        m_pGameCore->keyPressed(pKeyEvent->key());

        if (pKeyEvent->modifiers()==(Qt::ShiftModifier|Qt::ControlModifier)) {
            switch (pKeyEvent->key()) {
            case Qt::Key_I:
                if (m_pDetailedInfosItem)
                    m_pDetailedInfosItem->setVisible(!m_pDetailedInfosItem->isVisible());
                break;
            case Qt::Key_P:
                m_tickTimer.setInterval(m_tickTimer.interval()+1);
                qDebug() << "Tick interval set to " << m_tickTimer.interval();
                break;
            case Qt::Key_M:
                m_tickTimer.setInterval(m_tickTimer.interval()-1);
                qDebug() << "Tick interval set to " << m_tickTimer.interval();
                break;
            }
        }
        pKeyEvent->accept();
    }
}

//! Gère le relâchement d'une touche du clavier.
void GameCanvas::keyReleased(QKeyEvent* pKeyEvent) {
    // Supprimer ce premier test si la répétition de touche doit être signalée.
    if (pKeyEvent->isAutoRepeat())
        pKeyEvent->ignore();
    else {
        m_pGameCore->keyReleased(pKeyEvent->key());
        pKeyEvent->accept();
    }
}

//! Gère le déplacement de la souris.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée.
void GameCanvas::mouseMoved(QGraphicsSceneMouseEvent* pMouseEvent) {
    m_pGameCore->mouseMoved(pMouseEvent->scenePos());
}

//! Gère l'événement d'appui sur un bouton de la souris.
//! La conception de cette fonction fait que GameCore n'a pas de moyen de savoir quel
//! bouton a été pressé.
void GameCanvas::mouseButtonPressed(QGraphicsSceneMouseEvent* pMouseEvent) {
    m_pGameCore->mouseButtonPressed(pMouseEvent->scenePos(), pMouseEvent->buttons());
}

//! Gère l'événement de relâchement d'un bouton de la souris.
//! La conception de cette fonction fait que GameCore n'a pas de moyen de savoir quel
//! bouton a été relâché.
void GameCanvas::mouseButtonReleased(QGraphicsSceneMouseEvent* pMouseEvent) {
    m_pGameCore->mouseButtonReleased(pMouseEvent->scenePos(), pMouseEvent->buttons());
}

//! Instancie un objet de type GameCore, en charge de la logique du jeu.
//! L'instanciation est faite uniquement lorsque tous les objets sont en place,
//! afin que GameCore n'appelle pas la fonction startTick alors que le signaux
//! ne sont pas encore connectés.
void GameCanvas::onInit() {
    m_pGameCore = new GameCore(this, this);
}


//! Traite le tick : le temps exact écoulé entre ce tick et le tick précédent
//! est mesuré et l'objet GameCore est lui-même informé du tick.
//! Poursuit la génération du tick si nécessaire.
void GameCanvas::onTick() {
    int elapsedTime = m_lastUpdateTime.elapsed();

    // On évite une division par zéro (peu probable, mais on sait jamais)
    if (elapsedTime < 1)
        elapsedTime = 1;

    m_lastUpdateTime.start();

    m_pGameCore->tick(elapsedTime);
    currentScene()->tick(elapsedTime);

    if (m_pDetailedInfosItem && m_pDetailedInfosItem->isVisible())
        m_pDetailedInfosItem->setPlainText(QString("FPS : %1, Elapsed : %2ms, Tick duration : %3ms")
                                      .arg(1000/elapsedTime)
                                      .arg(elapsedTime)
                                      .arg(m_lastUpdateTime.elapsed()));

    if (m_keepTicking)
        m_tickTimer.start();
}
