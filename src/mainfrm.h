/**
  \file
  \brief    Déclaration de la classe MainFrm.
  \author   JCO
  \date     janvier 2014
  \version  1.0
*/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class MainFrm;
}

class GameCanvas;

//! \brief Fenêtre principale.
//!
//! MainFrm représente la fenêtre principale, dans laquelle est placé
//! une instance de GameView.
//! Cette fenêtre gère une instance de GameCanvas, qui est en charge
//! de la cadence du jeu.
class MainFrm : public QWidget
{
    Q_OBJECT

public:
    explicit MainFrm(QWidget* pParent = nullptr);
    ~MainFrm();

private:
    Ui::MainFrm *ui;

    GameCanvas* m_pGameCanvas;
};

#endif // WIDGET_H
