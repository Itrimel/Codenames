#ifndef WIDGETCONNEXION_H
#define WIDGETCONNEXION_H

#include <QTcpServer>
#include <QTcpSocket>
#include "serveur_internet_infos.h"
#include "Commun/protocole.h"
#include "Commun/socketcommun.h"

enum etatCo{
    PasCo,
    Co,
    Dropping,
    EchecCo
};

class ServeurConnexions : public QObject
{

    Q_OBJECT

public:
    ServeurConnexions(QObject*,std::vector<data_carte>*);
    virtual ~ServeurConnexions();
public slots:
    void nouvellePartie();
signals:
    void nbAgents(int);
    void nbEspions(int);

private:
    auto findTypeJoueur(SocketCommun*, typeJoueur*);
    bool clign=true;
    QTcpServer* server;
    std::vector<SocketCommun*> connections_agents;
    std::vector<SocketCommun*> connections_espions;
    std::vector<data_carte>* liste_cartes;
private slots:
    void nouvCo();
    void gererErreur(SocketCommun*,QAbstractSocket::SocketError);
    void sendBoard(SocketCommun*);
    void gererGuess(SocketCommun*, char);
    void typeRecu(SocketCommun*,typeJoueur);
};

#endif // WIDGETCONNEXION_H
