#ifndef SOCKETCOMMUN_H
#define SOCKETCOMMUN_H

#include <QHostAddress>
#include "Commun/protocole.h"

struct data_carte{
    QString carte;
    typeCarte type;
    bool guessed = false;
};

class QTcpSocket;

class SocketCommun : public QObject
{
    Q_OBJECT
public:
    SocketCommun(QHostAddress,quint16,QObject*);
    SocketCommun(QTcpSocket*);
    void lancerCo();
    void getNewBoard();
    QVector<data_carte> plateau_courant=QVector<data_carte>(25);
    bool bleuCommence;
    void readMessage();
    void sendBoard(std::vector<data_carte>*);
    void sendUpdate(char,typeCarte);
    void sendPing();
    void sendJoueurType(typeJoueur);
    QHostAddress inline getAddr() {return adresse;}
    quint16 inline getPrt() {return port;}
    QAbstractSocket::SocketState stateSocket() ;
signals:
    void newBoard();
    void carteUpdate(char,typeCarte);
    void guessRecu(SocketCommun*,char);
    void askForBoard(SocketCommun*);
    void demandeType();
    void typeRecu(SocketCommun*, typeJoueur);
    void erreur(SocketCommun*,QAbstractSocket::SocketError);
    void erreurCo(SocketCommun*,QAbstractSocket::SocketError);
public slots:
    void sendGuess(int);
    void sendAskType();
private:
    QTcpSocket* socket;
    const QHostAddress adresse;
    const quint16 port;
    bool gererNewBoard(char*, uint32_t);
private slots:
    void coEtablie();
    void gererErreur(QAbstractSocket::SocketError);
    void gererErreurCo(QAbstractSocket::SocketError);
};

#endif // SOCKETCOMMUN_H
