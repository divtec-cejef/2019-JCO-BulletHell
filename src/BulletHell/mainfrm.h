#ifndef MAINFRM_H
#define MAINFRM_H

#include <QWidget>

namespace Ui {
class mainfrm;
}

class mainfrm : public QWidget
{
    Q_OBJECT

public:
    explicit mainfrm(QWidget *parent = 0);
    ~mainfrm();

private:
    Ui::mainfrm *ui;
};

#endif // MAINFRM_H
