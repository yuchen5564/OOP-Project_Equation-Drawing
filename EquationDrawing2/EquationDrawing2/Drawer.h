#pragma once

#include <QtWidgets/QWidget>
#include "ui_Drawer.h"
#include <QMouseEvent>
#include <QPixmap>

class Drawer : public QWidget
{
    Q_OBJECT

public:
    Drawer(QWidget *parent = Q_NULLPTR);
    void creatCoordinates(qreal sacle_x, qreal scale_y, int num_mark);
    void paintEvent(QPaintEvent* event);

    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    void creatFunction();
private:
    Ui::DrawerClass ui;
   // QWidget* graph;
    QImage graph;

    qreal scroll = 1;

    bool m_press = false;
    QPoint oldPoint;
    QPoint movePos = { 0,0 };
    QPoint mid = { 0,0 };
    QPixmap* pImg;
};
