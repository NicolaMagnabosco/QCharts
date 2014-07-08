#ifndef LINECHART_H
#define LINECHART_H
#include "dataset.h"
#include "cartesianchart.h"

class LineChart:public CartesianChart{
    Q_OBJECT
public:
    LineChart(Dataset* &,QWidget *parent=0);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // LINECHART_H
