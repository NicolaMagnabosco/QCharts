#ifndef DATASET_H
#define DATASET_H
#include "data.h"
#include <list>
class Dataset{
private:
    list<Data*> set;
public:
    void add(const Data*);
    void removeOne(Data*);
    void remove(int);
    list<Data*>::iterator first();
    list<Data*>::iterator last();
    list<Data*>::iterator dataAt(int i);
    double minValue()const;
    double maxValue()const;
    double sumElements()const;
    void clearList();
    int countElement()const;
    ~Dataset();
};

#endif // DATASET_H
