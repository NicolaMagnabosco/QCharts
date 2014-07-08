#ifndef BARCHART_H
#define BARCHART_H
#include <QPainter>
#include <QColor>
#include "cartesianchart.h"

class BarChart:public CartesianChart{
    Q_OBJECT
public:
    BarChart(Dataset* &,QWidget *parent=0);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // BARCHART_H
