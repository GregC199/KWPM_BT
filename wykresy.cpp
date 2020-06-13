#include "mainwindow.h"

void MainWindow::utworz_osie(){

    //zmiana rozmiarow czcionki w celu poprawienia widoczności wartości na osiach
    QFont czcionka_osi;
    czcionka_osi.setPixelSize(9);

    //tworzenie osi czasu gyr x,y,z
    this->timeline_gyr_x = new QValueAxis;
    this->timeline_gyr_x->setLabelFormat("%.2f");
    this->timeline_gyr_x->setRange(0,10);
    this->timeline_gyr_x->setTickCount(6);
    this->timeline_gyr_x->setLabelsFont(czcionka_osi);


    this->timeline_gyr_y = new QValueAxis;
    this->timeline_gyr_y->setLabelFormat("%.2f");
    this->timeline_gyr_y->setRange(0,10);
    this->timeline_gyr_y->setTickCount(6);
    this->timeline_gyr_y->setLabelsFont(czcionka_osi);


    this->timeline_gyr_z = new QValueAxis;
    this->timeline_gyr_z->setLabelFormat("%.2f");
    this->timeline_gyr_z->setRange(0,10);
    this->timeline_gyr_z->setTickCount(6);
    this->timeline_gyr_z->setLabelsFont(czcionka_osi);

    //robot
    this->timeline_robot = new QValueAxis;
    this->timeline_robot->setLabelFormat("%.2f");
    this->timeline_robot->setRange(0,10);
    this->timeline_robot->setTickCount(6);
    this->timeline_robot->setLabelsFont(czcionka_osi);


    //tworzenie osi wartosci robot
    this->robot_wartosi = new QValueAxis;
    this->robot_wartosi->setLabelFormat("%.2f");
    this->robot_wartosi->setRange(-1,9);
    this->robot_wartosi->setTickCount(6);
    this->robot_wartosi->setLabelsFont(czcionka_osi);


    //tworzenie osi wartosci gyr x,y,z
    this->gyr_x_wartosci_y = new QValueAxis;
    this->gyr_x_wartosci_y->setLabelFormat("%.2f");
    this->gyr_x_wartosci_y->setRange(-360,360);
    this->gyr_x_wartosci_y->setTickCount(9);
    this->gyr_x_wartosci_y->setLabelsFont(czcionka_osi);

    this->gyr_y_wartosci_y = new QValueAxis;
    this->gyr_y_wartosci_y->setLabelFormat("%.2f");
    this->gyr_y_wartosci_y->setRange(-360,360);
    this->gyr_y_wartosci_y->setTickCount(9);
    this->gyr_y_wartosci_y->setLabelsFont(czcionka_osi);

    this->gyr_z_wartosci_y = new QValueAxis;
    this->gyr_z_wartosci_y->setLabelFormat("%.2f");
    this->gyr_z_wartosci_y->setRange(-360,360);
    this->gyr_z_wartosci_y->setTickCount(9);
    this->gyr_z_wartosci_y->setLabelsFont(czcionka_osi);
}

void MainWindow::utworz_serie(){

    //tworzenie serii
    //robot
    this->series_robot = new QLineSeries();
    this->series_robot->append(0,0);

    //gyr x, roll, kalman x, y, pitch, z
    this->series_gyr_wykres_x = new QLineSeries();
    this->series_gyr_wykres_x->append(0,0);
    this->series_gyr_wykres_x->setName("X_surowe");

    this->series_kom_wykres_x = new QLineSeries();
    this->series_kom_wykres_x->append(0,0);
    this->series_kom_wykres_x->setName("X_kom");

    this->series_kalman_wykres_x = new QLineSeries();
    this->series_kalman_wykres_x->append(0,0);
    this->series_kalman_wykres_x->setName("X_kal");


    this->series_gyr_wykres_y = new QLineSeries();
    this->series_gyr_wykres_y->append(0,0);
    this->series_gyr_wykres_y->setName("Y_surowe");


    this->series_kom_wykres_y = new QLineSeries();
    this->series_kom_wykres_y->append(0,0);
    this->series_kom_wykres_y->setName("Y_kom");


    this->series_gyr_wykres_z = new QLineSeries();
    this->series_gyr_wykres_z->append(0,0);
}

