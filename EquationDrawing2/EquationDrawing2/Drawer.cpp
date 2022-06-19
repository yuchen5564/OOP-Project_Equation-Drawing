#include "Drawer.h"
#include <QPainter>
#include <QPainterPath>
#include <iostream>
#include <QColorDialog>

#include <cmath>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include <QGraphicsItem>

using namespace std;

Drawer::Drawer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    setWindowTitle("Equation Drawing");
    setWindowIcon(QIcon(":/Drawer/pic/icon2.png"));
    setFixedSize(1200, 600); //設定視窗大小

    pImg = new QPixmap(800, 600);
    pImg->fill();
    
    /*=== 函數 ===*/
    funcList = new QListWidget(this);
    funcList->setGeometry(800, 40, 400, 300);
    
    addFuncBtn = new QPushButton(this);
    addFuncBtn->setGeometry(800, 0, 40, 40);
    addFuncBtn->setIcon(QIcon(":/Drawer/pic/add.png"));
    addFuncBtn->setIconSize(QSize(35, 35));
    addFuncBtn->setToolTip("新增函數");

    removeFuncBtn = new QPushButton(this);
    removeFuncBtn->setGeometry(840, 0, 40, 40);
    removeFuncBtn->setIcon(QIcon(":/Drawer/pic/remove.png"));
    removeFuncBtn->setIconSize(QSize(35, 35));
    removeFuncBtn->setToolTip("移除函數");

    editFuncBtn = new QPushButton(this);
    editFuncBtn->setGeometry(880, 0, 40, 40);
    editFuncBtn->setIcon(QIcon(":/Drawer/pic/edit2.png"));
    editFuncBtn->setIconSize(QSize(30, 30));
    editFuncBtn->setToolTip("編輯函數");

    showFuncBtn = new QPushButton(this);
    showFuncBtn->setGeometry(920, 0, 40, 40);
    showFuncBtn->setIcon(QIcon(":/Drawer/pic/cansee.png"));
    showFuncBtn->setIconSize(QSize(30, 30));
    showFuncBtn->setToolTip("顯示/隱藏");

    pickColorBtn = new QPushButton(this);
    pickColorBtn->setGeometry(960, 0, 40, 40);
    pickColorBtn->setIcon(QIcon(":/Drawer/pic/pick.png"));
    pickColorBtn->setIconSize(QSize(30, 30));
    pickColorBtn->setToolTip("選取顏色");

    /*=== 變數 ===*/
    defineList = new QListWidget(this);
    defineList->setGeometry(800, 380, 400, 220);

    addDefineBtn = new QPushButton(this);
    addDefineBtn->setGeometry(800, 340, 40, 40);
    addDefineBtn->setIcon(QIcon(":/Drawer/pic/add.png"));
    addDefineBtn->setIconSize(QSize(35, 35));
    addDefineBtn->setToolTip("新增變數");

    removeDefineBtn = new QPushButton(this);
    removeDefineBtn->setGeometry(840, 340, 40, 40);
    removeDefineBtn->setIcon(QIcon(":/Drawer/pic/remove.png"));
    removeDefineBtn->setIconSize(QSize(35, 35));
    removeDefineBtn->setToolTip("移除變數");

    editDefineBtn = new QPushButton(this);
    editDefineBtn->setGeometry(880, 340, 40, 40);
    editDefineBtn->setIcon(QIcon(":/Drawer/pic/edit2.png"));
    editDefineBtn->setIconSize(QSize(30, 30));
    editDefineBtn->setToolTip("編輯變數");

    connect(addFuncBtn, SIGNAL(clicked(bool)), this, SLOT(addFunction_slot()));
    connect(removeFuncBtn, SIGNAL(clicked(bool)), this, SLOT(removeFunction_slot()));
    connect(editFuncBtn, SIGNAL(clicked(bool)), this, SLOT(editFunction_slot()));
    connect(showFuncBtn, SIGNAL(clicked(bool)), this, SLOT(showFunction_slot()));
    connect(pickColorBtn, SIGNAL(clicked(bool)), this, SLOT(pickColor_slot()));

    connect(addDefineBtn, SIGNAL(clicked(bool)), this, SLOT(addDefine_slot()));
    connect(removeDefineBtn, SIGNAL(clicked(bool)), this, SLOT(removeDefine_slot()));
    connect(editDefineBtn, SIGNAL(clicked(bool)), this, SLOT(editDefine_slot()));

    parser = Parser();
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

    //倍數
    int step = 40 * scroll;
    if (scroll == 0.5) {
        step *= 4;
    }
    
    for (int i = 1; i <= 800 + abs(400 - mid.x() - movePos.x()); i+=step) //y軸方向線條
    {
        int line = abs(300 - mid.y() - movePos.y()); //位移參數
        if (step > 100) {
            painter.drawLine(i - step / 2, -h - line, i - step / 2, h + line);
            painter.drawLine(-i - step / 2, -h - line, -i - step / 2, h + line);
        }
            painter.drawLine(i, -h - line, i, h + line);
            painter.drawLine(-i, -h - line, -i, h + line);
    }
    for (int i = 1; i <= 600 + abs(300 - mid.y() - movePos.y()); i += step) //x軸方向線條
    {
        int line = abs(400 - mid.x() - movePos.x()); //位移參數
        if (step > 100) {
            painter.drawLine(-w - line, i - step / 2, w + line, i - step / 2);
            painter.drawLine(-w - line, -i - step / 2, w + line, -i - step / 2);
        }
        painter.drawLine(-w - line, i , w + line, i );
        painter.drawLine(-w - line, -i , w + line, -i );
    }


    painter.setPen(QPen(Qt::black,3)); //黑色筆刷，寬度3

    painter.drawLine(0, -h - abs(300 - mid.y() - movePos.y()), 0, h + abs(300 - mid.y() - movePos.y())); // 繪製X軸
    painter.drawLine(-w - abs(400 - mid.x() - movePos.x()), 0, w + abs(400 - mid.x() - movePos.x()), 0); // 繪製Y軸

    //在原點標記一個斜體字母O
    QFont font("Times", 16);
    font.setItalic(true);
    painter.drawText(1, -1, "O");
    font.setItalic(false);

    painter.setFont(QFont("微軟正黑體", 16));

    //繪製刻度
    int counter = 0;
    for (int i = 1; i <= 800 + abs(400 - mid.x() - movePos.x()); i += step) //x軸上刻度
    {
        if (counter == 0) {
            counter++;
            continue;
        }
        
        painter.drawLine(i, 0, i, 8);//X軸正向刻度，箭頭的邊長設為8個畫素
        painter.drawText(i, 0, QString::number(counter * scroll2)); //X軸正向刻度座標
        painter.drawLine(-i, 0, -i, 8); //X軸負向刻度
        painter.drawText(-i, 0, QString::number(-counter * scroll2)); //X軸負向刻度座標 
        counter++;
    }
    counter = 0;
    for (int i = 1; i <= 600 + abs(300 - mid.y() - movePos.y()); i += step ) //y軸上刻度
    {
        if (counter == 0) {
            counter++;
            continue;
        }
        painter.drawLine(0, i, 8, i); //Y軸正向刻度
        painter.drawText(8, i, QString::number(-counter * scroll2)); //Y軸正向刻度座標
        painter.drawLine(0, -i , 8, -i); //Y軸負向刻度
        painter.drawText(8, -i, QString::number(counter * scroll2)); //Y軸負向刻度座標
        counter++;
    }

    
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

    creatCoordinates(); //繪製坐標軸

    //繪製函數圖形
    for (int i = 0; i < funcList->count(); i++) {
        if (funcList->item(i)->background() != QColor(255, 255, 255)) { //以背景顏色來判別是否顯示
            if (!drawFunction(funcList->item(i)->text().toStdString(), funcList->item(i)->foreground().color())) {
                funcList->item(i)->setBackground(QColor(232, 107, 107)); //格式錯誤
            }
            else {
                funcList->item(i)->setBackground(QColor(211, 218, 219)); //格式正確
            }
        }
    }

    //繪製到視窗
    QPainter p(this);
    p.drawPixmap(0,0,*pImg);
    update();
}

