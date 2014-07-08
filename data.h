#ifndef DATA_H
#define DATA_H
#include <QString>
using namespace std;
class Data{
private:
    double value;
    QString label;
public:
    Data(double =0.0,QString ="None");
    double getValue()const;
    QString getLabel()const;
    void changeLabel(QString);
    void changeValue(double);
};

#endif // DATA_H
