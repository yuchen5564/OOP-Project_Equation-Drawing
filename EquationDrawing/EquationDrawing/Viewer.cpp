#include "Viewer.h"
#include <iostream>
//#include <QtMath>
using namespace std;
Viewer::Viewer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(1020, 520);

    chart = new QChart();

    chartView = new QChartView(this);
    chartView->setGeometry(10, 10, 1000, 500);
    chartView->setChart(chart);

    QLineSeries* series1 = new QLineSeries();
    chart->addSeries(series1);

    QValueAxis* axisX = new QValueAxis;
    axisX->setRange(-5, 5);
    QValueAxis* axisY = new QValueAxis;
    axisY->setRange(-100, 100);

    //vector<QValueAxis>;

    chart->setAxisX(axisX, series1);
    chart->setAxisY(axisY, series1);

    qreal t = -5, y;
    for (int i = 0; i < 100; i++) {
        y = qTan(t);
        //cout << y << endl;
        series1->append(t, y);
        t += 0.01;
    }

    //y=x+2*2+x
    //y=x^2+x+1
    



}
