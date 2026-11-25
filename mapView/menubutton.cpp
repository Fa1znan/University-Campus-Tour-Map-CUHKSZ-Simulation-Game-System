#include "menubutton.h"
#include <QStyle>

MenuButton::MenuButton(QWidget *parent) : QPushButton(parent)
{
    setFixedSize(40, 40);
    setText("≡");  // 使用三条横线作为菜单图标
    setStyleSheet(
        "QPushButton {"
        "    background-color: #ffffff;"
        "    border: 1px solid #cccccc;"
        "    border-radius: 5px;"
        "    font-size: 24px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #f0f0f0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #e0e0e0;"
        "}"
        );
}
