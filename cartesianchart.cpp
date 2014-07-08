#include "cartesianchart.h"

double CartesianChart::massimo = 400;

// COSTRUTTORE
CartesianChart::CartesianChart(Dataset* &da, QWidget *parent):
    Chart(da,60,parent),distance(massimo/(da->countElement()-1)){
//    QVector<double> n = normalizer();
//    points.push_back(new QPoint(getGap(),massimo-n[0]+getGap()) );
//    for(int i=1; i < n.size(); ++i){
//        points.push_back( new QPoint(distance*i+getGap(), massimo-n[i]+getGap()) );
//    }
}

// METODO PER DISEGNARE DEL TESTO OBLIQUO SUL PIANO
void CartesianChart::drawVerticalText(QPainter& p, float degrees, int x, int y, const QString &s){
   p.save();
   p.translate(x,y);
   p.rotate(degrees);
   p.drawText(0,0,s);
   p.restore();
}
//ritorna la distanza tra un punto e l'altro
double CartesianChart::getDistance()const{
    return distance;
}

void CartesianChart::updatePoints(){
    distance = massimo / (d->countElement());
    QVector<double> n = normalizer();
    // aggiorno vettore dei punti con i nuovi dati normalizzati
    points.erase(points.begin(),points.end());
//    //points.push_back(new QPoint(getGap(),massimo-n[0]+getGap()) ); *********MOD LAB*************
    for(int i=0; i < n.size(); ++i){
       points.push_back( new QPoint(distance*i+getGap(), massimo-n[i]+getGap()) );
    }
}

// questa paintevent disegna il piano
void CartesianChart::paintEvent(QPaintEvent *){
    //updatePoints();
    QPainter p(this);
    int gap=getGap();
    // disegna e colora il piano di bianco
    QRect plane2(QPoint(gap+1,gap+1),QPoint(massimo+gap-1,massimo+gap-1) );
    p.setBrush(QBrush(Qt::white));p.drawRect(plane2);

    // Stampo 10 valori sull'asse Y in scala col maggiore
    int k=massimo;
    int u=0;
    if(d->maxValue() > 0.0){
        for(double j=0.1;  j < 1.0 ; j+=0.1){
            // trovo il valore da stampare
            double v = (d->maxValue() * k) / massimo;

            // disegno un quadrato sull'asse Y in corrispondenza dei valori
            QRect vr(QPoint(getGap()-2, getGap() + u -2),QPoint(getGap() + 2,getGap() + u +2));
            p.setBrush(Qt::SolidPattern);p.drawRect(vr);

            // stampo il numero ..
            if(v > 0)
                p.drawText(QPoint(0,getGap()+u+5),QString(QString::number(v,'f',1)));

            // disegno delle linee orizzontali lungo il piano per aiutare a leggere i valori distanti
            p.drawLine(QPoint(gap,gap+u),QPoint(massimo+gap,gap+u));
            k=massimo*j;
            u = (1-j)*massimo;
        }
    }
}
void CartesianChart::drawPlane(QPainter &p){
    int gap=getGap();
    // disegna e colora il piano di bianco
    QRect plane2(QPoint(gap+1,gap+1),QPoint(massimo+gap-1,massimo+gap-1) );
    p.setBrush(QBrush(Qt::white));p.drawRect(plane2);

    // Stampo 10 valori sull'asse Y in scala col maggiore
    int k=massimo; int u=0;
    for(double j=0.1;  j < 1.0 ; j+=0.1){
        // trovo il valore da stampare
        double v = (d->maxValue() * k) / massimo;

        // disegno un quadrato sull'asse Y in corrispondenza dei valori
        QRect vr(QPoint(getGap()-2, getGap() + u -2),QPoint(getGap() + 2,getGap() + u +2));
        p.setBrush(Qt::SolidPattern);p.drawRect(vr);

        // stampo il numero ..
        if(v > 0)
            p.drawText(QPoint(0,getGap()+u+5),QString(QString::number(v,'f',1)));

        // disegno delle linee orizzontali lungo il piano per aiutare a leggere i valori distanti
        p.drawLine(QPoint(gap,gap+u),QPoint(massimo+gap,gap+u));
        k=massimo*j;
        u = (1-j)*massimo;
    }
}

QVector<double> CartesianChart::normalizer(){
    QVector<double> dataNormalized;
    list<Data*>::iterator it0=d->first(), it1=d->last();
    it1++; //it ora punta al past the end
    //trovo il massimo con cui normalizzerò
    double higher=0.0;
    for( ; it0 != it1 ; ++it0){
        Data* puntData = static_cast<Data*>(*it0);
        if(puntData->getValue() > higher)
            higher = puntData->getValue();
    }
    //costruisco vettore di dati normalizzati
    for( it0 = d->first() ; it0 != it1 ; ++it0){
        Data* aux =  static_cast<Data*>(*it0);
        if(higher > 0)
            dataNormalized.push_back(aux->getValue()*massimo/higher);
        else
            dataNormalized.push_back(aux->getValue()*massimo);
    }
    return dataNormalized;
}
