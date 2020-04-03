#include "mainwindowclient.h"
#include "ui_client1.h"
#include "ui_client2.h"

MainWindowClient::MainWindowClient(QWidget *parent)
    : QMainWindow(parent)
    , ui1(new Ui::Client1)
    , ui2(new Ui::Client2)
{
    ui1->setupUi(this);

    connect(ui1->pushButton,&QPushButton::clicked,this,&MainWindowClient::connexion);
}

void MainWindowClient::connexion(){
    if(comm_exists){
        communication->deleteLater();
    }
    ui1->pushButton->setEnabled(false);

    address = QHostAddress(ui1->ip1->text()+"."+ui1->ip2->text()+'.'+ui1->ip3->text()+"."+ui1->ip4->text());
    port = ui1->port_l->text().toInt();

    if(QAbstractSocket::IPv4Protocol != address.protocol()){
        ui1->label_status->setText("l'adresse IP n'est pas bonne");
        ui1->pushButton->setEnabled(true);
        return;
    }
    if(port>65535){
        ui1->label_status->setText("Port non valide");
        ui1->pushButton->setEnabled(true);
        return;
    }

    communication = new SocketCommun(address,port,this);
    comm_exists=true;
    connect(communication,&SocketCommun::demandeType,this,&MainWindowClient::demandeType,Qt::UniqueConnection);
    connect(communication,&SocketCommun::erreurCo,this,&MainWindowClient::erreurCo);
    communication->lancerCo();
    ui1->label_status->setText("Connexion en cours");
}

void MainWindowClient::demandeType(){
    disconnect(communication,&SocketCommun::erreurCo, this,&MainWindowClient::erreurCo);
    connect(communication,&SocketCommun::erreur,this,&MainWindowClient::erreur,Qt::UniqueConnection);

    if(ui1_exists) { ui2->setupUi(this); }
    ui1_exists = false;

    //Dialogue lancé qd ASK_TYPE arrive, se finit qd BOARD arrive
    connect(dialogueType, &DialogTypeJoueur::finished,this,&MainWindowClient::finDemandeType,Qt::UniqueConnection);
    dialogueType->open();
}

void MainWindowClient::finDemandeType(int res){
    joueur = (typeJoueur)res;
    if(res == Agent){
        setWindowTitle("Codenames - Agent");
    } else {
        setWindowTitle("Codenames - Espion");
    }
    changerBoard();
}

void MainWindowClient::changerBoard(){
    QCard* carte;
    int count = 0;
    for(auto i = liste_cartes->begin(); i < liste_cartes->end(); i++){
        ui2->gridLayout->removeWidget(*i);
        delete *i;
    }
    liste_cartes->clear();

    for(int i=0; i<25; i++){
        carte = new QCard(i,communication->plateau_courant[i].type,communication->plateau_courant[i].carte,liste_cartes,ui2->centralwidget);
        liste_cartes->emplace_back(carte);
        ui2->gridLayout->addWidget(carte,i%5+(int)(joueur==Espion),i/5);
        if(carte->getType()==Rouge) {count++;}
        if(joueur==Agent){
            connect(carte,&QCard::cardClicked,communication,&SocketCommun::sendGuess);
            if(carte->getType()!=SaisPas){ carte->setGuess();}
        }
    }
    if(joueur==Espion) {
       if(count ==9) {
           label_qui_commence->setText("Les rouges commencent");
           label_qui_commence->setStyleSheet("font-weight: bold; color: red");
       } else {
           label_qui_commence->setText("Les bleus commencent");
           label_qui_commence->setStyleSheet("font-weight: bold; color: blue");
       }
       ui2->gridLayout->addWidget(label_qui_commence,0,0,1,5,Qt::AlignCenter);
       ui2->gridLayout->setRowStretch(0,1);
       for(int i = 1; i<6; ui2->gridLayout->setRowStretch(i++,4));
   }
    connect(communication,&SocketCommun::carteUpdate,this,&MainWindowClient::guessCarte,Qt::UniqueConnection);

    //Ajout d'update pour pouvoir propager la taille opti du texte à tous les QCard
    this->update();
}

void MainWindowClient::guessCarte(char nb, typeCarte type){
    liste_cartes->at(nb)->setType(type);
    liste_cartes->at(nb)->setGuess();
}

void MainWindowClient::erreur(SocketCommun* comm, QAbstractSocket::SocketError err){
    qDebug() << "Erreur connexion : " << err;
    //TODO : mieux gérer erreur
    //Qd repart vers ui1, faire ui1_exists = true
    /*num_reco++;
    communication->deleteLater();
    communication = new SocketCommun(addr,prt,parent());
    label_co->setText(QString("<font color=red>Déconnecté, tentative de reconnexion %1</font>").arg(num_reco));
    ui2->menubar->setCornerWidget(label_co);
    connect(communication,&SocketCommun::coPrete,this,&MainWindowClient::demandeType);
    communication->lancerCo();
    reco_socket=communication;
    QTimer::singleShot(10*1000,this,&MainWindowClient::reconnexion);*/
}

void MainWindowClient::erreurCo(SocketCommun* socket, QAbstractSocket::SocketError err){
    //TODO : gérer erreur
}

/*void MainWindowClient::reconnexion(){
    if(communication->stateSocket()!=QAbstractSocket::ConnectedState && communication==reco_socket){
        erreur(communication,QAbstractSocket::UnknownSocketError);
    }
}*/

MainWindowClient::~MainWindowClient()
{
    delete ui1;
    delete ui2;
    if(comm_exists) {delete communication;}
    delete liste_cartes;
    delete dialogueType;
}

