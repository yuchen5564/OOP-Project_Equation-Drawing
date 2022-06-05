#pragma once

#include <QtWidgets/QWidget>
#include "ui_Viewer.h"
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>


class Viewer : public QWidget
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = Q_NULLPTR);

private:
    Ui::ViewerClass ui;
    QChartView* chartView;
    QChart* chart;
    
};
