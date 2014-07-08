#include "chart.h"

//costruttore
Chart::Chart(Dataset* &da,int g,QWidget *parent)
    :d(da),gapAxis(g),QWidget(parent){
    // Genero 25 colori
    QColor colori[25]={QColor(255,50,0), QColor(255,80,0), QColor(255,130,0),
                       QColor(255,150,0), QColor(255,180,0), QColor(255,210,0),
                       QColor(255,255,0), QColor(200,255,0), QColor(170,240,0),
                       QColor(140,230,0), QColor(100,230,0), QColor(0,200,50),
                       QColor(0,140,120), QColor(10,100,170), QColor(10,60,170),
                       QColor(20,20,200), QColor(40,10,200), QColor(80,10,200),
                       QColor(120,10,200), QColor(150,0,150), QColor(180,0,150),
                       QColor(200,0,130), QColor(230,0,100), QColor(255,0,80),
                       QColor(255,0,0)
                      };
    for (int i=0; i<25; ++i){
        colormap.push_back(colori[i]);
    }
}

int Chart::getGap()const{return gapAxis;}

