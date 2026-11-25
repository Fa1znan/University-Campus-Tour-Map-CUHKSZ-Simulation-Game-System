#include "mapscene.h"
#include <QFile>
#include <QPen>
#include <QGraphicsPathItem>
#include <QDebug>
#include <QFont>
#include <QTimer>
#include "clickableellipseitem.h"
#include "clickablepathitem.h"


MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_minX(0)
    , m_minY(0)
    , m_firstLoad(true)
    , m_dotTimer(new QTimer(this))
    , m_moveStep(50.0)
    , m_maxVelocity(15.0)      // 最大速度
    , m_acceleration(2.0)      // 加速度
    , m_deceleration(1.0)      // 减速度
{
    // 初始化蓝色的点
    auto createDot = [this](qint64 lineId) {
        auto dot = new ClickableEllipseItem(-1, -1, 2, 2);
        dot->setBrush(Qt::blue);
        dot->setPen(Qt::NoPen);
        dot->setZValue(10);
        addItem(dot);

        dot->setFlag(QGraphicsItem::ItemIsSelectable, true);
        dot->setFlag(QGraphicsItem::ItemIsFocusable, true);
        dot->setAcceptedMouseButtons(Qt::LeftButton);

        // 根据线路ID连接到对应的槽函数
        if (lineId == 732726571) {
            connect(dot, &ClickableEllipseItem::clicked, this, &MapScene::onBlueDot1Clicked);
        } else if (lineId == 732726588) {
            connect(dot, &ClickableEllipseItem::clicked, this, &MapScene::onBlueDot2Clicked);
        } else if (lineId == 544022523) {
            connect(dot, &ClickableEllipseItem::clicked, this, &MapScene::onBlueDot3Clicked);
        } else if (lineId == 732726583) {
            connect(dot, &ClickableEllipseItem::clicked, this, &MapScene::onBlueDot4Clicked);
        } else if (lineId == 330013797) {
            connect(dot, &ClickableEllipseItem::clicked, this, &MapScene::onBlueDot5Clicked);
        }

        return dot;
    };

    // 为五条线路创建点信息结构
    DotInfo dot1 = {createDot(732726571), QVector<QPointF>(), 0.0, true};
    DotInfo dot2 = {createDot(732726588), QVector<QPointF>(), 0.0, true};
    DotInfo dot3 = {createDot(544022523), QVector<QPointF>(), 0.0, true};
    DotInfo dot4 = {createDot(732726583), QVector<QPointF>(), 0.0, true};
    DotInfo dot5 = {createDot(330013797), QVector<QPointF>(), 0.0, true};

    m_dots.insert(732726571, dot1);
    m_dots.insert(732726588, dot2);
    m_dots.insert(544022523, dot3);
    m_dots.insert(732726583, dot4);
    m_dots.insert(330013797, dot5);

    // 初始化红色控制点
    m_controlPoint = new ClickableEllipseItem(-1, -1, 2, 2);
    m_controlPoint->setBrush(Qt::red);
    m_controlPoint->setPen(Qt::NoPen);
    m_controlPoint->setZValue(20);
    addItem(m_controlPoint);

    m_controlPoint->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_controlPoint->setFlag(QGraphicsItem::ItemIsFocusable, true);
    m_controlPoint->setAcceptedMouseButtons(Qt::LeftButton);

    // 连接红点的点击信号
    connect(m_controlPoint, &ClickableEllipseItem::clicked, this, &MapScene::onRedDotClicked);

    // 连接定时器到更新位置的槽函数
    connect(m_dotTimer, &QTimer::timeout, this, &MapScene::updateDotPosition);
    m_dotTimer->start(50);

    // 控制红色点移动
    m_controlPointVelocity = QPointF(0, 0);
    // 添加一个新的定时器连接用于平滑移动
    QTimer* moveTimer = new QTimer(this);
    moveTimer->setInterval(16);  // 约60fps
    connect(moveTimer, &QTimer::timeout, this, &MapScene::updateControlPoint);
    moveTimer->start();
    controlPointPos = m_controlPoint->pos();



}

