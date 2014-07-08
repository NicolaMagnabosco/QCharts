#include "barchart.h"

BarChart::BarChart(Dataset *&da, QWidget *parent):
    CartesianChart(da,parent){}

void BarChart::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    //disegno il piano con la paintEvent di CartesianChart
    drawPlane(p);

    //chiamo updatePoint che aggiorna i Punti con i nuovi valori
    updatePoints();

    list<Data*>::iterator it=d->first(), it2=d->last(); ++it2;
    // disegna il grafico
    double barWidht=getDistance()+1/2;
    for(int i=0; i<d->countElement() /*, it != it2*/; ++i , ++it){
        p.save();
        p.setBrush(QBrush(colormap[i]));
        QRect r(*points[i],QPoint((*points[i]).x()+barWidht,massimo+getGap()));
        p.drawRect(r);
        p.restore();
        drawVerticalText(p,35,(*points[i]).x(),massimo+getGap()+10,(**it).getLabel());
    }
}
