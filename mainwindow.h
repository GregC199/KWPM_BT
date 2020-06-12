#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "robot.h"

#include <QtCore>
#include <QtGui>

// Obsulga animacji
#include <QGraphicsScene>

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

//obsluga operacji na plikach
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>


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

    // Animacja robota
    QGraphicsScene *AnimationScene;
    QTimer *animationTimer;
    Robot *rob1;

    //diody
    void tworz_diode();
    void zapal_czerwone();
    void zapal_zielone();

    //pomiar czasu
    QTime pomiar;

    //inicjalizacja obslugi bluetooth
    void obsluga_bt();

    //informacje o nawiazanym polaczeniu
    void inicjalizuj_info();
    void informacje_bluetooth();

    //obsluga bt
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothSocket* socket;

    //aktualizacja logu polaczenia
    void addToLogs(QString message);

    //zapis do logu w pliku
    std::ofstream zapis;
    std::ifstream czytanie;

    //informacje o bluetooth
    QBluetoothLocalDevice* host;
    QString nazwa_local_host;
    QString adres_local_host;
    QString nazwa_polaczanego;
    QString adres_polaczanego;
    QString uuid_polaczonego;
    QString typ_polaczenia;
    QString data_polaczenia;
private slots:

    //obsluga dzialania polaczenia z plytka
    void captureDeviceProperties(const QBluetoothDeviceInfo &device);

    void searchingFinished();

    void on_wyszukaj_urzadzenie_clicked();
    void on_polacz_przycisk_clicked();
    void on_rozlacz_przycisk_clicked();

    void connectionEstablished();
    void connectionInterrupted();

    void socketReadyToRead();

};

#endif // MAINWINDOW_H
