#ifndef CARTESIANCHART_H
#define CARTESIANCHART_H
#include "chart.h"
#include <QPoint>

class CartesianChart : public Chart
{
protected:
    vector<QPoint*> points;
    double distance;
    void paintEvent(QPaintEvent *);
    QVector<double> normalizer();
    void updatePoints();
    static double massimo;
    void drawPlane(QPainter&);
public:
    CartesianChart(Dataset* &,QWidget *parent=0);
    double getDistance()const;
    void drawVerticalText(QPainter&,float, int, int, const QString &);
};

#endif // CARTESIANCHART_H
