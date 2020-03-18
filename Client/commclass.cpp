#include "commclass.h"

CommClass::CommClass(QHostAddress adresse, quint16 port, QObject* parent = nullptr):
    adresse(adresse),
    port(port)
{
    socket = new QTcpSocket(parent);
}

void CommClass::lancerCo(){
    socket->connectToHost(adresse.toString(),port);
}

void CommClass::getNewBoard(){
    //TODO : Implémenter
}

void CommClass::sendMessage(char * message, int len){
    //TODO : Implémenter
}

void CommClass::readMessage(){
    //TODO : Implémenter. readyRead() et bytesAvailable()
}

CommClass::~CommClass(){
    socket->deleteLater();
}
