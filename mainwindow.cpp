#include <QtGui>
#include <QAction>
#include "mainwindow.h"
#include <QTableWidget>
#include <QTabWidget>
#include <QImage>
#include <QSize>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "linechart.h"
#include "barchart.h"
#include "piechart.h"
#include <QMessageBox> 
#include <list>
//-------------- METHODS -----------------
void MainWindow::createActions(){
    blank=new QAction(QIcon(":/new/icons/blankChart.png"),tr("Blank Chart"),this);
    year=new QAction(QIcon(":/new/icons/yearChart.png"),tr("Year Chart"),this);
    open=new QAction(QIcon(":/new/icons/open.png"),tr("Open"),this);
    save=new QAction(QIcon(":/new/icons/document_save.png"),tr("Save"),this);
    saveAs=new QAction(QIcon(":/new/icons/document_save_as.png"),tr("Save As"),this);
    exit=new QAction(QIcon(":/new/icons/application_exit.ico"),tr("Exit QCharts"),this);
    connect(exit,SIGNAL(triggered()),this,SLOT(exitApp()));
    connect(blank,SIGNAL(triggered()),this,SLOT(newBlank()));
    connect(year,SIGNAL(triggered()),this,SLOT(newYear()));
    connect(save,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(open,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(saveAs,SIGNAL(triggered()),this,SLOT(saveFileAs()));
}
void MainWindow::createMenu(){
    QMenu* file=menuBar()->addMenu(tr("File"));
    file->addAction(blank);
    file->addAction(year);
    file->addAction(open);
    file->addAction(save);
    file->addAction(saveAs);
    file->addAction(exit);
}
void MainWindow::createToolBar(){
    toolbar=addToolBar("Toolbar");
    toolbar->addAction(blank);
    toolbar->addAction(year);
    toolbar->addAction(open);
    toolbar->addAction(save);
    toolbar->addAction(saveAs);
    toolbar->addSeparator();
    toolbar->addAction(exit);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}
void MainWindow::createChartTab(){
    // crea tre tab canvas contenenti i grafici
    lineCanvas=new LineChart(informations,this);//passaggio con puntatore
    barCanvas=new BarChart(informations,this);
    pieCanvas=new PieChart(informations,this);
    chartTab->addTab(lineCanvas,QIcon(":/new/icons/yearChart.png"),tr("Line Chart"));
    chartTab->addTab(barCanvas,QIcon(":/new/icons/barChart.png"),tr("Bar Chart"));
    chartTab->addTab(pieCanvas,QIcon(":/new/icons/pieChart.png"),tr("Pie Chart"));
    mainLayout->addWidget(chartTab,0,3);
}

void MainWindow::createBlankTableWidget(){
    // create a Table for the input of data values/label
    tableValue=new MyTableWidget(this);
    connect(tableValue,SIGNAL(sendUpdateData(vector<Data>)),this,SLOT(receiveUpdateData(vector<Data>)));
    connect(this,SIGNAL(populateBlank(Dataset* &)),tableValue,SLOT(populateBlankTable(Dataset* &)));
    connect(this,SIGNAL(populateYear(Dataset* &)),tableValue,SLOT(populateYearTable(Dataset* &)));
}
void MainWindow::createYearTableWidget(){
    // crea una tabella di 12 righe corrispondenti ai mesi dell'anno
    tableValue=new MyTableWidget(this,12,true);
    connect(tableValue,SIGNAL(sendUpdateData(vector<Data>)),this,SLOT(receiveUpdateData(vector<Data>)));
    // connessione con slot per l'apertura di un file che andrà a popolare la table con i valori in input
    connect(this,SIGNAL(populateBlank(Dataset* &)),tableValue,SLOT(populateBlankTable(Dataset* &)));
    connect(this,SIGNAL(populateYear(Dataset* &)),tableValue,SLOT(populateYearTable(Dataset* &)));
}

void MainWindow::createNewTableWidget(bool year){
    informations->clearList();
    delete tableValue;
    if(year){
       isYearProject = true;
       createYearTableWidget();
    }
    else{//is blank
        isYearProject = false;
        createBlankTableWidget();
    }
       currentFile.clear();
       mainLayout->addWidget(tableValue,0,0);
       drawOpenData();
}

//--------------PRIVATE SLOTS------------------

/* Questo slot riceve i dati aggiornati all'ultima interazione dell'utente.
 * Aggiorna automaticamente i grafici con i valori aggiornati */
void MainWindow::receiveUpdateData(vector<Data> r){
    // il documento è stato modificato
    documentModified=true;

    informations->clearList();
     for(int i=0; i<r.size(); ++i){
        Data* n=new Data(r[i]);
        informations->add(n);
    }
    lineCanvas->update();
    barCanvas->update();
    pieCanvas->update();
}

// questo slot apre un file di un progetto qCharts esistente
void MainWindow::openFile(){
    bool year=false;
    int rows=0;

    QString filename=QFileDialog::getOpenFileName(this,tr("Open QChart"),
                                                    ".",tr("Xml Files (*.xml)"));
    QFile file(filename);

    // se non apro nessun file, esco
    if (!file.open(QFile::ReadOnly | QFile::Text))
           return;
    informations->clearList();
    //file.open(QIODevice::ReadWrite);
    QXmlStreamReader xmlReader(&file);
    while(!xmlReader.atEnd() && !xmlReader.hasError()){
        // leggo il prossimo elemento
        QXmlStreamReader::TokenType token=xmlReader.readNext();
        // se il token è solo il StartDocument, vado al prossimo
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        // se il token è StartElement, guardo se posso leggerlo
        if(token==QXmlStreamReader::StartElement){
            // se il nome è 'chart', il documento è ok e vado al prossimo
            if(xmlReader.name()=="year" || xmlReader.name()=="blank"){
                // se il file da aprire riguarda uno year project..
                if(xmlReader.name()=="year"){
                    QXmlStreamAttributes nRows=xmlReader.attributes();
                    QString s=nRows.value("rows").toString();
                    rows=s.toInt();
                    year=true; // setto yaer a true
                    isYearProject=true;
                }
                else{ // altrimenti è un 'blank project'
                    year=false;
                    isYearProject=false;
                }
                continue;
             }
             // se il valore è 'element', leggo l'informazione
             if(xmlReader.name()=="element"){
                 parseElement(xmlReader);
             }
             if(xmlReader.name() != "year" && xmlReader.name() != "blank" && xmlReader.name() != "element"){
                 QMessageBox::warning(this,"The Xml file has errors.","The file contains some errors or is not an xml file.");
                 return;
             }
        }
    }

    if(year){
        drawOpenData();
        fillOpenTable(true);
    }
    else{
        drawOpenData();
        fillOpenTable(false );
    }
    // setto il file corrente a quello appena aperto
    currentFile=filename;
}

// questo slot è un parser dell'elemento
void MainWindow::parseElement(QXmlStreamReader &xml){
    QString label;
    double value=0.0;
    // guardo se abbiamo veramente un elemento da leggere
    if(!xml.isStartElement() && xml.name() == "element"){
        return;
    }
    while(!(xml.isEndElement() &&
            xml.name() == "element")){
        if(xml.isStartElement()){

            // abbiamo trovato il valore 'label'
            if(xml.name() == "label")
                label=xml.readElementText();

            // e il 'value'
            if(xml.name() == "value"){
                QString x=xml.readElementText();
                value=x.toDouble();
            }
        }
        xml.readNext();
    }

    // fine dell'elemento. aggiungo il dato letto alla lista 'informations'
    Data* n=new Data(value,label);
    informations->add(n);
}

// questo slot è usato da 'openFile()' per disegnare i grafici di un progetto appena aperto
void MainWindow::drawOpenData(){
    lineCanvas->update();
    barCanvas->update();
    pieCanvas->update();
}

// questo slot è usato da 'openFIle()' per popolare la tabella con i dati di un progetto appena aperto
void MainWindow::fillOpenTable(bool year){
    // se è un blank project creo nuova blank table
    if(!year){
         emit populateBlank(informations);
    }
    // se è year project creo year table
    else{
        emit populateYear(informations);
    }
}

void MainWindow::xmlSaverParser(QFile &file){
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    //scrivo tipo del progetto
    isYearProject? xmlWriter.writeStartElement("year")
                 : xmlWriter.writeStartElement("blank");
    xmlWriter.writeAttribute("rows",QString::number(tableValue->getRow()));
    //scrivo gli elementi e i loro valori
    list<Data*>::iterator it1=informations->first();
    list<Data*>::iterator it2=informations->last(); it2++;
    for(int i=0; i<tableValue->getRow() , it1 != it2; ++i, ++it1){
        Data* info=dynamic_cast<Data*>(*it1);
        xmlWriter.writeStartElement("element");
        xmlWriter.writeTextElement("label",info->getLabel());
        xmlWriter.writeTextElement("value",QString::number(info->getValue()));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // </year> or </blank>
}

// questo slot salva con nome un nuovo progetto
void MainWindow::saveFileAs(){
    QString filename=QFileDialog::getSaveFileName(this,tr("Save QCharts Project"),".",tr("Xml(*.xml)"));
    if(!filename.contains(".xml"))
        filename.append(".xml");
    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    //chiamo xmlSaverParser per salvare i valori
    xmlSaverParser(file);
    file.close();

    // setto il documento corrente a quello appena salvato
    filename = filename.remove(".xml");
    if(!filename.isEmpty()){
        QMessageBox::information(this,tr("Info"),tr("Project saved!"));
        currentFile=filename.append(".xml");
        documentModified=false;
    }
}

void MainWindow::saveFile(){
    if(documentModified == true && !currentFile.isEmpty()){
        QFile file(currentFile);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);

        //chiamo xmlSaverParser per salvare i valori
        xmlSaverParser(file);

        if(!currentFile.isEmpty())
            QMessageBox::information(this,tr("Info"),tr("Project saved!"));

        file.close();
        documentModified=false;
    }
    else{ // è la prima occorrenza del documento e quindi chiamo saveFileAs()
        saveFileAs();
    }
}

// questo slot salva il grafico corrente in un'immagine
void MainWindow::saveImg(){
    QPixmap pix;
    pix=QPixmap::grabWidget(chartTab->currentWidget());
    QString filename=QFileDialog::getSaveFileName(this,tr("Save Image"),".",tr("Png Files(*.png)"));
    if(!filename.isEmpty()){
        if(!pix.save(filename,"PNG",100)){
            QMessageBox::warning(this,tr("Error"),tr("The image has not be saved :/"));
        }
        else{
            QMessageBox::information(this,tr("Info"),tr("Image saved!"));
        }
    }
}

// questo slot viene invocato prima di chiudere l'applicazione
void MainWindow::exitApp(){
    if(documentModified){
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
              // è stato cliccato Save
            this->saveFileAs();
              break;

          case QMessageBox::Discard:
              // è stato cliccato Don't Save
            qApp->exit();
              break;
          case QMessageBox::Cancel:
              // è stato cliccato cancel
              break;
          default:
              // qui non arrivo mai
              break;
        }
    }
    else{
        // il documento non ha subito modifiche quindi esco subito
        qApp->quit();
    }
}

void MainWindow::askAboutNewProject(bool year){
    if(documentModified){
        QMessageBox msg;
        msg.setText("Continue and create a new project?");
        msg.setInformativeText("If you continue the previuos project will bew lost.");
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Save | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Ok);
        int ret = msg.exec();
        switch (ret) {
          case QMessageBox::Ok: {
               // e' stato cliccato Ok
                createNewTableWidget(year);
               break;
             }
          case QMessageBox::Save:
              // è stato cliccato Save
            this->saveFileAs();

              break;

          case QMessageBox::Cancel:
              // è stato cliccato cancel
              break;
          default:
              // qui non arrivo mai
              break;
        }
    }
    else{
        createNewTableWidget(year);
    }
}

//questi slot permettono di passare ad un nuovo progetto blank o year
void MainWindow::newBlank(){
    //richiamo
    askAboutNewProject(/* year=false*/);
}

void MainWindow::newYear(){
    askAboutNewProject(true);
}
//************END OF PRIVATE SLOTS************


//------------DEFINITION OF CONSTRUCTOR----------
MainWindow::MainWindow(QMainWindow *parent):QMainWindow(parent){

    informations = new Dataset();
    // all'inizio currentFile è 'empty'
    documentModified=false;

    isYearProject=false;
    createBlankTableWidget();

    createActions();
    createMenu();
    createToolBar();
    QWidget* w=new QWidget(this);
    mainLayout=new QGridLayout;
    chartTab=new QTabWidget(this);
    createChartTab();

    saveChartImg=new QPushButton(tr("Save Image"),this);
    connect(saveChartImg,SIGNAL(clicked()),this,SLOT(saveImg()));

    mainLayout->addWidget(chartTab,0,3);
    mainLayout->addWidget(tableValue,0,0);
    mainLayout->addWidget(saveChartImg,1,3);
    w->setLayout(mainLayout);
    setCentralWidget(w);
}
//***********END****************
