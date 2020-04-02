#include "widgetconnexion.h"


auto WidgetConnexion::findTypeJoueur(SocketCommun* socket, typeJoueur* joueur){
    auto iterator = std::find(connections_agents.begin(),connections_agents.end(),socket);
    auto iterator2 = std::find(connections_espions.begin(),connections_espions.end(),socket);
    if(iterator==connections_agents.end()){
        if(iterator2==connections_espions.end()){
            //ça ne devrait pas arriver
            //TODO : raise erreur ?
            qDebug() << "Pb : pas trouvé socket";
            *joueur = ErreurJoueur;
        } else {
            *joueur = Espion;
        }
        return iterator2;
    } else {
        *joueur = Agent;
        return iterator;
    }
}


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
    setText("<font color=red>Pas de connexion</font>");
    menuBar->setCornerWidget(this);
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
    for(auto i = connections_agents.begin();i<connections_agents.end();i++){
        delete *i;
    }
    for(auto i = connections_espions.begin();i<connections_espions.end();i++){
        delete *i;
    }
}

void WidgetConnexion::nouvCo(){
    if(etat==PasCo){
        clignotement_pas_co->stop();
    }
    etat=Co;
    SocketCommun* new_socket = new SocketCommun(server->nextPendingConnection());
    setText(QString("<font color=green>Connecté : %1</font>").arg(connections_agents.size()+connections_espions.size()));
    menuBar->setCornerWidget(this);
    connect(new_socket,&SocketCommun::erreur,this,&WidgetConnexion::gererErreur);
    connect(new_socket,&SocketCommun::guessRecu,this,&WidgetConnexion::gererGuess);
    connect(new_socket,&SocketCommun::askForBoard,this,&WidgetConnexion::sendBoard);
    connect(new_socket,&SocketCommun::nouvJoueur,this,&WidgetConnexion::nouvJoueur);
    new_socket->sendCoPrete();
}

void WidgetConnexion::sendBoard(SocketCommun* sock){
    typeJoueur joueur = ErreurJoueur;
    findTypeJoueur(sock,&joueur);

    if(joueur==ErreurJoueur){
        qDebug() << "Joueur non enregistré ou inconnu";
    }

    std::vector<data_carte>* plateau = new std::vector<data_carte>(25);
    for(int i=0; i<25; i++){
        plateau->at(i).carte = liste_cartes->at(i)->getMot();
        if(joueur==Espion || liste_cartes->at(i)->getGuess()){
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
    for(auto i = connections_agents.begin(); i<connections_agents.end(); i++){
        (*i)->sendUpdate(nb,liste_cartes->at(nb)->getType());
    }
    for(auto i = connections_espions.begin(); i<connections_espions.end(); i++){
        (*i)->sendUpdate(nb,liste_cartes->at(nb)->getType());
    }
}

void WidgetConnexion::resendBoard(){
    if(etat==Co){
        for(auto i = connections_agents.begin(); i<connections_agents.end(); i++){
            sendBoard(*i);
        }
        for(auto i = connections_espions.begin(); i<connections_espions.end(); i++){
            sendBoard(*i);
        }
    }
}

void WidgetConnexion::gererErreur(SocketCommun* origine, QAbstractSocket::SocketError erreur){
    qDebug() << erreur;
    //Gestion simple : on droppe sans réflechir
    typeJoueur joueur;
    auto iterator = findTypeJoueur(origine,&joueur);
    switch(joueur){
    case(Agent):
        connections_agents.erase(iterator);
        break;
    case(Espion):
        connections_espions.erase(iterator);
        break;
    case(ErreurJoueur):
        break;
    }
    origine->deleteLater();
    if(connections_agents.size()+connections_espions.size()==0){
        goPasCo();
    } else {
        setText(QString("<font color=green>Connecté : %1</font>").arg(connections_agents.size()+connections_espions.size()));
        menuBar->setCornerWidget(this);
    }
}

void WidgetConnexion::nouvJoueur(SocketCommun* socket, typeJoueur joueur){
    //TODO : permettre le changement de camp : pas partir du principe que le socket est pas attribué

    if(joueur==Agent){
        connections_agents.emplace_back(socket);
    } else if(joueur==Espion) {
        connections_espions.emplace_back(socket);
    }
    setText(QString("<font color=green>Connecté : %1</font>").arg(connections_agents.size()+connections_espions.size()));
    menuBar->setCornerWidget(this);
}