void MainWindow::utworz_wykresy(){
    //stworzenie wykresu liniowego z serii - acc_x
    this->line_robot = new QChart();

    //dodanie osi
    this->line_robot->addAxis(this->robot_wartosi,Qt::AlignLeft);
    this->line_robot->addAxis(this->timeline_robot,Qt::AlignBottom);

    //dodanie serii danych
    this->line_robot->addSeries(this->series_robot);

    //dodanie tytulu wykresu
    this->line_robot->setTitle("Prędkość robota");

    //wylaczenie legendy
    this->line_robot->legend()->hide();




    //gyr_x
    this->line_gyr_wykres_x = new QChart();

    this->line_gyr_wykres_x->addAxis(this->gyr_x_wartosci_y,Qt::AlignLeft);
    this->line_gyr_wykres_x->addAxis(this->timeline_gyr_x,Qt::AlignBottom);

    this->line_gyr_wykres_x->addSeries(this->series_gyr_wykres_x);
    this->line_gyr_wykres_x->addSeries(this->series_kom_wykres_x);
    this->line_gyr_wykres_x->addSeries(this->series_kalman_wykres_x);

    this->line_gyr_wykres_x->setTitle("Zczytania wzdłuż osi X");

    this->line_gyr_wykres_x->legend()->setAlignment(Qt::AlignLeft);




    //gyr_y
    this->line_gyr_wykres_y = new QChart();

    this->line_gyr_wykres_y->addAxis(this->gyr_y_wartosci_y,Qt::AlignLeft);
    this->line_gyr_wykres_y->addAxis(this->timeline_gyr_y,Qt::AlignBottom);

    this->line_gyr_wykres_y->addSeries(this->series_gyr_wykres_y);
    this->line_gyr_wykres_y->addSeries(this->series_kom_wykres_y);

    this->line_gyr_wykres_y->setTitle("Zczytania wzdłuż osi Y");

    this->line_gyr_wykres_y->legend()->setAlignment(Qt::AlignLeft);




    //gyr_z
    this->line_gyr_wykres_z = new QChart();

    this->line_gyr_wykres_z->addAxis(this->gyr_z_wartosci_y,Qt::AlignLeft);
    this->line_gyr_wykres_z->addAxis(this->timeline_gyr_z,Qt::AlignBottom);

    this->line_gyr_wykres_z->addSeries(this->series_gyr_wykres_z);

    this->line_gyr_wykres_z->setTitle("Zczytania wzdłuż osi Z");

    this->line_gyr_wykres_z->legend()->hide();
}

void MainWindow::wizualizuj_wykresy(){

    //robot
    this->view_robot = new QChartView(this->line_robot);
    this->view_robot->setRenderHint(QPainter::Antialiasing);
    this->view_robot->setParent(ui->predkosc_robota);

    //gyr_x
    this->view_gyr_wykres_x = new QChartView(this->line_gyr_wykres_x);
    this->view_gyr_wykres_x->setRenderHint(QPainter::Antialiasing);
    this->view_gyr_wykres_x->setParent(ui->wykres_x);

    //gyr_y
    this->view_gyr_wykres_y = new QChartView(this->line_gyr_wykres_y);
    this->view_gyr_wykres_y->setRenderHint(QPainter::Antialiasing);
    this->view_gyr_wykres_y->setParent(ui->wykres_y);

    //gyr_z
    this->view_gyr_wykres_z = new QChartView(this->line_gyr_wykres_z);
    this->view_gyr_wykres_z->setRenderHint(QPainter::Antialiasing);
    this->view_gyr_wykres_z->setParent(ui->wykres_z);
}

void MainWindow::przypnij_serie_do_osi(){
    //laczenie osi czasu z seriami danych
    this->series_robot->attachAxis(this->timeline_robot);

    this->series_gyr_wykres_x->attachAxis(this->timeline_gyr_x);
    this->series_kom_wykres_x->attachAxis(this->timeline_gyr_x);
    this->series_kalman_wykres_x->attachAxis(this->timeline_gyr_x);

    this->series_gyr_wykres_y->attachAxis(this->timeline_gyr_y);
    this->series_kom_wykres_y->attachAxis(this->timeline_gyr_y);

    this->series_gyr_wykres_z->attachAxis(this->timeline_gyr_z);

    //laczenie osi wartosci z seriami danych
    this->series_robot->attachAxis(this->robot_wartosi);

    this->series_gyr_wykres_x->attachAxis(this->gyr_x_wartosci_y);
    this->series_kom_wykres_x->attachAxis(this->gyr_x_wartosci_y);
    this->series_kalman_wykres_x->attachAxis(this->gyr_x_wartosci_y);

    this->series_gyr_wykres_y->attachAxis(this->gyr_y_wartosci_y);
    this->series_kom_wykres_y->attachAxis(this->gyr_y_wartosci_y);

    this->series_gyr_wykres_z->attachAxis(this->gyr_z_wartosci_y);
}

void MainWindow::stworz_wykresy(){

    this->utworz_serie();

    this->utworz_osie();

    this->utworz_wykresy();

    this->przypnij_serie_do_osi();

    this->wizualizuj_wykresy();

    //dostosowanie startowego rozmiaru wykresow - robot
    ui->predkosc_robota->setGeometry(13,13,441,198);

    //gyr
    ui->wykres_x->setGeometry(13,13,441,198);
    ui->wykres_y->setGeometry(13,13,441,198);
    ui->wykres_z->setGeometry(13,13,441,198);

}

