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
public:
    WidgetConnexion(QWidget*, QMenuBar*);
    virtual ~WidgetConnexion();
private:
    etatCo etat=PasCo;
    void goPasCo();
    QTimer* clignotement_pas_co;
    bool clign=true;
    QMenuBar* menuBar;
    QTcpServer* server;
    QTcpSocket* socket;
    void sendBoard(QTcpSocket*);
    std::vector<QCard*>* liste_cartes;
private slots:
    void textePasCo();
    void nouvCo();
    void receptionMessage();
};

#endif // WIDGETCONNEXION_H
