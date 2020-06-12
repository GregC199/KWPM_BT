#include "mainwindow.h"

void MainWindow::obsluga_bt(){

    this->discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    this->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(captureDeviceProperties(QBluetoothDeviceInfo)));

    connect(this->discoveryAgent, SIGNAL(finished()),this, SLOT(searchingFinished()));

    connect(this->socket, SIGNAL(connected()),this, SLOT(connectionEstablished()));
    connect(this->socket, SIGNAL(disconnected()),this, SLOT(connectionInterrupted()));
    connect(this->socket, SIGNAL(readyRead()),this, SLOT(socketReadyToRead()));
    ui->rozlacz_przycisk->setEnabled(false);
}

void MainWindow::on_wyszukaj_urzadzenie_clicked()
{
    ui->lista_urzadzen->clear();
    ui->wyszukaj_urzadzenie->setEnabled(false);

    this->addToLogs("Szukam urzadzen...");

    this->discoveryAgent->start();
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->log_polaczenia->append(currentDateTime + "\t" + message);
}

void MainWindow::captureDeviceProperties(const QBluetoothDeviceInfo &device) {
  ui->lista_urzadzen->addItem(device.name() + " " + device.address().toString());
  this->addToLogs("Znaleziono urządzenie o nazwie: " + device.name() + " i adresie: " + device.address().toString());
}
void MainWindow::searchingFinished() {
  ui->wyszukaj_urzadzenie->setEnabled(true);
  this->addToLogs("Wyszukiwanie zakonczone");
  ui->wyszukaj_urzadzenie->setEnabled(true);

}

void MainWindow::on_polacz_przycisk_clicked()
{
    QString comboBoxQString = ui->lista_urzadzen->currentText();
    QStringList portList = comboBoxQString.split(" ");
    QString deviceAddres = portList.last();
    QString deviceName = portList.first();

    static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));

    //zapis danych do informacji o polaczeniu
    this->uuid_polaczonego = serviceUuid;
    this->adres_polaczanego = deviceAddres;
    this->typ_polaczenia = "Read/Write";
    this->nazwa_polaczanego = deviceName;

    this->socket->connectToService(QBluetoothAddress(deviceAddres),QBluetoothUuid(serviceUuid),QIODevice::ReadWrite);
    this->addToLogs("Laczenie z urządzeniem o nazwie: " + deviceName + " i adresie: " + deviceAddres);

}

void MainWindow::on_rozlacz_przycisk_clicked()
{
    this->addToLogs("Zamykam polaczenie");
    this->socket->disconnectFromService();
}

void MainWindow::connectionEstablished() {

    //zapis daty nawiazania polaczenia
    this->data_polaczenia = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");

    this->addToLogs("Polaczenie ustanowione");

    //deaktywacja przycisku połącz
    ui->polacz_przycisk->setEnabled(false);
    ui->rozlacz_przycisk->setEnabled(true);

    //aktualizacja informacji bluetooth
    informacje_bluetooth();

    //zapalanie zielonej
    zapal_zielone();


}

void MainWindow::connectionInterrupted() {
    this->addToLogs("Polaczenie przerwane");
    ui->polacz_przycisk->setEnabled(true);
    ui->rozlacz_przycisk->setEnabled(false);

    //reset informacji o polaczeniu do poczatkowych wartosci
    inicjalizuj_info();

    zapal_czerwone();
}

void MainWindow::socketReadyToRead() {

    unsigned long long zmierzony;
    while(this->socket->canReadLine()) {

        //zapis ilosci milisekund uplynietych od startu programu
        zmierzony = (unsigned long long) this->pomiar.elapsed();

        //zczytanie danych z socketu bluetooth
        QString line = this->socket->readLine();

        //zmienna pomocniczna do zapisu danych do logu polaczenia
        std::string zczytanie;

        //przeksztalcenie QString do std::string
        zczytanie = line.toStdString();

        //zapis danych z bluetooth do pliku
        zapis << zczytanie<<std::endl;


        //dodanie terminatora na koniec
        QString terminator = "\r";
        int pos = line.lastIndexOf(terminator);

        this->addToLogs(line.left(pos));
      }
}
