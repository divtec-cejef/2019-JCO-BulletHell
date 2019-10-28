/**
  \file
  \brief    Déclaration de la classe item.
  \author   Thibaud Nussbaumer
  \date    octobre 2019
*/
#ifndef ITEM_H
#define ITEM_H

#include "sprite.h"

class Item : public Sprite
{
    // Nécessaire de déclarer cette macro pour que la classe puisse
    // bénéficier du mécanisme de signaux/slots.
    Q_OBJECT
public:
    Item(const QPixmap& rPixmap, QGraphicsItem* pParent = nullptr);
    Item(QGraphicsItem* pParent = nullptr);
    virtual void tick(int elapsedTimeInMilliseconds);

    virtual void updateVelocity(int x, int y) = 0;

protected:
    virtual void configureAnimation() = 0;
    virtual void updateAnimationState() = 0;

private:
    QPointF m_itemVelocity;
};

#endif // ITEM_H
