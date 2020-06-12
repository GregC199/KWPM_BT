#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "robot.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Dodanie sceny
    RobotScene = new QGraphicsScene(this);
    ui->Robot_graphicsView->setScene(RobotScene);
    ui->Robot_graphicsView->setRenderHint(QPainter::Antialiasing);
    RobotScene->setSceneRect(-200, -200, 600, 450);

    QPen mypen = QPen(Qt::red);
    QLineF TopLine(RobotScene->sceneRect().topLeft(),
                   RobotScene->sceneRect().topRight());

    QLineF BottomLine(RobotScene->sceneRect().bottomLeft(),
                   RobotScene->sceneRect().bottomRight());

    QLineF LeftLine(RobotScene->sceneRect().topLeft(),
                   RobotScene->sceneRect().bottomLeft());

    QLineF RightLine(RobotScene->sceneRect().topRight(),
                   RobotScene->sceneRect().bottomRight());

    RobotScene->addLine(TopLine, mypen);
    RobotScene->addLine(BottomLine, mypen);
    RobotScene->addLine(RightLine, mypen);
    RobotScene->addLine(LeftLine, mypen);

    int robotCount = 1;
    for(int i = 0; i <robotCount; i++){
        Robot *rob1 = new Robot();
        RobotScene->addItem(rob1);
    }



    RobotTimer = new QTimer(this);
    connect(RobotTimer, SIGNAL(timeout()), RobotScene, SLOT(advance()));
    RobotTimer->start(100);

    //tworzymy diody
    tworz_diode();

    //zapalamy czerwony kolor
    zapal_czerwone();

    //wywolanie inicjalizacji obslugi komunikacji bluetooth
    obsluga_bt();

    //inicjalizacja danych o local hoscie
    this->host = new QBluetoothLocalDevice;

    inicjalizuj_info();




    //rozpoczecie zliczania czasu dla osi czasu na wykresach
    this->pomiar.start();

}

void MainWindow::inicjalizuj_info(){

    ui->naglowki_info_pol->clear();
    this->adres_local_host = this->host->address().toString();
    this->nazwa_local_host = this->host->name();
    this->adres_polaczanego = "Brak połączenia!";
    this->nazwa_polaczanego = "Brak połączenia!";
    this->uuid_polaczonego = "Brak połączenia!";
    this->typ_polaczenia = "Brak połączenia!";
    this->data_polaczenia = "Brak połączenia!";
    ui->naglowki_info_pol->append("Nazwa lokalnego hosta:");
    ui->naglowki_info_pol->append("Adres lokalnego hosta:");
    ui->naglowki_info_pol->append("Nazwa urządzenia połączonego:");
    ui->naglowki_info_pol->append("Adres urządzenia połączonego:");
    ui->naglowki_info_pol->append("Uuid urządzenia połączonego:");
    ui->naglowki_info_pol->append("Typ połączenia:");
    ui->naglowki_info_pol->append("Czas nawiązania połączenia:");


    informacje_bluetooth();
}

void MainWindow::informacje_bluetooth(){

    ui->nazwy_info_pol->clear();

    ui->nazwy_info_pol->append(this->nazwa_local_host);
    ui->nazwy_info_pol->append(this->adres_local_host);
    ui->nazwy_info_pol->append(this->nazwa_polaczanego);
    ui->nazwy_info_pol->append(this->adres_polaczanego);
    ui->nazwy_info_pol->append(this->uuid_polaczonego );
    ui->nazwy_info_pol->append(this->typ_polaczenia);
    ui->nazwy_info_pol->append(this->data_polaczenia);

}

MainWindow::~MainWindow()
{
    delete ui;
}
