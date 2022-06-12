#include "Drawer.h"
#include <QPainter>
#include <QPainterPath>
#include <iostream>

using namespace std;



Drawer::Drawer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    pImg = new QPixmap(800, 600);
    pImg->fill();
    setFixedSize(1000, 600);
    //resize(1000, 600);
    //resize(800, 600);
}

void Drawer::creatCoordinates(qreal sacle_x, qreal scale_y, int num_mark)
{
    //resize(win_width, win_height);
    QPainter painter(pImg);
    //ui.widget->show();
    int w = 800; //width of widget
    int h = 600; //height of widget
    //painter.setWindow(-w / 2 + movePos.x(), -h /2+movePos.y(), w, h); //set window
    //painter.setWindow(mid.x()+movePos.x(), mid.y()+movePos.y(), w, h);
    painter.translate(400- mid.x() - movePos.x(), 300- mid.y() - movePos.y());
    //painter.setWindow(0, 0, w, h);
    //QPoint topleft(-w / 2, h / 2); //topleft point of window
    //QSize size(w - 1, h - 1); //window size
    //QRect rectBoder(topleft, size); //window border rect
    ////painter.drawRect(rectBoder); //繪製視窗邊框
    

    
    

    //繪製網格
    painter.setPen(QPen(QColor(188, 194, 204)));
    for (int i = 1; i <= 800 + abs(400 - mid.x() - movePos.x()); i+=40 * scroll)
    {
        int line = abs(300 - mid.y() - movePos.y());
        painter.drawLine(i , -h - line, i  ,h + line);
        painter.drawLine(-i, -h - line , -i, h + line);

        /*painter.drawLine(-w / 2, i , w / 2, i );
        painter.drawLine(-w / 2, -i , w / 2, -i );*/

    }
    for (int i = 1; i <= 600 + abs(300 - mid.y() - movePos.y()); i += 40 * scroll)
    {
        int line = abs(400 - mid.x() - movePos.x());
        painter.drawLine(-w - line, i , w + line, i );
        painter.drawLine(-w - line, -i , w + line, -i );

    }
    /*for (int i = mid.x() - 300 ; i < 300; i += 40 * scroll) {
        painter.drawLine(i, -h / 2, i, h / 2);
    }*/


    painter.setPen(QPen(Qt::black,3));
    painter.drawLine(0, -h - abs(300 - mid.y() - movePos.y()), 0, h + abs(300 - mid.y() - movePos.y())); // 繪製X軸
    painter.drawLine(-w - abs(400 - mid.x() - movePos.x()), 0, w + abs(400 - mid.x() - movePos.x()), 0); // 繪製Y軸
    painter.setFont(QFont("微軟正黑體",16));
    //繪製X軸上刻度

    int counter = 0;
    for (int i = 1; i <= 800 + abs(400 - mid.x() - movePos.x()); i += 40 * scroll)
    {
        if (counter == 0) {
            counter++;
            continue;
        }
        painter.drawLine(i, 0, i, 8);
        //X軸正向刻度，箭頭的邊長設為8個畫素
        painter.drawText(i, 0, QString::number(counter)); //* stepX
        //X軸正向刻度座標
        painter.drawLine(-i, 0, -i, 8); //X軸負向刻度
        painter.drawText(-i, 0, QString::number(-counter)); //X軸負向刻度座標 //* stepX
        counter++;
    }
    //繪製Y軸上刻度
    counter = 0;
    for (int i = 1; i <= 600 + abs(300 - mid.y() - movePos.y()); i += 40 * scroll)
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

void Drawer::paintEvent(QPaintEvent* event)
{
    
    creatCoordinates(1, 1, 20);
    QPainter p(this);
    creatFunction();
    p.drawPixmap(0,0,*pImg);
    update();
    //scale(1,1),X軸畫20個刻度，視窗尺寸800*600

}

void Drawer::creatFunction()
{
    qreal mult = 800 / 20 * scroll;  //放大倍數
    QList<QPointF>points;
    qreal t = -4, y;
    for (int i = 0; i < 80; i++) {
        y = -pow(4 * abs(t), 0.5);
        //y = -1 * qTan(t);
        //cout << y << endl;
        points.push_back({ t * mult,y * mult});
        t += 0.1;
    }

    

    QPainterPath path(points[0]);
    for (int i = 0; i < points.size() - 1; ++i)
    {
        QPointF sp = points[i];
        QPointF ep = points[i + 1];
        QPointF c1 = (QPointF((sp.x() + ep.x()) / 2, (sp.y() + ep.y()) / 2));
        //QPointF c2 = c1;
        path.cubicTo(c1, c1, ep);     //这个函数就是用来绘制曲线的
            //第一个参数和第二个参数都是控制点(百度贝赛尔曲线)
            //第三个参数endpoint,是这小段曲线的终点
            //起点是按照你的队列顺序的前一个节点
            //这亘将cl和C2设置成一样的是因为这小段曲线凹凸性是一样的，

    }
    QPainter painter(pImg);
    //painter.setWindow(-400 + movePos.x(), -300+movePos.y(), 800, 600); //set window
    //painter.setWindow(mid.x() + movePos.x(), mid.y() + movePos.y(), 800, 600); //set window
    painter.translate(400 - mid.x() - movePos.x(), 300 - mid.y() - movePos.y());
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::red, 2));
    //painter.translate(450, 450);
    painter.drawPath(path);
}

void Drawer::wheelEvent(QWheelEvent* event)
{
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

void Drawer::mousePressEvent(QMouseEvent* event)
{
    
    cout << "Press!\n";
    oldPoint = event->pos();
    m_press = true;
}

void Drawer::mouseReleaseEvent(QMouseEvent* event)
{
    cout << "Release!\n";
    m_press = false;
    mid = { mid.x() + movePos.x(),mid.y() + movePos.y() };
    cout << mid.x() << " " << mid.y() << endl;
    movePos = { 0,0 };
}

void Drawer::mouseMoveEvent(QMouseEvent* event)
{
    pImg->fill();
    QPoint nowPoint = event->pos();
    movePos = { oldPoint.x() - nowPoint.x(),oldPoint.y() - nowPoint.y() };
   // mid = { mid.x() + movePos.x(),mid.y() + movePos.y() };
}

