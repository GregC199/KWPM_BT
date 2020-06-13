#include "mainwindow.h"



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

    // Utworzenie i dodanie robota do sceny
    rob1 = new Robot();
    RobotScene->addItem(rob1);

    RobotTimer = new QTimer(this);
    connect(RobotTimer, SIGNAL(timeout()), RobotScene, SLOT(advance()));
    RobotTimer->start(100);

    // Utworzenie i dodanie przeszkody
    obs1 = new Obstacle(100,100);
    RobotScene->addItem(obs1);

    //tworzymy diody
    tworz_diode();

    //tworzenie wykresow
    stworz_wykresy();

    //wywolanie inicjalizacji obslugi komunikacji bluetooth
    obsluga_bt();

    //inicjalizacja danych o local hoscie
    this->host = new QBluetoothLocalDevice;

    inicjalizuj_info();

    //nadpisanie/utworzenie pliku log_polaczenia.txt
    zapis.open("log_polaczenia.txt",std::ios_base::out);
    zapis.close();

    //otwarcie strumienia zapisu danych typu dodawania na koniec pliku
    zapis.open("log_polaczenia.txt", std::ios_base::app);

    //otwarcie czytania z pliku log_polaczenia.txt
    czytanie.open("log_polaczenia.txt",std::ios_base::in);


    //rozpoczecie zliczania czasu dla osi czasu na wykresach
    this->pomiar.start();

    //ustawienie statusu polaczenia jako startowego widgetu
    ui->tabWidget->setCurrentWidget(ui->Status_polaczenia);

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

void MainWindow::wczytanie_danych_z_logu(unsigned long long czas_zmierzony){

    //zmienne pomocnicze do realizacji zczytywania danych
    char a[10],b[10],c[10],d[10],e[10],f[10],g[10],h[10],i[10],j[10],k[10];
    float  acc_x = 0.0;
    float  acc_y = 0.0;
    float  acc_z = 0.0;
    float  gyr_x = 0.0;
    float  gyr_y = 0.0;
    float  gyr_z = 0.0;
    float  roll = 0.0;
    float  pitch = 0.0;
    float  robot_predkosc = 0.0;
    float  kalman_x = 0.0;

    //zczytywanie kolejnych danych z pliku
    czytanie >> a;
    czytanie >> acc_x;
    czytanie >> b;
    czytanie >> acc_y;
    czytanie >> c;
    czytanie >> acc_z;
    czytanie >> d;
    czytanie >> gyr_x;
    czytanie >> e;
    czytanie >> gyr_y;
    czytanie >> f;
    czytanie >> gyr_z;
    czytanie >> g;
    czytanie >> roll;
    czytanie >> h;
    czytanie >> pitch;
    czytanie >> i;
    czytanie >> kalman_x;
    czytanie >> j;
    czytanie >> k;

    std::cout << "a:" << a << " " << gyr_x << " "<<  gyr_y << " "<< gyr_z << std::endl;


    aktualizuj_wykres(robot_predkosc,gyr_x,gyr_y,gyr_z,roll,pitch,kalman_x,czas_zmierzony);

}

void MainWindow::aktualizuj_wykres(float rob_predkosc,float g_x,float g_y,float g_z,float rkom,float pkom, float x_kalman, unsigned long long czas){

    //os czasu - zmienne pomocnicze sluzace do jej przesuwania
    long double test = ((long double)czas)/1000;
    long double koniec = 0.0;
    long double memory;
    long double poczatek = 0.0;

    //przesuniecie osi czasu
    while(koniec < test){
        memory=koniec;
        koniec=koniec+30;
    }
    if(koniec>30.0)poczatek = ceil(memory);

    //sprawdzenie czy nastąpiła zmiana osi czasy
    if(timeline_robot->max() != ceil(koniec)){

        this->timeline_robot->setRange(poczatek,ceil(koniec));
        this->timeline_gyr_x->setRange(poczatek,ceil(koniec));
        this->timeline_gyr_y->setRange(poczatek,ceil(koniec));
        this->timeline_gyr_z->setRange(poczatek,ceil(koniec));
    }

    //dodawanie nowych punktow na wykresie
    //robot
    this->series_robot->append(test,rob_predkosc);

    //gyrx
    this->series_gyr_wykres_x->append(test,g_x);

    this->series_kom_wykres_x->append(test,rkom);

    this->series_kalman_wykres_x->append(test,x_kalman);

    //gyry
    this->series_gyr_wykres_y->append(test,g_y);

    this->series_kom_wykres_y->append(test,pkom);

    //gyrz
    this->series_gyr_wykres_z->append(test,g_z);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//////////////////////////////////////////////////////
// Przyciski sterowania robotem z poziomu aplikacji //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Zmiana prędkości robota o +/-1
void MainWindow::on_robotSpeedFwd_pushButton_clicked()
{
    int newRobotSpeed = 0;

    newRobotSpeed = this->rob1->getCurrentRobotSpeed();
    newRobotSpeed += 1;

    if(newRobotSpeed < 10){
        this->rob1->setRobotSpeed(newRobotSpeed);
    }
}

void MainWindow::on_robotSpeedBwd_pushButton_clicked()
{
    int newRobotSpeed;

    newRobotSpeed = this->rob1->getCurrentRobotSpeed();
    newRobotSpeed -= 1;

    // Predkość -1, aby była maożliwość wycofania po kolizji
    if(newRobotSpeed >= -1){
        this->rob1->setRobotSpeed(newRobotSpeed);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Zmiana orientacji robota o 5 stopni w lewo/prawo
void MainWindow::on_robotTurnLeft_pushButton_clicked()
{
    int t_currentAngle = this->rob1->getCurrentRobotAngle();
    t_currentAngle -= 5;

    // Podstawienie t_currentAngle, za CurrentRobotAngle w klasie Robot
    this->rob1->setRobotAngle(t_currentAngle);
}

void MainWindow::on_robotTurnRight_pushButton_clicked()
{
    int t_currentAngle = this->rob1->getCurrentRobotAngle();
    t_currentAngle += 5;

    this->rob1->setRobotAngle(t_currentAngle);
}
