#pragma once

#include <QtWidgets/QWidget>
#include "ui_Drawer.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QListWidget>
#include <QPushButton>
#include <vector>
#include <iostream>
#include "Parser.h"
using namespace std;

class Drawer : public QWidget
{
    Q_OBJECT

public:
    Drawer(QWidget *parent = Q_NULLPTR);
    void creatCoordinates(); //劃出坐標系

    void drawFunction(string function, QColor color);

    void paintEvent(QPaintEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    

public slots:
    /*=== 函數 ===*/
    void addFunction_slot();
    void removeFunction_slot();
    void editFunction_slot();
    void showFunction_slot();
    void pickColor_slot();
    /*=== 變數 ===*/
    void addDefine_slot();
    void removeDefine_slot();
    void editDefine_slot();

private:
    Ui::DrawerClass ui;
   // QWidget* graph;
    QImage graph;

    qreal scroll = 1;
    qreal scroll2 = 1;

    bool m_press = false;
    QPoint oldPoint;
    QPoint movePos = { 0,0 };
    QPoint mid = { 0,0 };
    QPixmap* pImg;
    //QPainter* painter;
    QListWidget* funcList, * defineList;
    QPushButton* addFuncBtn, * removeFuncBtn, * editFuncBtn;
    QPushButton* showFuncBtn, * pickColorBtn;
    QPushButton* addDefineBtn, * removeDefineBtn, * editDefineBtn;
    int cc = 0;

    Parser parser;
};
