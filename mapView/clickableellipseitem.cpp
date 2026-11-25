#include "clickableellipseitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

ClickableEllipseItem::ClickableEllipseItem(qreal x, qreal y, qreal width, qreal height)
    : QGraphicsEllipseItem(x, y, width, height)
{
    setCursor(Qt::PointingHandCursor);
}

void ClickableEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    emit clicked();
    QGraphicsEllipseItem::mousePressEvent(event);
}
