#include "Drawer.h"
#include <QPainter>
#include <QPainterPath>
#include <iostream>
#include <QColorDialog>

using namespace std;

Drawer::Drawer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    pImg = new QPixmap(800, 600);
    pImg->fill();

    funcList = new QListWidget(this);
    funcList->setGeometry(800, 50, 400, 600);

    showList = new QListWidget();
    
    addBtn = new QPushButton(this);
    addBtn->setGeometry(800, 0, 50, 50);
    addBtn->setIcon(QIcon(":/Drawer/pic/add.png"));
    addBtn->setIconSize(QSize(45, 45));
    addBtn->setToolTip("新增函數");

    removeBtn = new QPushButton(this);
    removeBtn->setGeometry(850, 0, 50, 50);
    removeBtn->setIcon(QIcon(":/Drawer/pic/remove.png"));
    removeBtn->setIconSize(QSize(45, 45));
    removeBtn->setToolTip("移除函數");

    editBtn = new QPushButton(this);
    editBtn->setGeometry(900, 0, 50, 50);
    editBtn->setIcon(QIcon(":/Drawer/pic/edit2.png"));
    editBtn->setIconSize(QSize(40, 40));
    editBtn->setToolTip("編輯函數");

    showBtn = new QPushButton(this);
    showBtn->setGeometry(950, 0, 50, 50);
    showBtn->setIcon(QIcon(":/Drawer/pic/cansee.png"));
    showBtn->setIconSize(QSize(40, 40));
    showBtn->setToolTip("顯示函數");

    notShowBtn = new QPushButton(this);
    notShowBtn->setGeometry(1000, 0, 50, 50);
    notShowBtn->setIcon(QIcon(":/Drawer/pic/cannotsee.png"));
    notShowBtn->setIconSize(QSize(40, 40));
    notShowBtn->setToolTip("隱藏函數");

    pickColorBtn = new QPushButton(this);
    pickColorBtn->setGeometry(1050, 0, 50, 50);
    pickColorBtn->setIcon(QIcon(":/Drawer/pic/pick.png"));
    pickColorBtn->setIconSize(QSize(40, 40));
    pickColorBtn->setToolTip("選取顏色");

    setFixedSize(1200, 600); //設定視窗大小

    connect(addBtn, SIGNAL(clicked(bool)), this, SLOT(addFunction_slot()));
    connect(removeBtn, SIGNAL(clicked(bool)), this, SLOT(removeFunction_slot()));
    connect(editBtn, SIGNAL(clicked(bool)), this, SLOT(editFunction_slot()));
    connect(showBtn, SIGNAL(clicked(bool)), this, SLOT(showFunction_slot()));
    connect(notShowBtn, SIGNAL(clicked(bool)), this, SLOT(notShowFunction_slot()));
    connect(pickColorBtn, SIGNAL(clicked(bool)), this, SLOT(pickColor_slot()));
}

