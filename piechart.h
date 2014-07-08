#ifndef PIECHART_H
#define PIECHART_H
#include "chart.h"

class PieChart:public Chart{
private:
    static double massimo;
    double total;
protected:
    void paintEvent(QPaintEvent *);
    QVector<double> normalizer();
public:
    PieChart(Dataset* &,QWidget *parent=0);
};

#endif // PIECHART_H
