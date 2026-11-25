#include "customgraphicsview.h"
#include "mapscene.h"
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsTextItem>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , currentScaleFactor(1.0)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing);

    // 设置初始拖动模式为无拖动模式
    setDragMode(QGraphicsView::NoDrag);
    setInteractive(true);

    // 隐藏滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void CustomGraphicsView::setCurrentScaleFactor(double scale)
{
    currentScaleFactor = scale;
    updateTextVisibility();
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    double angle = event->angleDelta().y();
    double factor = qPow(scaleFactorStep, angle / 240.0); // 根据滚轮角度计算缩放因子

    // 更新缩放因子并限制范围
    double newScaleFactor = currentScaleFactor * factor;
    if (newScaleFactor < 0.1 || newScaleFactor > 10.0) {
        return; // 超出范围，不进行缩放
    }

    scale(factor, factor);
    currentScaleFactor = newScaleFactor;

    // 更新文本项的可见性
    updateTextVisibility();

    event->accept();
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // 按下左键时，设置为拖动模式
        setDragMode(QGraphicsView::ScrollHandDrag);
        QGraphicsView::mousePressEvent(event);
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QGraphicsView::mouseReleaseEvent(event);
        // 松开左键后，恢复为无拖动模式
        setDragMode(QGraphicsView::NoDrag);
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void CustomGraphicsView::updateTextVisibility()
{
    bool showText = currentScaleFactor > 3.0; // 调整阈值

    MapScene* mapScene = qobject_cast<MapScene*>(scene());
    if (mapScene) {
        const QList<QGraphicsTextItem*>& textItems = mapScene->getTextItems();
        for (QGraphicsTextItem* textItem : textItems) {
            textItem->setVisible(showText);
        }
    }
}
