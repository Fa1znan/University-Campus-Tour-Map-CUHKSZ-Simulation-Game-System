#include "popupmenu.h"
#include <QGridLayout>
#include <QMouseEvent>

PopupMenu::PopupMenu(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);

    // 设置整个弹窗的样式，包括背景色
    setStyleSheet(
        "PopupMenu {"
        "    background-color: #e6f3ff;"  // 浅蓝色背景
        "    border: 1px solid #cccccc;"
        "    border-radius: 5px;"
        "}"
        );

    QGridLayout* layout = new QGridLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);  // 添加一些内边距

    // 创建8个按钮并布局
    QPushButton* btn1 = createMenuButton("功能1");
    QPushButton* btn2 = createMenuButton("功能2");
    QPushButton* btn3 = createMenuButton("功能3");
    QPushButton* btn4 = createMenuButton("功能4");
    QPushButton* btn5 = createMenuButton("功能5");
    QPushButton* btn6 = createMenuButton("功能6");
    QPushButton* btn7 = createMenuButton("功能7");
    QPushButton* btn8 = createMenuButton("功能8");

    layout->addWidget(btn1, 0, 0);
    layout->addWidget(btn2, 0, 1);
    layout->addWidget(btn3, 1, 0);
    layout->addWidget(btn4, 1, 1);
    layout->addWidget(btn5, 2, 0);
    layout->addWidget(btn6, 2, 1);
    layout->addWidget(btn7, 3, 0);
    layout->addWidget(btn8, 3, 1);

    connect(btn1, &QPushButton::clicked, this, &PopupMenu::button1Clicked);
    connect(btn2, &QPushButton::clicked, this, &PopupMenu::button2Clicked);
    connect(btn3, &QPushButton::clicked, this, &PopupMenu::button3Clicked);
    connect(btn4, &QPushButton::clicked, this, &PopupMenu::button4Clicked);
    connect(btn5, &QPushButton::clicked, this, &PopupMenu::button5Clicked);
    connect(btn6, &QPushButton::clicked, this, &PopupMenu::button6Clicked);
    connect(btn7, &QPushButton::clicked, this, &PopupMenu::button7Clicked);
    connect(btn8, &QPushButton::clicked, this, &PopupMenu::button8Clicked);

    setFixedSize(200, 200);
}

QPushButton* PopupMenu::createMenuButton(const QString& text)
{
    QPushButton* btn = new QPushButton(text, this);
    btn->setStyleSheet(
        "QPushButton {"
        "    background-color: #ffffff;"
        "    border: 1px solid #cccccc;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    min-width: 80px;"
        "    min-height: 30px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #f0f0f0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #e0e0e0;"
        "}"
        );
    return btn;
}

void PopupMenu::mousePressEvent(QMouseEvent *event)
{
    // 点击弹窗外部区域时关闭弹窗
    if (!rect().contains(event->pos())) {
        close();
    }
    QWidget::mousePressEvent(event);
}
