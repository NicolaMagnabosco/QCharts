#include "data.h"

Data::Data(double v,QString l):value(v), label(l){}
QString Data::getLabel() const{return label;}
double Data::getValue() const{return value;}
void Data::changeLabel(QString s){label=s;}
void Data::changeValue(double d){value=d;}
