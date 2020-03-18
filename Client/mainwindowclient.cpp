#include "mainwindowclient.h"
#include "ui_client1.h"

MainWindowClient::MainWindowClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client1)
{
    ui->setupUi(this);

    connect(ui->pushButton,&QPushButton::released,this,&MainWindowClient::connexion);
}

void MainWindowClient::connexion(){
    auto splitting = ui->lineEdit->text().split(":");
    if(splitting.length()!=2){
        ui->lineEdit->setText("Format invalide, doit être adresse:port");
        return;
    }
    QHostAddress address(splitting[0]);
    if(QAbstractSocket::IPv4Protocol != address.protocol()){
        ui->lineEdit->setText("L'adresse doit être en IPv4");
        return;
    }
    bool val=false;
    quint16 port = splitting[1].toUInt(&val,10);
    if(val==false){
        ui->lineEdit->setText("Port non valide");
        return;
    }

    QTcpSocket* socket = new QTcpSocket(ui->centralwidget);
    connect(socket,&QTcpSocket::connected,this,&MainWindowClient::connexionEtab);
    socket->connectToHost(address.toString(),port);
    ui->lineEdit->setText("Connexion en cours");
}

void MainWindowClient::connexionEtab(){
    ui->lineEdit->setText("Connexion établie !");
}

MainWindowClient::~MainWindowClient()
{
    delete ui;
}