//繪製坐標軸
void Drawer::creatCoordinates()
{
    QPainter painter(pImg); //繪製於Qpixmap元件上

    //坐標軸區塊大小
    int w = 800; 
    int h = 600; 

    //設置中心點
    painter.translate(400- mid.x() - movePos.x(), 300- mid.y() - movePos.y());
  
    //繪製網格
    //memo: 網格格線依放大倍率調整
    painter.setPen(QPen(QColor(188, 194, 204))); //灰色筆刷
    for (int i = 1; i <= 800 + abs(400 - mid.x() - movePos.x()); i+=40 * scroll) //x軸方向線條
    {
        int line = abs(300 - mid.y() - movePos.y());
        painter.drawLine(i , -h - line, i  ,h + line);
        painter.drawLine(-i, -h - line , -i, h + line);
    }
    for (int i = 1; i <= 600 + abs(300 - mid.y() - movePos.y()); i += 40 * scroll) //y軸方向線條
    {
        int line = abs(400 - mid.x() - movePos.x());
        painter.drawLine(-w - line, i , w + line, i );
        painter.drawLine(-w - line, -i , w + line, -i );
    }


    painter.setPen(QPen(Qt::black,3)); //黑色筆刷，寬度3
    painter.drawLine(0, -h - abs(300 - mid.y() - movePos.y()), 0, h + abs(300 - mid.y() - movePos.y())); // 繪製X軸
    painter.drawLine(-w - abs(400 - mid.x() - movePos.x()), 0, w + abs(400 - mid.x() - movePos.x()), 0); // 繪製Y軸
    painter.setFont(QFont("微軟正黑體",16));
    
    //繪製刻度
    int counter = 0;
    for (int i = 1; i <= 800 + abs(400 - mid.x() - movePos.x()); i += 40 * scroll) //x軸上刻度
    {
        if (counter == 0) {
            counter++;
            continue;
        }
        painter.drawLine(i, 0, i, 8);//X軸正向刻度，箭頭的邊長設為8個畫素
        painter.drawText(i, 0, QString::number(counter)); //X軸正向刻度座標
        painter.drawLine(-i, 0, -i, 8); //X軸負向刻度
        painter.drawText(-i, 0, QString::number(-counter)); //X軸負向刻度座標 //* stepX
        counter++;
    }
    counter = 0;
    for (int i = 1; i <= 600 + abs(300 - mid.y() - movePos.y()); i += 40 * scroll) //y軸上刻度
    {
        if (counter == 0) {
            counter++;
            continue;
        }
        painter.drawLine(0, i, 8, i); //Y軸正向刻度
        painter.drawText(8, i, QString::number(-counter)); //Y軸正向刻度座標
        painter.drawLine(0, -i , 8, -i); //Y軸負向刻度
        painter.drawText(8, -i, QString::number(counter)); //Y軸負向刻度座標
        counter++;
    }

    //在原點標記一個斜體字母O
    QFont font("Times", 16);
    font.setItalic(true);
    painter.setFont(font);
    painter.drawText(1, -1, "O");
}

//繪製函數(sys)
void Drawer::paintEvent(QPaintEvent* event)
{
    /*
    * 繪製流程：
    * 1. 先將座標軸與函數圖形繪製於pImg(QPixmap)
    * 2. 將pImg繪製到視窗上
    */

    pImg->fill();

    //繪製到pImg
    creatCoordinates();
    //drawFunction(" ", Qt::darkGreen);    
    //cout << showList->count() << endl;
    for (int i = 0; i < funcList->count(); i++) {
        drawFunction(funcList->item(i)->text().toStdString(), funcList->item(i)->foreground().color());
        //qDebug()<<funcList->item(i)->icon().themeName();
    }

    //繪製到視窗
    QPainter p(this);
    p.drawPixmap(0,0,*pImg);
    update();
}

void Drawer::drawFunction(string function, QColor color)
{
    qreal mult = 800 / 20 * scroll;  //放大倍數

    /*=== For Test ===*/
    QList<QPointF>points;
    qreal t = -4, y;
    for (int i = 0; i < 80; i++) {
        y = -pow(4 * abs(t), 0.5);
        points.push_back({ t * mult,y * mult});
        t += 0.1;
    }
    /*================*/
    

    QPainterPath path(points[0]);
    for (int i = 0; i < points.size() - 1; ++i)
    {
        QPointF sp = points[i];
        QPointF ep = points[i + 1];
        QPointF c1 = (QPointF((sp.x() + ep.x()) / 2, (sp.y() + ep.y()) / 2));
        //QPointF c2 = c1;
        path.cubicTo(c1, c1, ep); //製造出曲線
    }

    QPainter painter(pImg);
    painter.translate(400 - mid.x() - movePos.x(), 300 - mid.y() - movePos.y()); //設置中心點
    painter.setRenderHint(QPainter::Antialiasing,true); //反鋸齒
    painter.setPen(QPen(color, 2));
    painter.drawPath(path); //劃出路徑
}

