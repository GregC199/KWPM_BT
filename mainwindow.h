#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"



//wykresy
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QResizeEvent>
#include <QFont>

//bluetooth
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>
#include <QDebug>
#include <QDateTime>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //diody
    void tworz_diode();
    void zapal_czerwone();
    void zapal_zielone();
};

#endif // MAINWINDOW_H