bool Drawer::drawFunction(string function, QColor color)
{
    
    vector<Point> point;
    point.clear();

    if (!parser.calculate(function, &point)) {
        return false;
    }

    qreal mult = 40 * scroll;  //放大倍數
    if (scroll == 0.5) {
        mult =  80 / scroll2;
    }

    vector<QPointF>points;
    vector<Point>::iterator i;
    for (i = point.begin(); i != point.end(); i++) {
        points.push_back({ i->x * mult,i->y * mult * (-1)});
    }

    QPainterPath path(points[0]);
    QPainter painter(pImg);
    painter.translate(400 - mid.x() - movePos.x(), 300 - mid.y() - movePos.y()); //設置中心點
    painter.setRenderHint(QPainter::Antialiasing, true); //反鋸齒
    painter.setPen(QPen(color, 2));

    for (int i = 0; i < points.size() - 1; ++i)
    {
        
        QPointF sp = points[i];
        QPointF ep = points[i + 1];
        QPointF c1 = (QPointF((sp.x() + ep.x()) / 2, (sp.y() + ep.y()) / 2));

        if (isinf(sp.y()) || isnan(sp.y())) {
            painter.drawPath(path); //劃出路徑
            path.clear();
            path = QPainterPath(points[i + 1]);
        }

        if (sp.y() >= 0 && ep.y() <= 0 && sp.y()-ep.y()>10*mult) { //分母=0 Type 1
            painter.drawPath(path); //劃出路徑
            path.clear();
            path = QPainterPath(points[i + 1]);
        }
        else if (sp.y() <= 0 && ep.y() >= 0 && ep.y() - sp.y() > 10*mult) { //分母=0 Type 2
            painter.drawPath(path); //劃出路徑
            path.clear();
            path = QPainterPath(points[i + 1]);
        }
        else {
            path.cubicTo(c1, c1, ep); //製造出曲線
        }

    }

    painter.drawPath(path); //劃出路徑
    return true;
}

