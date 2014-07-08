 #include "linechart.h"
#include <iostream>
using std::cout;

LineChart::LineChart(Dataset* &da, QWidget *parent):
    CartesianChart(da,parent){}
void LineChart::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::SolidPattern);
    //chiamo il metodo drawPlane di CartesianChart che disegna il piano
    drawPlane(p);
    //chiamo updatePoint che aggiorna i Punti con i nuovi valori
    updatePoints();

    // Disegno un quadrato in corrispondenza dei valori sull'asse X
    list<Data*>::iterator it=d->first(), it2=++(d->last());
    for(int i=0; it != it2, i<d->countElement(); ++i, ++it){
        QPoint one((*points[i]).x()-2 , massimo+getGap()-2 );
        QPoint two((*points[i]).x()+2 , massimo+getGap()+2 );
        //
        QRect rect(one,two);
        p.drawRect(rect);
   }
//
//   // ORA DISEGNO IL GRAFICO
    int i=0;
    it=d->first();
    it2=d->last();
    it2++;
    for(; it != it2, i < d->countElement()-1; ++it, ++i){
        p.setPen(QPen(QBrush(colormap[i]),2));
        QPoint one((*points[i]).x(),((*points[i]).y()));
        QPoint two((*points[i+1]).x(),((*points[i+1]).y()));
        p.drawLine(one,two); // Traccio una linea tra il 'one' e il 'two'

        QRect q1(one-QPoint(1,1),one+QPoint(1,1)); // disegno un piccolo quadrato in prossimità del valore
        p.drawRect(q1);
        p.setPen(QColor(Qt::black));

        // scrivo poi il valore sopra il quadrato
        p.save(); p.setBackgroundMode(Qt::OpaqueMode);
        drawVerticalText(p,30,one.x()+8,one.y()+8,QString(QString::number((**it).getValue())));
        p.restore();
        p.setPen(QColor(Qt::black));
        drawVerticalText(p,30,one.x()-4,massimo+getGap()+15,(**it).getLabel());
    }
    // rimane solo l'ultimo valore. Stampo il quadrato, label e value.
    if(i>0){
        p.setPen(QPen(QBrush(Qt::red),1));
        QPoint last((*points[i]).x(),((*points[i]).y()));
        QRect q1(last-QPoint(1,1),last+QPoint(1,1));
        p.drawRect(q1);
        p.setPen(QColor(Qt::black));
        p.save();
        p.setBackgroundMode(Qt::OpaqueMode);
        p.drawText(last-QPoint(0,5),QString(QString::number((**it).getValue())));
        p.restore();
        p.setPen(QColor(Qt::black));
        drawVerticalText(p,30,last.x()-4,massimo+getGap()+15,(**it).getLabel());
    }

}
