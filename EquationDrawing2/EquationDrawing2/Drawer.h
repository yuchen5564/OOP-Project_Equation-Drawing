#pragma once

#include <QtWidgets/QWidget>
#include "ui_Drawer.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QListWidget>
#include <QPushButton>
#include <iostream>
using namespace std;

class Drawer : public QWidget
{
    Q_OBJECT

public:
    Drawer(QWidget *parent = Q_NULLPTR);
    void creatCoordinates(); //¹º¥X§¤¼Ð¨t
    void drawFunction(string function = " ", QColor color = Qt::red);

    void paintEvent(QPaintEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    

public slots:
    void addFunction_slot();
    void removeFunction_slot();
    void editFunction_slot();
    void showFunction_slot();
    void notShowFunction_slot();
    void pickColor_slot();

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
    //QPainter* painter;
    QListWidget* funcList, * showList;
    QPushButton* addBtn, * removeBtn, * editBtn;
    QPushButton* showBtn, * notShowBtn, * pickColorBtn;
    int cc = 0;
};
