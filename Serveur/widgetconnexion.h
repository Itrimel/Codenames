#ifndef WIDGETCONNEXION_H
#define WIDGETCONNEXION_H

#include <QLabel>
#include <QTimer>
#include <QMenuBar>
#include <QTcpServer>
#include <QTcpSocket>
#include "serveur_internet_infos.h"
#include "Commun/protocole.h"
#include "Commun/qcard.h"
#include "Commun/socketcommun.h"

enum etatCo{
    PasCo,
    Co,
    Dropping,
    EchecCo
};

class WidgetConnexion : public QLabel
{

    Q_OBJECT

public:
    WidgetConnexion(QWidget*, QMenuBar*, std::vector<QCard*>*);
    virtual ~WidgetConnexion();
    void resendBoard();
private:
    etatCo etat=PasCo;
    void goPasCo();
    auto findTypeJoueur(SocketCommun*, typeJoueur*);
    QTimer* clignotement_pas_co;
    bool clign=true;
    QMenuBar* menuBar;
    QTcpServer* server;
    std::vector<SocketCommun*> connections_agents;
    std::vector<SocketCommun*> connections_espions;
    std::vector<QCard*>* liste_cartes;
private slots:
    void textePasCo();
    void nouvCo();
    void gererErreur(SocketCommun*,QAbstractSocket::SocketError);
    void sendBoard(SocketCommun*);
    void gererGuess(SocketCommun*, char);
    void nouvJoueur(SocketCommun*,typeJoueur);
};

#endif // WIDGETCONNEXION_H
