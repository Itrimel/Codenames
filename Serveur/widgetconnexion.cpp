#include "widgetconnexion.h"



WidgetConnexion::WidgetConnexion(QWidget* parent,QMenuBar* menuBar):
    QLabel(parent),
    menuBar(menuBar)
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
}

void WidgetConnexion::nouvCo(){
    if(etat==PasCo){
        clignotement_pas_co->stop();
    }
    etat=Co;
    socket = server->nextPendingConnection();
    setText("<font color=green>Connecté</font>");
    menuBar->setCornerWidget(this);
    connect(socket,&QTcpSocket::readyRead,this,&WidgetConnexion::receptionMessage);
}

void WidgetConnexion::receptionMessage(){
    message_header header;
    while(socket->bytesAvailable()){
        socket->read(reinterpret_cast<char*>(&header), sizeof(message_header));
        char buffer[header.length];
        socket->read(buffer,header.length);
        switch(header.type){
        case(MSG_TYPE_NOP):
            break;
        case(MSG_TYPE_GUESS):
            //TODO: Implémenter réception guess
            break;
        case(MSG_TYPE_PING):
            //TODO : Implémenter ping
            break;
        case(MSG_TYPE_GET_BOARD):
            //TODO : Implémenter envoi plateau
            sendBoard(socket);
            break;
        case(MSG_TYPE_OK):
            //TODO : Implémenter (peut-être) OK
            break;
        default:
            //Implement pas normal
            break;
        }
    }
}

void WidgetConnexion::sendBoard(QTcpSocket * sock){
    char buffer[1000]; //Ca devrait aller, il y a pas de mots de plus de 35 caractères en français
    int position=0,len;
    std::string mot;
    message_header header;
    header.type=MSG_TYPE_BOARD;
    for(int i=0; i<25; i++){
        mot = liste_cartes->at(i)->getMot().toStdString();
        len = mot.length();
        memcpy(buffer+position+sizeof(header),mot.data(),len+1);
        position+=len+1;
        buffer[position-1+sizeof(header)]=0;
        buffer[position+sizeof(header)]=(char)liste_cartes->at(i)->getType();
        buffer[position+1+sizeof(header)]=0;
        position+=2;
    }
    header.length=position;
    memcpy(buffer,&header,sizeof(header));
    socket->write(buffer,position);
}
