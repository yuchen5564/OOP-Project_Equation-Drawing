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
    bool drawFunction(string function, QColor color); //繪製函數

    void paintEvent(QPaintEvent* event);        //Qt繪製函數
    void wheelEvent(QWheelEvent* event);        //Qt滑鼠滾輪函數
    void mousePressEvent(QMouseEvent* event);   //Qt滑鼠按下函數
    void mouseReleaseEvent(QMouseEvent* event); //Qt滑鼠釋放函數
    void mouseMoveEvent(QMouseEvent* event);     //Qt滑鼠移動函數

    
//信號槽
public slots:
    /*=== 函數 ===*/
    void addFunction_slot();    //增加新函數
    void removeFunction_slot(); //移除選定函數
    void editFunction_slot();   //編輯選定函數
    void showFunction_slot();   //選定函數切換顯示/隱藏
    void pickColor_slot();      //更改選定函數的圖形顏色
    /*=== 變數 ===*/
    void addDefine_slot();      //增加新的變數定義
    void removeDefine_slot();   //移除選定的變數定義
    void editDefine_slot();     //編輯選定的變數定義

private:
    Ui::DrawerClass ui; //ui

    qreal scroll = 1;   //基本放大倍數
    qreal scroll2 = 1;  //次放大倍數(縮小時使用)

    bool m_press = false;       //紀錄滑鼠是否按下
    QPointF oldPoint;           //紀錄滑鼠按下時的點
    QPointF movePos = { 0,0 };  //紀錄滑鼠移動的距離
    QPointF mid = { 0,0 };      //紀錄中心點偏移量
    QPixmap* pImg;              //函數圖形

    QListWidget* funcList, * defineList;         //函數與變數List
    QPushButton* addFuncBtn, * removeFuncBtn;    //函數新增、移除=按鈕
    QPushButton* editFuncBtn;                    //函數編輯按鈕
    QPushButton* showFuncBtn, * pickColorBtn;    //函數顯示、選擇顏色按鈕
    QPushButton* addDefineBtn, * removeDefineBtn;//變數新增、移除按鈕
    QPushButton* editDefineBtn;                  //變數編輯按鈕

    Parser parser;//Parser function
};
