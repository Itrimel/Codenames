#include "mainwindowclient.h"
#include "ui_client1.h"
#include "ui_client2.h"
#include <QLabel>

MainWindowClient::MainWindowClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client2)
    , dialogueType(new DialogTypeJoueur(this,&communication))
    , dialogueIP(new DialogIP(this,&address,&port,&communication))
    , label_qui_commence(new QLabel(this))
    , liste_cartes(new std::vector<QCard*>)
{
    ui->setupUi(this);
    connect(dialogueIP,&DialogIP::finished,this,&MainWindowClient::finDiagIP);
    dialogueIP->open();
}

void MainWindowClient::finDiagIP(int){
    connect(communication,&SocketCommun::erreur,this,&MainWindowClient::erreur);
    connect(communication,&SocketCommun::demandeType,this,&MainWindowClient::demandeType);
    demandeType();
}

void MainWindowClient::demandeType(){
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
    for(auto i = liste_cartes->begin(); i < liste_cartes->end(); i++){
        ui->gridLayout->removeWidget(*i);
        delete *i;
    }
    label_qui_commence->hide();
    liste_cartes->clear();

    for(int i=0; i<25; i++){
        carte = new QCard(i,communication->plateau_courant[i].type,communication->plateau_courant[i].carte,liste_cartes,ui->centralwidget);
        liste_cartes->emplace_back(carte);
        ui->gridLayout->addWidget(carte,i%5+1,i/5);
        if(communication->plateau_courant[i].guessed){
            carte->setGuess();
        }
        if(joueur==Agent){
            connect(carte,&QCard::cardClicked,communication,&SocketCommun::sendGuess);
        }
    }
    if(communication->bleuCommence) {
       label_qui_commence->setText("Les bleus commencent");
       label_qui_commence->setStyleSheet("font-weight: bold; color: blue");
    } else {
       label_qui_commence->setText("Les rouges commencent");
       label_qui_commence->setStyleSheet("font-weight: bold; color: red");
    }
    ui->gridLayout->addWidget(label_qui_commence,0,0,1,5,Qt::AlignCenter);
    ui->gridLayout->setRowStretch(0,1);
    label_qui_commence->show();
    for(int i = 1; i<6; ui->gridLayout->setRowStretch(i++,4));

    connect(communication,&SocketCommun::carteUpdate,this,&MainWindowClient::guessCarte,Qt::UniqueConnection);

    //Ajout d'update pour pouvoir propager la taille opti du texte à tous les QCard
    this->update();
}

void MainWindowClient::guessCarte(char nb, typeCarte type){
    liste_cartes->at(nb)->setType(type);
    liste_cartes->at(nb)->setGuess();
}

void MainWindowClient::erreur(SocketCommun*, QAbstractSocket::SocketError err){
    qDebug() << "Erreur connexion : " << err;
    dialogueType->closeFromParent();
    communication->deleteLater();
    dialogueIP->open();
}

MainWindowClient::~MainWindowClient()
{
    delete ui;
    if(comm_exists) {delete communication;}
    delete liste_cartes;
    delete dialogueType;
    delete dialogueIP;
    delete label_qui_commence;
}

