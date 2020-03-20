#ifndef SOCKETCOMMUN_H
#define SOCKETCOMMUN_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QVector>
#include "Commun/protocole.h"

struct data_carte{
    QString carte;
    typeCarte type;
};

class SocketCommun : public QTcpSocket
{
    Q_OBJECT
public:
    SocketCommun(QHostAddress,quint16,QObject*);
    void lancerCo();
    void getNewBoard();
    QVector<data_carte> plateau_courant=QVector<data_carte>(25);
    void readMessage();
signals:
    void newBoard();
    void carteUpdate(char,typeCarte);
public slots:
    void sendGuess(int);
private:
    const QHostAddress adresse;
    const quint16 port;
    bool gererNewBoard(char*, uint32_t);
private slots:
    void coEtablie();
};

#endif // SOCKETCOMMUN_H
