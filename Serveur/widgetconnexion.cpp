#include "widgetconnexion.h"



WidgetConnexion::WidgetConnexion(QWidget* parent,QMenuBar* menuBar, std::vector<QCard*>* liste):
    QLabel(parent),
    menuBar(menuBar),
    liste_cartes(liste)
{
    clignotement_pas_co=new QTimer(this);
    connect(clignotement_pas_co,&QTimer::timeout,this,&WidgetConnexion::textePasCo);
    goPasCo();

    //initialisation connexion
    server = new QTcpServer();
    if(!server->listen(QHostAddress::Any,PORT_SERVEUR)){
        qDebug() << "échec connexion : ne peut pas écouter les connexions entrantes";
        clignotement_pas_co->stop();
        setText("<font color=red>Erreur !</font>");
        menuBar->setCornerWidget(this);
    }
    connect(server,&QTcpServer::newConnection,this,&WidgetConnexion::nouvCo);
}

void WidgetConnexion::goPasCo(){
    etat=PasCo;
    clignotement_pas_co->start(600);
}

void WidgetConnexion::textePasCo(){
    clign=!clign;
    if(clign){
        setText("<font color=red>Pas de connexion</font>");
    } else {
        setText("<font color=red>⬤ Pas de connexion</font>");
    }
    menuBar->setCornerWidget(this);
}

WidgetConnexion::~WidgetConnexion(){
    if(etat==PasCo){
        clignotement_pas_co->stop();
    }
    clignotement_pas_co->deleteLater();
    server->deleteLater();
    for(auto i = connections.begin();i<connections.end();i++){
        delete *i;
    }
}

void WidgetConnexion::nouvCo(){
    if(etat==PasCo){
        clignotement_pas_co->stop();
    }
    etat=Co;
    SocketCommun* new_socket = new SocketCommun(server->nextPendingConnection());
    connections.emplace_back(new_socket);
    setText("<font color=green>Connecté</font>");
    menuBar->setCornerWidget(this);
    connect(new_socket,&SocketCommun::erreur,this,&WidgetConnexion::gererErreur);
    connect(new_socket,&SocketCommun::guessRecu,this,&WidgetConnexion::gererGuess);
    connect(new_socket,&SocketCommun::askForBoard,this,&WidgetConnexion::sendBoard);
}

void WidgetConnexion::sendBoard(SocketCommun* sock){
    std::vector<data_carte>* plateau = new std::vector<data_carte>(25);
    bool isGuessed;
    for(int i=0; i<25; i++){
        isGuessed = liste_cartes->at(i)->getType();
        plateau->at(i).carte = liste_cartes->at(i)->getMot();
        if(liste_cartes->at(i)->getGuess()){
            plateau->at(i).type = liste_cartes->at(i)->getType();
        } else {
            plateau->at(i).type = SaisPas;
        }
    }
    sock->sendBoard(plateau);
    delete plateau;
}

void WidgetConnexion::gererGuess(SocketCommun* origine, char nb){
    liste_cartes->at(nb)->setGuess();
    for(auto i = connections.begin(); i<connections.end(); i++){
        (*i)->sendUpdate(nb,liste_cartes->at(nb)->getType());
    }
}

void WidgetConnexion::resendBoard(){
    if(etat==Co){
        for(auto i = connections.begin(); i<connections.end(); i++){
            sendBoard(*i);
        }
    }
}

void WidgetConnexion::gererErreur(SocketCommun* origine,QAbstractSocket::SocketError erreur){
    qDebug() << erreur;
}
