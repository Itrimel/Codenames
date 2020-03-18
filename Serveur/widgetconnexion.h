#ifndef WIDGETCONNEXION_H
#define WIDGETCONNEXION_H

#include <QLabel>
#include <QTimer>
#include <QMenuBar>
#include <QTcpServer>
#include <QTcpSocket>
#include "serveur_internet_infos.h"

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
private slots:
    void textePasCo();
    void nouvCo();
};

#endif // WIDGETCONNEXION_H
