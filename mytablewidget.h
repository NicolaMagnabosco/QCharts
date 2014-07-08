#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H
#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "dataset.h"
#include <vector>

class QLineEdit;
class QDoubleSpinBox;

class MyTableWidget:public QWidget{
  Q_OBJECT
private:
    int rows;
    QTableWidget* t;
    QPushButton* add, *remove;
    QGridLayout* layout;
public slots:
    void populateBlankTable(Dataset* &informations);
    void populateYearTable(Dataset* &informations);
    void addNewRow();//used
    void removeRow();//used
    void genChartSlot(); //used
signals:
    void genChart();//used
    void sendUpdateData(vector<Data>);
public:
    Dataset d;
    bool year;
    MyTableWidget(QWidget* parent=0,int r=0,bool y=false);
    int getRow()const;
    void populateTable(Dataset* &);
    void connectRows(QLineEdit*, QDoubleSpinBox*)const;
    void addRowWidgets()const;
};
#endif // MYTABLEWIDGET_H
