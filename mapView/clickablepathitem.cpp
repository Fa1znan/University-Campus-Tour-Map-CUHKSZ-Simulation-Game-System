#include "clickablepathitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

ClickablePathItem::ClickablePathItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    setCursor(Qt::PointingHandCursor);  // 在设置光标为抓手形状
}

void ClickablePathItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
    QGraphicsPathItem::mousePressEvent(event);
}
