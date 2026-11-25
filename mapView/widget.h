#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "customgraphicsview.h"
#include "mapscene.h"
#include "menubutton.h"
#include "popupmenu.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QResizeEvent>
#include <QShowEvent>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
//新增ui及变量
public:
    QGraphicsView* graphicsView;  // 存放地图的视图
    QWidget* uiOverlayTopRight;  // 右上角显示游戏时间的 UI
    QWidget* uiOverlayTopRight2;  // 右上角略下显示角色状态的 UI
    QLabel* timeLabel;  // 显示游戏时间的标签
    QLabel* statusLabel;  // 显示角色状态的标签
    QWidget* uiTask;  // 右下角显示角色状态的 UI
    QLabel* TaskLabel;  // 显示游戏任务的标签


public:
    double healthValue = 100.0;      // 健康值
    double hungerValue = 100.0;      // 饱食度
    double energyValue = 100.0;     // 精力值
    QString location= "初始位置";     // 所在位置

public:
    double redDotX = 10;
    double redDotY = 0.0;//存储红点坐标
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void onMenuButtonClicked();    //菜单的槽函数
    void onMenuButton1Clicked();
    void onMenuButton2Clicked();
    void onMenuButton3Clicked();
    void onMenuButton4Clicked();
    void onMenuButton5Clicked();
    void onMenuButton6Clicked();
    void onMenuButton7Clicked();
    void onMenuButton8Clicked();

private:
    void fitViewToScene();
    CustomGraphicsView* view;
    MapScene* scene;
    double initialScale;
    MenuButton* menuButton;
    PopupMenu* popupMenu;
};

#endif // WIDGET_H
