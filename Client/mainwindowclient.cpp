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
    if(comm_exists){
        communication->deleteLater();
    }
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

    communication = new SocketCommun(address,port,this);
    comm_exists=true;
    connect(communication,&SocketCommun::coPrete,this,&MainWindowClient::connexionEtab);
    communication->lancerCo();
    ui1->lineEdit->setText("Connexion en cours");
}

void MainWindowClient::connexionEtab(){
    num_reco=0;
    if(ui1_exists) {ui1->lineEdit->setText("Connexion établie ! Récup plateau en cours");}
    addr = communication->getAddr();
    prt = communication->getPrt();
    connect(communication,&SocketCommun::newBoard,this,&MainWindowClient::changerBoard);
    connect(communication,&SocketCommun::erreur,this,&MainWindowClient::erreur);
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
    ui1_exists=false;
    label_co->setText("<font color=green>Connecté</font>");
    ui2->menubar->setCornerWidget(label_co);
    premier_plateau=false;
    liste_cartes->clear();
    for(int i=0; i<25; i++){
        carte = new QCard(i,communication->plateau_courant[i].type,communication->plateau_courant[i].carte,liste_cartes,ui2->centralwidget);
        liste_cartes->emplace_back(carte);
        ui2->gridLayout->addWidget(carte,i%5,i/5);
        if(carte->getType()!=SaisPas){
            carte->setGuess();
        }
        connect(carte,&QCard::cardClicked,communication,&SocketCommun::sendGuess);
    }
    connect(communication,&SocketCommun::carteUpdate,this,&MainWindowClient::guessCarte,Qt::UniqueConnection);
}

void MainWindowClient::guessCarte(char nb, typeCarte type){
    liste_cartes->at(nb)->setType(type);
    liste_cartes->at(nb)->setGuess();
}

void MainWindowClient::erreur(SocketCommun* comm, QAbstractSocket::SocketError err){
    qDebug() << "Erreur connexion : " << err;
    num_reco++;
    communication->deleteLater();
    communication = new SocketCommun(addr,prt,parent());
    label_co->setText(QString("<font color=red>Déconnecté, tentative de reconnexion %1</font>").arg(num_reco));
    ui2->menubar->setCornerWidget(label_co);
    connect(communication,&SocketCommun::coPrete,this,&MainWindowClient::connexionEtab);
    communication->lancerCo();
    reco_socket=communication;
    QTimer::singleShot(10*1000,this,&MainWindowClient::reconnexion);
}

void MainWindowClient::reconnexion(){
    if(communication->stateSocket()!=QAbstractSocket::ConnectedState && communication==reco_socket){
        erreur(communication,QAbstractSocket::UnknownSocketError);
    }
}

MainWindowClient::~MainWindowClient()
{
    delete ui1;
    delete ui2;
    if(comm_exists) {delete communication;}
    delete liste_cartes;
}

