#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serveurconnexions.h"
#include "extipget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Serveur; }
QT_END_NAMESPACE

class MainWindowServeur : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowServeur(QWidget *parent = nullptr);
    ~MainWindowServeur();
private:
    Ui::Serveur *ui;
    ServeurConnexions* serveur;
    ExtIPGet* ip_ext_getter;
private slots:
    void nbAgents(int);
    void nbEspions(int);
    void setExtIP(QString);
};

#endif // MAINWINDOW_H