//滾輪放大
//memo: 滑鼠位置放大縮小尚未實作
void Drawer::wheelEvent(QWheelEvent* event)
{
    if (event->position().x() < 800) { //超出版面(pImg)之動作不進行
        pImg->fill();
        if (event->angleDelta().y() > 0) {
            cout << "++";
            scroll = scroll + 0.5;
        }
        if (event->angleDelta().y() < 0) {
            cout << "--";
            scroll = scroll - 0.5;
        }

        if (scroll == 0) {
            scroll = 0.5;
        }
        cout << scroll << endl;
    }
    
}

//滑鼠點擊
void Drawer::mousePressEvent(QMouseEvent* event)
{
    QApplication::setOverrideCursor(Qt::OpenHandCursor);
    cout << "Press!\n";
    oldPoint = event->pos();
    m_press = true;
}

//滑鼠釋放
void Drawer::mouseReleaseEvent(QMouseEvent* event)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    cout << "Release!\n";
    m_press = false;
    mid = { mid.x() + movePos.x(),mid.y() + movePos.y() };
    cout << mid.x() << " " << mid.y() << endl;
    movePos = { 0,0 };
}

//滑鼠移動
void Drawer::mouseMoveEvent(QMouseEvent* event)
{
    pImg->fill();
    QPoint nowPoint = event->pos();
    movePos = { oldPoint.x() - nowPoint.x(),oldPoint.y() - nowPoint.y() };
    //cout << event->pos().x() << " " << event->pos().y() << endl;
}

void Drawer::addFunction_slot() {
    QListWidgetItem* item = new QListWidgetItem("y = " + QString::number(cc));
    item->setFont(QFont("Consolas", 18));
    item->setSizeHint(QSize(10, 50));
    item->setIcon(QIcon(":/Drawer/pic/cansee.png"));
    item->setBackground(QBrush(QColor(211, 218, 219)));
    cout << item << endl;
    funcList->addItem(item);
    /*cout << item << endl;
    showList->addItem(funcList->);*/
    cc++;
}

void Drawer::removeFunction_slot()
{
    QListWidgetItem* nowSelect = funcList->currentItem();

    /*if (nowSelect != nullptr) {
        cout << nowSelect->text().toStdString() << endl;
    }*/

    delete nowSelect;
}

void Drawer::editFunction_slot()
{
    static bool flag = 1;
    QListWidgetItem* nowSelect = funcList->currentItem();
    static QListWidgetItem* lastSelect = nullptr;

    if (nowSelect != nullptr) {
        if (flag) {
            funcList->openPersistentEditor(nowSelect);
            lastSelect = nowSelect;
            flag = 0;
            editBtn->setIcon(QIcon(":/Drawer/pic/check.png"));
        }
        else {
            if (nowSelect != lastSelect) {
                funcList->closePersistentEditor(lastSelect);
                funcList->openPersistentEditor(nowSelect);
                lastSelect = nowSelect;
            }
            else {
                funcList->closePersistentEditor(nowSelect);
                flag = 1;
                editBtn->setIcon(QIcon(":/Drawer/pic/edit2.png"));
            }

        }
    }
}

void Drawer::showFunction_slot()
{
    QListWidgetItem* nowSelect = funcList->currentItem();
    if (nowSelect != nullptr) {
        nowSelect->setIcon(QIcon(":/Drawer/pic/cansee.png"));
    }
}

void Drawer::notShowFunction_slot()
{
    QListWidgetItem* nowSelect = funcList->currentItem();
    if (nowSelect != nullptr) {
        nowSelect->setIcon(QIcon(":/Drawer/pic/cannotsee.png"));
    }
}

void Drawer::pickColor_slot()
{
    QListWidgetItem* nowSelect = funcList->currentItem();
    if (nowSelect != nullptr) {
        QColor color = QColorDialog::getColor(Qt::white, this);
        nowSelect->setForeground(color);
       
        //nowSelect->setBackground(QBrush(color));
        //drawFunction(" ", nowSelect->foreground().color());
        
    }
}
