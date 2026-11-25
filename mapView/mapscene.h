#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QKeyEvent>
#include "clickableellipseitem.h"
#include <QLabel> //新增
#include <QVBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWidget>

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScene(QObject *parent = nullptr);
    void loadGeoJson(const QString& filePath);
    const QList<QGraphicsTextItem*>& getTextItems() const;
//新增
public:
    ClickableEllipseItem* getControlPoint() const { return m_controlPoint; }
    QPointF controlPointPos;
    QWidget* uiNPC;  // 显示NPC状态的 UI
    QLabel* NPCLabel;  // 显示NPC的标签
protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onBlueDot1Clicked();    // 238822691线路的蓝点
    void onBlueDot2Clicked();    // 219060439线路的蓝点
    void onBlueDot3Clicked();    // 717875658线路的蓝点
    void onBlueDot4Clicked();    // 273078858线路的蓝点
    void onBlueDot5Clicked();    // 240371052线路的蓝点
    void onRedDotClicked();      // 红点的点击事件
    void onBuildingClicked();    // 处理建筑物点击事件

private:
    void updateSceneBounds();
    void drawFeature(const QJsonObject& feature);
    void drawMultiLineString(const QJsonArray& coordinates, const QPen& pen, const QString& name, qint64 osm_id);
    void drawMultiPolygon(const QJsonArray& coordinates, const QBrush& brush, const QPen& pen, const QString& name, qint64 osm_id);
    void generateDotPath();
    void updateDotPosition();
    void updateControlPoint();

private:
    struct DotInfo {
        QGraphicsEllipseItem* dot;
        QVector<QPointF> pathPoints;
        qreal progress;  // 用0-1之间的值表示移动进度
        bool forward;
    };

    double m_minX;
    double m_minY;
    bool m_firstLoad;
    QList<QGraphicsTextItem*> m_textItems;  // 保存文本项的列表

    QMap<qint64, DotInfo> m_dots;  // 存储不同线路的点信息
    QTimer* m_dotTimer;            // 共用一个定时器
    QMap<qint64, QVector<QPointF>> m_linePointsMap;

    ClickableEllipseItem* m_controlPoint;  // 可控制的红色点
    qreal m_moveStep;  // 移动步长
    QPointF m_controlPointVelocity;  // 控制点的速度向量
    qreal m_maxVelocity;            // 最大速度
    qreal m_acceleration;           // 加速度
    qreal m_deceleration;          // 减速度

    QList<QGraphicsItem*> m_clickableBuildings;
};

#endif // MAPSCENE_H
