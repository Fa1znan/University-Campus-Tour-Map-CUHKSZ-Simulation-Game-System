#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <QWidget>
#include <QPushButton>

class PopupMenu : public QWidget
{
    Q_OBJECT
public:
    explicit PopupMenu(QWidget *parent = nullptr);

signals:
    void button1Clicked();
    void button2Clicked();
    void button3Clicked();
    void button4Clicked();
    void button5Clicked();
    void button6Clicked();
    void button7Clicked();
    void button8Clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPushButton* createMenuButton(const QString& text);
};

#endif // POPUPMENU_H
