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
    void creatCoordinates(); //���X���Шt
    bool drawFunction(string function, QColor color); //ø�s���

    void paintEvent(QPaintEvent* event);        //Qtø�s���
    void wheelEvent(QWheelEvent* event);        //Qt�ƹ��u�����
    void mousePressEvent(QMouseEvent* event);   //Qt�ƹ����U���
    void mouseReleaseEvent(QMouseEvent* event); //Qt�ƹ�������
    void mouseMoveEvent(QMouseEvent* event);     //Qt�ƹ����ʨ��

    
//�H����
public slots:
    /*=== ��� ===*/
    void addFunction_slot();    //�W�[�s���
    void removeFunction_slot(); //������w���
    void editFunction_slot();   //�s���w���
    void showFunction_slot();   //��w��Ƥ������/����
    void pickColor_slot();      //����w��ƪ��ϧ��C��
    /*=== �ܼ� ===*/
    void addDefine_slot();      //�W�[�s���ܼƩw�q
    void removeDefine_slot();   //������w���ܼƩw�q
    void editDefine_slot();     //�s���w���ܼƩw�q

private:
    Ui::DrawerClass ui; //ui

    qreal scroll = 1;   //�򥻩�j����
    qreal scroll2 = 1;  //����j����(�Y�p�ɨϥ�)

    bool m_press = false;       //�����ƹ��O�_���U
    QPointF oldPoint;           //�����ƹ����U�ɪ��I
    QPointF movePos = { 0,0 };  //�����ƹ����ʪ��Z��
    QPointF mid = { 0,0 };      //���������I�����q
    QPixmap* pImg;              //��ƹϧ�

    QListWidget* funcList, * defineList;         //��ƻP�ܼ�List
    QPushButton* addFuncBtn, * removeFuncBtn;    //��Ʒs�W�B����=���s
    QPushButton* editFuncBtn;                    //��ƽs����s
    QPushButton* showFuncBtn, * pickColorBtn;    //�����ܡB����C����s
    QPushButton* addDefineBtn, * removeDefineBtn;//�ܼƷs�W�B�������s
    QPushButton* editDefineBtn;                  //�ܼƽs����s

    Parser parser;//Parser function
};
