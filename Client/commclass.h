#ifndef COMMCLASS_H
#define COMMCLASS_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include "Commun/protocole.h"

class CommClass : public QObject
{
    Q_OBJECT

public:
    CommClass(QHostAddress, quint16, QObject*);
    ~CommClass();
    QTcpSocket* socket;
    void lancerCo();
    void getNewBoard();
signals:
    void coEtablie();
    void newBoard();
private:
    const QHostAddress adresse;
    const quint16 port;
    void sendMessage(char*,int);
    void readMessage();
};

#endif // COMMCLASS_H
