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
    QTimer* clignotement_pas_co;
    bool clign=true;
    QMenuBar* menuBar;
    QTcpServer* server;
    QTcpSocket* socket;
    std::vector<QCard*>* liste_cartes;
    void sendBoard(QTcpSocket*);
private slots:
    void textePasCo();
    void nouvCo();
    void receptionMessage();
    void gererErreur(QAbstractSocket::SocketError);
};

#endif // WIDGETCONNEXION_H
