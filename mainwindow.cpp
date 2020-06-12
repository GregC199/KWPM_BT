#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //tworzymy diody
    tworz_diode();

    //zapalamy czerwony kolor
    zapal_czerwone();

}

MainWindow::~MainWindow()
{
    delete ui;
}
