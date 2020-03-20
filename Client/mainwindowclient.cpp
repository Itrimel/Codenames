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
    ui1->lineEdit->setText("127.0.0.1:8081");
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
    connect(communication,&CommClass::newBoard,this,&MainWindowClient::changerBoard);
    connect(communication->socket,&QTcpSocket::readyRead,communication,&CommClass::readMessage);
    communication->getNewBoard();
}


void MainWindowClient::changerBoard(){
    QCard* carte;
    if(!premier_plateau){
        for(auto i = liste_cartes->begin(); i < liste_cartes->end(); i++){
            ui2->gridLayout->removeWidget(*i);
            delete *i;
        }
    }
    ui2->setupUi(this);
    premier_plateau=false;
    liste_cartes->clear();
    for(int i=0; i<25; i++){
        carte = new QCard(i,communication->plateau_courant[i].type,communication->plateau_courant[i].carte,liste_cartes,ui2->centralwidget);
        liste_cartes->emplace_back(carte);
        ui2->gridLayout->addWidget(carte,i%5,i/5);
        if(carte->getType()!=SaisPas){
            carte->setGuess();
        }
        connect(carte,&QCard::cardClicked,communication,&CommClass::sendGuess);
    }
    connect(communication,&CommClass::carteUpdate,this,&MainWindowClient::guessCarte,Qt::UniqueConnection);
}

void MainWindowClient::guessCarte(char nb, typeCarte type){
    liste_cartes->at(nb)->setType(type);
    liste_cartes->at(nb)->setGuess();
}


MainWindowClient::~MainWindowClient()
{
    delete ui1;
    delete ui2;
    if(comm_exists) {delete communication;}
    delete liste_cartes;
}

