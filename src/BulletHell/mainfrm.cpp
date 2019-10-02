#include "mainfrm.h"
#include "ui_mainfrm.h"

mainfrm::mainfrm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainfrm)
{
    ui->setupUi(this);
}

mainfrm::~mainfrm()
{
    delete ui;
}
