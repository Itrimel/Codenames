#ifndef WIDGETCONNEXION_H
#define WIDGETCONNEXION_H

#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QMenuBar>

enum etatCo{
    PasCo,
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
private slots:
    void textePasCo();
};

#endif // WIDGETCONNEXION_H
