/**
  \file
  \brief    Définition de la classe MainFrm.
  \author   JCO
  \date     janvier 2014
  \version  1.0
*/
#include "mainfrm.h"
#include "ui_mainfrm.h"

#include "gamecanvas.h"

//! Construit la fenêtre principale.
MainFrm::MainFrm(QWidget *pParent) : QWidget(pParent), ui(new Ui::MainFrm) {
    ui->setupUi(this);

    m_pGameCanvas = new GameCanvas(ui->grvGame);

    // Pour que la vue adapte sa taille à celle de la fenêtre
    ui->grvGame->setFitToScreenEnabled(true);

    // Pour supprimer les marges de la zone de jeu
    ui->verticalLayout->setContentsMargins(QMargins(0,0,0,0));
}

//! Détruit la fenêtre principale.
MainFrm::~MainFrm() {
    // Effacer le GameCanvas AVANT l'UI, donc avant la destruction de GameView
    delete m_pGameCanvas;
    m_pGameCanvas = nullptr;

    delete ui;
}