void MapScene::loadGeoJson(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        return;
    }

    QJsonObject root = doc.object();
    QJsonArray features = root["features"].toArray();

    // 用于计算边界的变量
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    // 计算边界
    for (const QJsonValue &featureVal : features) {
        QJsonObject feature = featureVal.toObject();
        QJsonObject geometry = feature["geometry"].toObject();
        QString type = geometry["type"].toString();
        QJsonArray coordinates;

        if (type == "MultiPolygon") {
            coordinates = geometry["coordinates"].toArray();
            for (int i = 0; i < coordinates.size(); ++i) {
                QJsonArray polygon = coordinates[i].toArray();
                for (int j = 0; j < polygon.size(); ++j) {
                    QJsonArray ring = polygon[j].toArray();
                    for (int k = 0; k < ring.size(); ++k) {
                        QJsonArray point = ring[k].toArray();
                        double x = point[0].toDouble();
                        double y = point[1].toDouble();
                        minX = qMin(minX, x);
                        minY = qMin(minY, y);
                        maxX = qMax(maxX, x);
                        maxY = qMax(maxY, y);
                    }
                }
            }
        } else if (type == "MultiLineString") {
            coordinates = geometry["coordinates"].toArray();
            for (int i = 0; i < coordinates.size(); ++i) {
                QJsonArray line = coordinates[i].toArray();
                for (int j = 0; j < line.size(); ++j) {
                    QJsonArray point = line[j].toArray();
                    double x = point[0].toDouble();
                    double y = point[1].toDouble();
                    minX = qMin(minX, x);
                    minY = qMin(minY, y);
                    maxX = qMax(maxX, x);
                    maxY = qMax(maxY, y);
                }
            }
        }
    }

    // 更新或设置最小值
    if (m_firstLoad) {
        m_minX = minX;
        m_minY = minY;
        m_firstLoad = false;
    } else {
        m_minX = qMin(m_minX, minX);
        m_minY = qMin(m_minY, minY);
    }

    // 绘制所有特征
    for (const QJsonValue &featureVal : features) {
        drawFeature(featureVal.toObject());
    }

    updateSceneBounds();

    // 当确认所有数据加载完成后，调用生成路径的函数
    if (filePath.contains("1.geojson")) {
        QRectF bounds = itemsBoundingRect();
        QPointF center = bounds.center();
        m_controlPoint->setPos(center);
    }
}

void MapScene::drawFeature(const QJsonObject& feature)
{
    QJsonObject geometry = feature["geometry"].toObject();
    QString type = geometry["type"].toString();
    QJsonObject properties = feature["properties"].toObject();
    QString name = properties["name"].toString();
    qint64 osm_id = properties["osm_id"].toVariant().toLongLong();
    QString fclass = properties["fclass"].toString();

    if (type == "MultiLineString") {
        QPen pen;
        // 根据实际数据中的道路类型设置样式
        if (fclass == "motorway") {
            // 高速公路
            pen = QPen(QColor(255, 128, 128), 1.2);
        } else if (fclass == "primary" || fclass == "primary_link") {
            // 主干道
            pen = QPen(QColor(255, 170, 170), 1.2);
        } else if (fclass == "secondary") {
            // 次干道
            pen = QPen(QColor(255, 200, 200), 1.0);
        } else if (fclass == "tertiary") {
            // 三级道路
            pen = QPen(QColor(220, 220, 220), 0.8);
        } else if (fclass == "unclassified") {
            // 未分类道路
            pen = QPen(QColor(200, 200, 200), 0.6);
        } else if (fclass == "service") {
            // 服务道路
            pen = QPen(QColor(210, 210, 210), 0.5);
        } else if (fclass == "cycleway") {
            // 自行车道
            pen = QPen(QColor(190, 210, 190), 0.5, Qt::DashLine);
        } else if (fclass == "path") {
            // 人行道
            pen = QPen(QColor(200, 200, 200), 0.3, Qt::DashLine);
        } else {
            // 其他道路类型
            pen = QPen(Qt::black, 0.5);
        }

        // 如果是隧道，使用虚线
        if (properties["tunnel"].toString() == "T") {
            pen.setStyle(Qt::DashLine);
        }

        // 如果是桥梁，加粗线条
        if (properties["bridge"].toString() == "T") {
            pen.setWidth(pen.width() * 1.5);
        }

        drawMultiLineString(geometry["coordinates"].toArray(), pen, name, osm_id);
    }
    else if (type == "MultiPolygon") {
        // 保持原有的建筑物处理逻辑不变
        bool isSpecialBuilding = (osm_id == 675768172 || osm_id == 556004689 ||
                                  osm_id == 556004687 || osm_id == 544063809 ||
                                  osm_id == 544063812 || osm_id == 475871696);
        QBrush brush(isSpecialBuilding ? Qt::yellow : QColor(181, 187, 188));
        QPen pen(Qt::black, 0.5);
        drawMultiPolygon(geometry["coordinates"].toArray(), brush, pen, name, osm_id);
    }
}

