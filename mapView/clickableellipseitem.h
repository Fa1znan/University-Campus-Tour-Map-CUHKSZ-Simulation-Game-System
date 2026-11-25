#ifndef CLICKABLEELLIPSEITEM_H
#define CLICKABLEELLIPSEITEM_H

#include <QGraphicsEllipseItem>
#include <QObject>

class ClickableEllipseItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    ClickableEllipseItem(qreal x, qreal y, qreal width, qreal height);

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // CLICKABLEELLIPSEITEM_H
