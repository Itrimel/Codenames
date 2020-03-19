#ifndef COMMCLASS_H
#define COMMCLASS_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include <QVector>
#include "Commun/protocole.h"

struct data_carte{
    QString carte;
    typeCarte type;
};

class CommClass : public QObject
{
    Q_OBJECT

public:
    CommClass(QHostAddress, quint16, QObject*);
    ~CommClass();
    QTcpSocket* socket;
    void lancerCo();
    void getNewBoard();
    QVector<data_carte> plateau_courant=QVector<data_carte>(25);
    void readMessage();
signals:
    void coEtablie();
    void newBoard();
public slots:
    void sendGuess(int);

private:
    const QHostAddress adresse;
    const quint16 port;
    void sendMessage(char*,int);
    bool gererNewBoard(char*, uint32_t);
};

#endif // COMMCLASS_H