void MapScene::drawMultiLineString(const QJsonArray& coordinates, const QPen& pen, const QString& name, qint64 osm_id)
{
    QVector<QPointF> linePoints; // 用于保存该线的所有坐标点

    for (const QJsonValue &lineVal : coordinates) {
        QPainterPath path;
        QJsonArray points = lineVal.toArray();

        if (!points.isEmpty()) {
            QJsonArray start = points[0].toArray();
            double x = (start[0].toDouble() - m_minX) * 10000;
            double y = -(start[1].toDouble() - m_minY) * 10000;
            path.moveTo(x, y);
            linePoints.append(QPointF(x, y));

            for (int i = 1; i < points.size(); ++i) {
                QJsonArray point = points[i].toArray();
                x = (point[0].toDouble() - m_minX) * 10000;
                y = -(point[1].toDouble() - m_minY) * 10000;
                path.lineTo(x, y);
                linePoints.append(QPointF(x, y));
            }

            addPath(path, pen);

            // 添加名称标签
            if (!name.isEmpty()) {
                QPointF midPoint = linePoints.at(linePoints.size() / 2);
                QGraphicsTextItem* textItem = addText(name);
                textItem->setDefaultTextColor(QColor(0, 0, 147));
                textItem->setPos(midPoint);

                QFont font = textItem->font();
                font.setPointSize(8);
                textItem->setFont(font);

                // 设置文字的 Z 值比线路高
                textItem->setZValue(5);  // 确保文字在线的上方

                textItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
                m_textItems.append(textItem);
            }
        }
    }

    if (!linePoints.isEmpty()) {
        m_linePointsMap.insert(osm_id, linePoints);

        // 如果是目标线路，检查是否所有目标线路都已加载
        if (m_dots.contains(osm_id)) {
            bool allLinesLoaded = true;
            for (auto it = m_dots.begin(); it != m_dots.end(); ++it) {
                if (!m_linePointsMap.contains(it.key())) {
                    allLinesLoaded = false;
                    break;
                }
            }

            // 只有当所有线路都加载完成后，才生成路径
            if (allLinesLoaded) {
                generateDotPath();
            }
        }
    }
}

void MapScene::drawMultiPolygon(const QJsonArray &coordinates, const QBrush &brush, const QPen &pen, const QString &name, qint64 osm_id)
{
    for (const QJsonValue &polyVal : coordinates) {
        QPainterPath path;
        QJsonArray rings = polyVal.toArray();

        if (!rings.isEmpty()) {
            QJsonArray exteriorRing = rings[0].toArray();
            QVector<QPointF> pointList; // 用于计算多边形的中心点

            if (!exteriorRing.isEmpty()) {
                QJsonArray start = exteriorRing[0].toArray();
                double x = (start[0].toDouble() - m_minX) * 10000;
                double y = -(start[1].toDouble() - m_minY) * 10000;
                path.moveTo(x, y);
                pointList.append(QPointF(x, y));

                for (int i = 1; i < exteriorRing.size(); ++i) {
                    QJsonArray point = exteriorRing[i].toArray();
                    x = (point[0].toDouble() - m_minX) * 10000;
                    y = -(point[1].toDouble() - m_minY) * 10000;
                    path.lineTo(x, y);
                    pointList.append(QPointF(x, y));
                }
                path.closeSubpath();

                // 创建并添加路径项
                if (osm_id == 675768172 || osm_id == 556004689 ||
                    osm_id == 556004687 || osm_id == 544063809 ||
                    osm_id == 544063812 || osm_id == 475871696) {
                    // 对于特殊建筑使用可点击的路径项
                    ClickablePathItem* clickableItem = new ClickablePathItem();
                    clickableItem->setPath(path);
                    clickableItem->setPen(pen);
                    clickableItem->setBrush(brush);
                    clickableItem->setCursor(Qt::PointingHandCursor);
                    clickableItem->setData(0, osm_id);
                    clickableItem->setAcceptHoverEvents(true);

                    // 连接点击信号
                    connect(clickableItem, &ClickablePathItem::clicked, this, &MapScene::onBuildingClicked);

                    addItem(clickableItem);
                    m_clickableBuildings.append(clickableItem);
                } else {
                    // 对于普通建筑使用普通路径项
                    addPath(path, pen, brush);
                }

                // 添加名称标签
                if (!name.isEmpty()) {
                    // 计算多边形的中心点
                    double sumX = 0;
                    double sumY = 0;
                    for (const QPointF& pt : pointList) {
                        sumX += pt.x();
                        sumY += pt.y();
                    }
                    QPointF centroid(sumX / pointList.size(), sumY / pointList.size());

                    QGraphicsTextItem* textItem = addText(name);
                    textItem->setDefaultTextColor(QColor(0, 0, 147));
                    textItem->setPos(centroid);

                    QFont font = textItem->font();
                    font.setPointSize(8);
                    textItem->setFont(font);
                    textItem->setZValue(5);  // 确保文字在最上方

                    textItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
                    m_textItems.append(textItem);
                }
            }
        }
    }
}

