#include "mainwindowclient.h"
#include "ui_client1.h"
#include "ui_client2.h"

MainWindowClient::MainWindowClient(QWidget *parent)
    : QMainWindow(parent)
    , ui1(new Ui::Client1)
    , ui2(new Ui::Client2)
{
    ui1->setupUi(this);

    connect(ui1->pushButton,&QPushButton::released,this,&MainWindowClient::connexion);
}

void MainWindowClient::connexion(){
    auto splitting = ui1->lineEdit->text().split(":");
    if(splitting.length()!=2){
        ui1->lineEdit->setText("Format invalide, doit être adresse:port");
        return;
    }
    QHostAddress address(splitting[0]);
    if(QAbstractSocket::IPv4Protocol != address.protocol()){
        ui1->lineEdit->setText("L'adresse doit être en IPv4");
        return;
    }
    bool val=false;
    quint16 port = splitting[1].toUInt(&val,10);
    if(val==false){
        ui1->lineEdit->setText("Port non valide");
        return;
    }

    communication = new CommClass(address,port,this);
    comm_exists=true;
    connect(communication->socket,&QTcpSocket::connected,this,&MainWindowClient::connexionEtab);
    communication->lancerCo();
    ui1->lineEdit->setText("Connexion en cours");
}

void MainWindowClient::connexionEtab(){
    ui1->lineEdit->setText("Connexion établie ! Récup plateau en cours");
    connect(communication,&CommClass::coEtablie,this,&MainWindowClient::changerBoard);
}

void MainWindowClient::changerBoard(){
    //TODO : Implémenter
}


MainWindowClient::~MainWindowClient()
{
    delete ui1;
    delete ui2;
    if(comm_exists) {delete communication;}
}

