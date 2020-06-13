#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "robot.h"
#include "obstacle.h"

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

    //wczytanie danych z pliku bedacego logiem polaczenia bluetooth
    void wczytanie_danych_z_logu(unsigned long long czas_zmierzony);

    //aktualizacja danych na wykresie
    void aktualizuj_wykres(float rob_predkosc,float g_x,float g_y,float g_z,float rkom,float pkom, float x_kalman, unsigned long long czas);

    //inicjalizacja obslugi bluetooth
    void obsluga_bt();

    //informacje o nawiazanym polaczeniu
    void inicjalizuj_info();
    void informacje_bluetooth();

    // Dodanie przeszod
    void addObstaclesDefaultSet();

    // Wyswitlenie aktualnej pozycji robota
    void showCurrentRobotPos();

    //diody
    void tworz_diode();
    void zapal_czerwone();
    void zapal_zielone();


    //tworzenie wykresow
    void stworz_wykresy();

    //kolejne operacje tworzenia wykresow
    void utworz_osie();

    void utworz_serie();

    void utworz_wykresy();

    void wizualizuj_wykresy();

    void przypnij_serie_do_osi();

private:
    Ui::MainWindow *ui;

    // Animacja robota
    Robot *rob1;
    QGraphicsScene *RobotScene;
    QTimer *RobotTimer;

    // Przeszkody
    // Obstacle *obs;
    void addObstacle(int t_x, int t_y);

    //wyswietlanie wykresow
    QChartView* view_robot;
    QChartView* view_gyr_wykres_x;
    QChartView* view_gyr_wykres_y;
    QChartView* view_gyr_wykres_z;

    //serie z wykresow
    QLineSeries* series_robot;

    QLineSeries* series_gyr_wykres_x;
    QLineSeries* series_kom_wykres_x;
    QLineSeries* series_kalman_wykres_x;

    QLineSeries* series_gyr_wykres_y;
    QLineSeries* series_kom_wykres_y;

    QLineSeries* series_gyr_wykres_z;

    //osie czasu
    QValueAxis* timeline_robot;
    QValueAxis* timeline_gyr_x;
    QValueAxis* timeline_gyr_y;
    QValueAxis* timeline_gyr_z;

    //osie wartosci
    QValueAxis* robot_wartosi;
    QValueAxis* gyr_x_wartosci_y;
    QValueAxis* gyr_y_wartosci_y;
    QValueAxis* gyr_z_wartosci_y;


    //wykresy
    QChart* line_robot;
    QChart* line_gyr_wykres_x;
    QChart* line_gyr_wykres_y;
    QChart* line_gyr_wykres_z;

    //pomiar czasu
    QTime pomiar;

    //obsluga bt
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothSocket* socket;
    int stan_polaczenia;

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

    void on_robotSpeedFwd_pushButton_clicked();
    void on_robotSpeedBwd_pushButton_clicked();
    void on_robotTurnLeft_pushButton_clicked();
    void on_robotTurnRight_pushButton_clicked();
};

#endif // MAINWINDOW_H
