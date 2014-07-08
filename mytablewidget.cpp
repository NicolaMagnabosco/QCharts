#include "mytablewidget.h"
#include <QStringList>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include "mainwindow.h"
#include <sstream>
#include <QShortcut>

// --------------------CUSTOM SLOTS:------------------------

// inserisce i valori di un file qCharts riguardante un 'blank project'
void MyTableWidget::populateBlankTable(Dataset* &informations){
    populateTable(informations);
    if(year){
        QStringList lll; lll << "LABELS"<<"VALUES";
        t->setHorizontalHeaderLabels(lll);
        // il progetto precedente non aveva i bottoni per aggiungere/togliere
        // righe quindi li aggiungo
        add->show();
        remove->show();
        year=false;
    }
}

// inserisce i valori di un file qCharts riguardante un 'year project'
void MyTableWidget::populateYearTable(Dataset* &informations){
    populateTable(informations);
    if(!year)
        add->hide();
    if(!year)
        remove->hide();
    year=true;
}

// inserisce una nuova riga in un blank project
void MyTableWidget::addNewRow(){
    if(rows<24){
        ++rows;
        t->insertRow(t->rowCount());
        addRowWidgets();
        emit(genChart());
    }
    else{
        add->hide();
    }
}

void MyTableWidget::removeRow(){
    if(rows == 24){
        add->show();
    }
    if(rows>0){
        t->removeRow(t->rowCount()-1);
        --rows;
        emit(genChart());
    }
}

void MyTableWidget::genChartSlot(){
    vector<Data> r;
    for(int i=0; i<t->rowCount();++i){
        QLineEdit* label=dynamic_cast<QLineEdit*>(t->cellWidget(i,0));
        QDoubleSpinBox* spinBox=dynamic_cast<QDoubleSpinBox*>(t->cellWidget(i,1));
        if(label && spinBox)
        {Data x(spinBox->value(),label->text()); r.push_back(x);}
    }
    emit(sendUpdateData(r));
}


//-----------PUBLIC METHODS--------------
int MyTableWidget::getRow()const{
    return rows;
}
// popola la tabella con i dati di informations
void MyTableWidget::populateTable(Dataset* &informations){
    list<Data*>::iterator it=informations->first(), it1=informations->last();
    it1++; //adesso punta al past the end
    for(int i=rows; i > -1; --i ){
        t->removeRow(i);
    }
    rows=0;

    for(; it != it1 ; ++it){
        Data* d=static_cast<Data*>(*it);
        t->insertRow(rows);

        QLineEdit* le=new QLineEdit(d->getLabel());

        QDoubleSpinBox *sp=new QDoubleSpinBox();
        sp->setValue(d->getValue());
        sp->setMaximum(99999.99);

        t->setCellWidget(rows,0,le);
        t->setCellWidget(rows,1,sp);

        connectRows(le,sp);

        rows++;
    }
}
// crea le connessioni per le due widget
void MyTableWidget::connectRows(QLineEdit *le, QDoubleSpinBox *sp) const{
    connect(le,SIGNAL(textChanged(QString)),this,SIGNAL(genChart()));
    connect(sp,SIGNAL(valueChanged(double)),this,SIGNAL(genChart()));
    connect(this,SIGNAL(genChart()),this,SLOT(genChartSlot()));
}
// inserisce nella riga le due Widget
void MyTableWidget::addRowWidgets()const{
    QLineEdit* le=new QLineEdit();
    le->setPlaceholderText("Insert Label");
    QDoubleSpinBox *sp=new QDoubleSpinBox();
    sp->setMaximum(99999.99);
    sp->setValue(0.0);
    t->setCellWidget(t->rowCount()-1,0,le);
    t->setCellWidget(t->rowCount()-1,1,sp);
    connectRows(le,sp);
}
//*****************************************

//CONSTRUCTOR:
MyTableWidget::MyTableWidget(QWidget *parent, int r,bool y):
    QWidget(parent),rows(r),year(y){
    add=new QPushButton(tr("Add Row"),this);
    add->setFixedSize(120,50);
    add->setIcon(QIcon(":/new/icons/plus.png"));
    remove=new QPushButton(tr("Remove Row"),this);
    remove->setFixedSize(120,50);
    remove->setIcon(QIcon(":/new/icons/minus.png"));


    connect(add,SIGNAL(clicked()),this,SLOT(addNewRow()));
    connect(remove,SIGNAL(clicked()),this,SLOT(removeRow()));

     if( year==true){// creo una YEAR TABLE
        //nascondo i button
        add->hide();
        remove->hide();

        rows=12;
        t=new QTableWidget(12,2,this);
        QStringList months;
        months << "January"<<"February"<<"March"<<"April"<<"May"<<"June"<<"July"
               <<"August"<<"September"<<"October"<<"November"<<"December";
        for(int i=0; i<12; ++i){
            QLineEdit* le=new QLineEdit(months.at(i));
            QDoubleSpinBox *sp=new QDoubleSpinBox();
            sp->setMaximum(99999.99);
            t->setCellWidget(i,0,le);
            t->setCellWidget(i,1,sp);

            connectRows(le,sp);
        }
        QStringList lll; lll<<"MONTHS" <<"VALUES";
        t->setHorizontalHeaderLabels(lll);
    }
    else{ // creo una 'BLANK' table modificabile per numero di righe
        t= new QTableWidget(1,2,this);
        QStringList lll; lll << "LABELS"<<"VALUES";
        t->setHorizontalHeaderLabels(lll);

        addRowWidgets();
    }
    layout=new QGridLayout;
    layout->addWidget(t,0,0);
    layout->addWidget(add,1,0);
    layout->addWidget(remove,2,0);
    setLayout(layout);
    setFixedSize(250,600);
}
