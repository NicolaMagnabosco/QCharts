 #include <QApplication>
#include "mainwindow.h"
#include <QSplashScreen>
#include <QTimer>
using namespace std;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QPixmap image(":/new/icons/qCharts.png");
    QSplashScreen splash(image,Qt::WindowStaysOnTopHint);

    splash.show();
    QTimer::singleShot(2000, &splash, SLOT(close()));

    MainWindow m;

    m.show();
    m.setWindowIcon(QIcon(":/new/icons/barChart.png"));
    m.showMaximized();

    return a.exec();

}