void MapScene::generateDotPath()
{
    // 为每条线生成路径
    for (auto it = m_dots.begin(); it != m_dots.end(); ++it) {
        qint64 lineId = it.key();
        DotInfo& dotInfo = it.value();

        QVector<QPointF> linePoints = m_linePointsMap.value(lineId);

        if (linePoints.isEmpty()) {
            qDebug() << "无法找到ID为" << lineId << "的线条";
            continue;
        }

        dotInfo.pathPoints = linePoints;
        dotInfo.progress = 0.0;
        dotInfo.forward = true;
        dotInfo.dot->setPos(linePoints.first());
    }
}

void MapScene::updateDotPosition()
{
    const qreal speed = 0.002;  // 调整这个值可以改变移动速度，值越小越慢

    // 更新每个点的位置
    for (auto it = m_dots.begin(); it != m_dots.end(); ++it) {
        DotInfo& dotInfo = it.value();

        if (dotInfo.pathPoints.size() < 2) continue;

        // 更新进度
        if (dotInfo.forward) {
            dotInfo.progress += speed;
            if (dotInfo.progress >= 1.0) {
                dotInfo.progress = 1.0;
                dotInfo.forward = false;
            }
        } else {
            dotInfo.progress -= speed;
            if (dotInfo.progress <= 0.0) {
                dotInfo.progress = 0.0;
                dotInfo.forward = true;
            }
        }

        // 计算当前所在的线段
        qreal totalLength = 0.0;
        QVector<qreal> segmentLengths;

        // 计算所有线段的长度
        for (int i = 0; i < dotInfo.pathPoints.size() - 1; ++i) {
            qreal length = QLineF(dotInfo.pathPoints[i], dotInfo.pathPoints[i + 1]).length();
            segmentLengths.append(length);
            totalLength += length;
        }

        // 根据总进度找到当前位置
        qreal targetDistance = dotInfo.progress * totalLength;
        qreal currentDistance = 0.0;

        // 找到当前所在的线段
        for (int i = 0; i < segmentLengths.size(); ++i) {
            if (currentDistance + segmentLengths[i] >= targetDistance) {
                // 找到了当前线段
                qreal segmentProgress = (targetDistance - currentDistance) / segmentLengths[i];
                QPointF p1 = dotInfo.pathPoints[i];
                QPointF p2 = dotInfo.pathPoints[i + 1];

                // 计算插值位置
                QPointF currentPos(
                    p1.x() + (p2.x() - p1.x()) * segmentProgress,
                    p1.y() + (p2.y() - p1.y()) * segmentProgress
                    );

                dotInfo.dot->setPos(currentPos);
                break;
            }
            currentDistance += segmentLengths[i];
        }
    }
}

