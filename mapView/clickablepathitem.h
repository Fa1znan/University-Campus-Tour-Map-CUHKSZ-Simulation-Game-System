#ifndef CLICKABLEPATHITEM_H
#define CLICKABLEPATHITEM_H

#include <QGraphicsPathItem>
#include <QObject>
#include <QCursor>

class ClickablePathItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    explicit ClickablePathItem(QGraphicsItem *parent = nullptr);

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CLICKABLEPATHITEM_H
