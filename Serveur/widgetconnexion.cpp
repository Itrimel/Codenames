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
    QTcpSocket* nouv = server->nextPendingConnection();
    setText("<font color=green>Connecté</font>");
    menuBar->setCornerWidget(this);
}