//滾輪放大
void Drawer::wheelEvent(QWheelEvent* event)
{
    if (event->position().x() < 800) { //超出版面(pImg)之動作不進行
        pImg->fill();
        
        //先調整基本倍數，再調整次倍數
        if (scroll == 0.5) { 
            scroll = 0.5;
            if (event->angleDelta().y() > 0) {
                if (scroll2 == 1) scroll += 0.5;
                else scroll2-=0.5;
            }
            if (event->angleDelta().y() < 0) {
                scroll2+=0.5;
            }
        }
        else {
            if (event->angleDelta().y() > 0) scroll = scroll + 0.5;
            if (event->angleDelta().y() < 0) scroll = scroll - 0.5;
        }
        
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
}

void Drawer::addFunction_slot() {
    QListWidgetItem* item = new QListWidgetItem("y = x");
    item->setFont(QFont("Consolas", 12));
    item->setSizeHint(QSize(10, 30));
    item->setIcon(QIcon(":/Drawer/pic/cansee.png"));
    item->setBackground(QBrush(QColor(211, 218, 219)));
    item->setForeground(Qt::GlobalColor(14));

    funcList->addItem(item);
    //cc++;
}

void Drawer::removeFunction_slot()
{
    QListWidgetItem* nowSelect = funcList->currentItem();
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
            editFuncBtn->setIcon(QIcon(":/Drawer/pic/check.png"));
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
                editFuncBtn->setIcon(QIcon(":/Drawer/pic/edit2.png"));
            }

        }
    }
}

void Drawer::showFunction_slot()
{
    QListWidgetItem* nowSelect = funcList->currentItem();
    if (nowSelect != nullptr) {
        if (nowSelect->background() == QColor(211, 218, 219)) { //選擇當下為顯示，切換為隱藏
            nowSelect->setIcon(QIcon(":/Drawer/pic/cannotsee.png"));
            nowSelect->setBackground(QColor(255, 255, 255));
        }
        else if (nowSelect->background() == QColor(255, 255, 255)) { //選擇當下為隱藏，切換為顯示
            nowSelect->setIcon(QIcon(":/Drawer/pic/cansee.png"));
            nowSelect->setBackground(QColor(211, 218, 219));
        }
    }
    
}

void Drawer::pickColor_slot()
{
    QListWidgetItem* nowSelect = funcList->currentItem();
    if (nowSelect != nullptr) {
        QColor color = QColorDialog::getColor(Qt::white, this);
        nowSelect->setForeground(color);
    }
}

/*=== 變數 ===*/
void Drawer::addDefine_slot() {
    QListWidgetItem* item = new QListWidgetItem("a = 1");
    item->setFont(QFont("Consolas", 15));
    item->setSizeHint(QSize(10, 30));
    item->setIcon(QIcon(":/Drawer/pic/define.png"));

    defineList->addItem(item);

    parser.clearVariety();
    for (int i = 0; i < defineList->count(); i++) {
        if (!parser.setVariable(defineList->item(i)->text().toStdString())) {
            defineList->item(i)->setBackground(QColor(232, 107, 107)); //格式錯誤
        }
        else {
            defineList->item(i)->setBackground(QColor(211, 218, 219)); //格式正確
        }
    }

    
}

void Drawer::removeDefine_slot()
{
    QListWidgetItem* nowSelect = defineList->currentItem();
    delete nowSelect;

    parser.clearVariety();
    for (int i = 0; i < defineList->count(); i++) {
        if (!parser.setVariable(defineList->item(i)->text().toStdString())) {
            defineList->item(i)->setBackground(QColor(232, 107, 107)); //格式錯誤
        }
        else {
            defineList->item(i)->setBackground(QColor(211, 218, 219)); //格式正確
        }
    }
}

void Drawer::editDefine_slot()
{
    static bool flag = 1;
    QListWidgetItem* nowSelect = defineList->currentItem();
    static QListWidgetItem* lastSelect = nullptr;

    if (nowSelect != nullptr) {
        if (flag) {
            defineList->openPersistentEditor(nowSelect);
            lastSelect = nowSelect;
            flag = 0;
            editDefineBtn->setIcon(QIcon(":/Drawer/pic/check.png"));

        }
        else {
            if (nowSelect != lastSelect) {
                defineList->closePersistentEditor(lastSelect);
                defineList->openPersistentEditor(nowSelect);
                lastSelect = nowSelect;

            }
            else {
                defineList->closePersistentEditor(nowSelect);
                flag = 1;
                editDefineBtn->setIcon(QIcon(":/Drawer/pic/edit2.png"));
            }

        }

        parser.clearVariety();
        for (int i = 0; i < defineList->count(); i++) {
            if (!parser.setVariable(defineList->item(i)->text().toStdString())) {
                defineList->item(i)->setBackground(QColor(232, 107, 107)); //格式錯誤
            }
            else {
                defineList->item(i)->setBackground(QColor(211, 218, 219)); //格式正確
            }
        }
    }
}