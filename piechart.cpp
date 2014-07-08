#include "piechart.h"

double PieChart::massimo=430;

//costruttore
PieChart::PieChart(Dataset* &da,QWidget *parent):
    Chart(da,60,parent),total(da->sumElements()){}

//metodo normalizzatore
QVector<double> PieChart::normalizer(){
    total = d->sumElements();
    QVector<double> dataNormalized;
    list<Data*>::iterator it0 = d->first(), it1 = d->last();
    it1++; //ora punta al past the end
    for( ; it0 != it1 ; ++it0){
        Data* aux = static_cast<Data*>(*it0);
        dataNormalized.push_back( (aux->getValue()*5760) /total);
    }
    return dataNormalized;
}
// paintEvent
void PieChart::paintEvent(QPaintEvent *){
    QPainter p(this);

    //disegno la torta
    QRectF r(QPoint(getGap(),getGap()),QPoint(massimo+getGap(),massimo+getGap()));
    double startAngle=0.0;
    double spanAngle=0.0;

    p.setRenderHint(QPainter::Antialiasing);
    QVector<double> normalizedData = normalizer();
    for(int i=0; i<normalizedData.size(); ++i){
        spanAngle = normalizedData[i];
        p.setBrush(QBrush(colormap[i]));
        p.drawPie(r,startAngle,spanAngle);
        startAngle+=spanAngle;
    }
    // disegno la legenda sul lato destro
    int topRight=massimo+getGap()*2;
    int bottomLeft=d->countElement()*20;
    int distBetweenRect=5;
    int miniRect=15;
    QRect backgLabel(QPoint(topRight,getGap()), QPoint(topRight+200,bottomLeft+distBetweenRect+getGap()));
    p.setBrush(QBrush(Qt::white));
    p.drawRect(backgLabel);

    list<Data*>::iterator it=d->first(), it2=d->last(); ++it2;

    QPoint topMiniRect(topRight+distBetweenRect,getGap()+distBetweenRect);
    QPoint botMiniRect(topMiniRect.x()+miniRect,getGap()+distBetweenRect+miniRect);
    for(int i=0; i<d->countElement() , it != it2; ++i, ++it){
        QRect mini(topMiniRect,botMiniRect);

        p.setBrush(QBrush(colormap[i])); p.drawRect(mini);
        p.save();
        p.setPen(QColor(Qt::black));
        p.drawText(botMiniRect+QPoint(10,0),(**it).getLabel());
        p.restore();

        topMiniRect.setY(topMiniRect.y()+miniRect+distBetweenRect);
        botMiniRect.setY(botMiniRect.y()+miniRect+distBetweenRect);
    }
}
