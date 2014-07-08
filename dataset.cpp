#include "dataset.h"

void Dataset::removeOne(Data *d){
    list<Data*>::iterator it=set.begin();
    for(; it!=set.end(); ++it){
        if(*it==d){
            it=set.erase(it); --it; // punta adesso all'elemento successivo nella lista. quindi decremento
        }
    }
}
void Dataset::remove(int pos){
    list<Data*>::iterator it=set.begin();
    for(int i=0; it!=set.end(); ++it, ++i){
        if(i == pos){
            it=set.erase(it); return;
        }
    }
}

void Dataset::add(const Data *d){
    Data* p=const_cast<Data*>(d);
    set.push_back(p);
}
double Dataset::minValue()const{
    bool first=true; double m=0;
    for(list<Data*>::const_iterator it=set.begin(); it != set.end(); ++it){
        if(!first){
            if( (**it).getValue() < m) m = (**it).getValue();
        }
        if(first){
            first=false;
            m = (**it).getValue();
        }
    }
    return m;
}
double Dataset::maxValue()const{
    bool first=true; double m=0;
    for(list<Data*>::const_iterator it=set.begin(); it != set.end(); ++it){
        if(!first){
            if( (**it).getValue() > m) m=(**it).getValue();
        }
        if(first){
            first=false;
            m=(**it).getValue();
        }
    }
    return m;
}
list<Data*>::iterator Dataset::first(){
    list<Data*>::iterator it=set.begin();
    return it;
}
// last() ritorna l'iteratore all'ultimo elemento della lista
// NB. si ricorda di incrementare per raggiungere il past-the-end
list<Data*>::iterator Dataset::last(){
    list<Data*>::iterator it=set.end();
    return --it;
}

double Dataset::sumElements()const{
    list<Data*>::const_iterator it=set.begin();
    double sum=0.0;
    for (; it != set.end(); ++it){
        sum += (**it).getValue();
    }
    return sum;
}

void Dataset::clearList(){
        set.erase(set.begin(),set.end());
}

list<Data*>::iterator Dataset::dataAt(int i){
    list<Data*>::iterator it=set.begin();
    list<Data*>::iterator DataToReturn;
    int index=0;
    for(; it != set.end(); ++it){
        if(index==i)
            DataToReturn=it;
        else ++index;
    }
    return DataToReturn;
}

int Dataset::countElement()const{
    return set.size();
}

Dataset::~Dataset(){
    list<Data*>::iterator it=set.begin(), it1=set.end();
    for( ; it != it1; ++it){
        delete *it;
    }
}