void MapScene::updateControlPoint()
{
    // 应用减速度
    if (m_controlPointVelocity.x() > 0)
        m_controlPointVelocity.setX(qMax(0.0, m_controlPointVelocity.x() - m_deceleration));
    else if (m_controlPointVelocity.x() < 0)
        m_controlPointVelocity.setX(qMin(0.0, m_controlPointVelocity.x() + m_deceleration));

    if (m_controlPointVelocity.y() > 0)
        m_controlPointVelocity.setY(qMax(0.0, m_controlPointVelocity.y() - m_deceleration));
    else if (m_controlPointVelocity.y() < 0)
        m_controlPointVelocity.setY(qMin(0.0, m_controlPointVelocity.y() + m_deceleration));

    // 如果还有速度，更新位置
    if (!m_controlPointVelocity.isNull()) {
        QPointF newPos = m_controlPoint->pos() + m_controlPointVelocity;
        m_controlPoint->setPos(newPos);
    }
}

void MapScene::updateSceneBounds()
{
    QRectF bounds = itemsBoundingRect();
    if (!bounds.isEmpty()) {
        double margin = qMax(bounds.width(), bounds.height()) * 0.1;
        bounds.adjust(-margin, -margin, margin, margin);
        setSceneRect(bounds);
    }
}

// 实现 getTextItems() 方法
const QList<QGraphicsTextItem*>& MapScene::getTextItems() const {
    return m_textItems;
}

void MapScene::keyPressEvent(QKeyEvent *event)
{
    if (!m_controlPoint) return;

    QPointF acceleration(0, 0);
    bool keyHandled = true;

    switch (event->key()) {
    case Qt::Key_W:
        acceleration.setY(-m_acceleration);
        break;
    case Qt::Key_S:
        acceleration.setY(m_acceleration);
        break;
    case Qt::Key_A:
        acceleration.setX(-m_acceleration);
        break;
    case Qt::Key_D:
        acceleration.setX(m_acceleration);
        break;
    default:
        keyHandled = false;
        QGraphicsScene::keyPressEvent(event);
    }

    if (keyHandled) {
        // 应用加速度
        m_controlPointVelocity += acceleration;

        // 限制最大速度
        if (m_controlPointVelocity.x() > m_maxVelocity) m_controlPointVelocity.setX(m_maxVelocity);
        if (m_controlPointVelocity.x() < -m_maxVelocity) m_controlPointVelocity.setX(-m_maxVelocity);
        if (m_controlPointVelocity.y() > m_maxVelocity) m_controlPointVelocity.setY(m_maxVelocity);
        if (m_controlPointVelocity.y() < -m_maxVelocity) m_controlPointVelocity.setY(-m_maxVelocity);

        // 更新位置
        QPointF newPos = m_controlPoint->pos() + m_controlPointVelocity;
        m_controlPoint->setPos(newPos);

        event->accept();
    }
}

void MapScene::onBlueDot1Clicked()
{

}

void MapScene::onBlueDot2Clicked()
{

}

void MapScene::onBlueDot3Clicked()
{

}

void MapScene::onBlueDot4Clicked()
{

}

void MapScene::onBlueDot5Clicked()
{


}

void MapScene::onRedDotClicked()
{

}

void MapScene::onBuildingClicked()
{
    ClickablePathItem* building = qobject_cast<ClickablePathItem*>(sender());
    if (building) {
        qint64 buildingId = building->data(0).toLongLong();
        if (buildingId == 675768172) {
            qDebug() << "逸夫书院A栋被点击";
            // TODO: 在这里添加逸夫书院A栋的点击处理逻辑
        } else if (buildingId == 556004689) {
            qDebug() << "行政楼被点击";
            // TODO: 在这里添加行政楼的点击处理逻辑
        } else if (buildingId == 556004687) {
            qDebug() << "图书馆被点击";
            // TODO: 在这里添加图书馆的点击处理逻辑
        } else if (buildingId == 544063809) {
            qDebug() << "教学楼被点击";
            // TODO: 在这里添加教学楼的点击处理逻辑
        } else if (buildingId == 544063812) {
            qDebug() << "实验楼被点击";
            // TODO: 在这里添加实验楼的点击处理逻辑
        } else if (buildingId == 475871696) {
            qDebug() << "体育馆被点击";
            // TODO: 在这里添加体育馆的点击处理逻辑
        }
    }
}
