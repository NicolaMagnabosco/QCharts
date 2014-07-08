#ifndef MAINWINDOW_H
 #define MAINWINDOW_H
#include <QPushButton>
#include <QMainWindow>
#include <QGridLayout>
#include "mytablewidget.h"
#include "linechart.h"
#include "piechart.h"
#include "barchart.h"
#include "dataset.h"
#include <vector>
 class QAction;
 class QMenu;
 class QPlainTextEdit;
 class QMessageBox;
 class QXmlStreamReader;
 class QFile;
class MainWindow : public QMainWindow{
 Q_OBJECT
public:
    MainWindow(QMainWindow* parent=0);
    void parseElement(QXmlStreamReader &);
    void xmlSaverParser(QFile&);
    void askAboutNewProject(bool =false);
    void createNewTableWidget(bool);
signals:
    void populateBlank(Dataset* &);
    void populateYear(Dataset* &);
private slots:
    void drawOpenData();
    void fillOpenTable(bool year=false);
    void receiveUpdateData(vector<Data>);
    void saveFile();
    void openFile();
    void saveFileAs();
    void saveImg();
    void newBlank();
    void newYear();
    void exitApp();
private:
    QString currentFile;
    bool documentModified;
    bool isYearProject;
    Dataset* informations;
    QAction* blank;
    QAction* year;
    QAction* open;
    QAction* save;
    QAction* saveAs;
    QAction* exit;
    QMenuBar* menu;
    QToolBar* toolbar;
    QTabWidget* chartTab;
    LineChart* lineCanvas;
    BarChart* barCanvas;
    PieChart* pieCanvas;
    QTabWidget* yearsContainer;
    MyTableWidget* tableValue;
    QMessageBox* startMsgBox;
    QGridLayout* mainLayout;
    QPushButton* saveChartImg;
    QPushButton* updateButton;
    void createActions();
    void createMenu();
    void createToolBar();
    void createBlankTableWidget();
    void createYearTableWidget();
    void createYearTabWidget();
    void createChartTab();
};

 #endif
