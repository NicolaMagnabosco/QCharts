#ifndef CHART_H
#define CHART_H
#include "dataset.h"
#include <QPainter>
#include <QWidget>
#include <QVector>
#include <vector>
//classe astratta
class Chart:public QWidget{
    Q_OBJECT
protected:
    vector<QColor> colormap;
    int gapAxis;
    Dataset* d;
    virtual QVector<double> normalizer() =0;
public:
    Chart(Dataset* &,int =60,QWidget* =0);
    int getGap()const;
    virtual ~Chart(){};
};

#endif // CHART_H
