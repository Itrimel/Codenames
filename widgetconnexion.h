#ifndef WIDGETCONNEXION_H
#define WIDGETCONNEXION_H

#include <QLabel>
#include <QTimer>
#include <QMenuBar>
#include <QTcpServer>
#include <QTcpSocket>

#define PORT_SERVEUR 8081

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
