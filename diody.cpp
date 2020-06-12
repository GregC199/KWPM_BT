#include "mainwindow.h"


void MainWindow::tworz_diode(){

    //dioda 1 - tab "menu glowne"
    //dioda 2 - tab "status polaczenia"
    ui->dioda1->setFixedHeight(150);
    ui->dioda1->setFixedWidth(150);
    ui->dioda2->setFixedHeight(150);
    ui->dioda2->setFixedWidth(150);


    //utworzenie ksztaltu kola
    QRect ksztalt(0,0,150,150);
    QRegion kolo(ksztalt, QRegion::Ellipse);

    //nadanie ksztaltu kola diodom
    ui->dioda1->setMask(kolo);
    ui->dioda2->setMask(kolo);

    //zapalenie czerwonej diody
    zapal_czerwone();
}

void MainWindow::zapal_czerwone(){

    //dioda prawa ustawienie koloru
    ui->dioda1->setStyleSheet("QPushButton:flat { color: rgb(255, 0, 0); background-color: rgb(255, 0, 0); border: none;}");
    ui->dioda2->setStyleSheet("QPushButton:flat { color: rgb(255, 0, 0); background-color: rgb(255, 0, 0); border: none;}");

}

void MainWindow::zapal_zielone(){

    //dioda lewa ustawienie koloru
    ui->dioda1->setStyleSheet("QPushButton:flat { color: rgb(38, 255, 0); background-color: rgb(38, 255, 0);  border: none; }");
    ui->dioda2->setStyleSheet("QPushButton:flat { color: rgb(38, 255, 0); background-color: rgb(38, 255, 0);  border: none; }");

}
