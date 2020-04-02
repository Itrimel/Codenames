#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkInterface>
#include "serveurconnexions.h"
#include "extipget.h"
#include "Commun/liste_mots.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Serveur; }
QT_END_NAMESPACE

class MainWindowServeur : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowServeur(QWidget *parent = nullptr);
    ~MainWindowServeur();
signals:
    void nouvPartie();

private:
    Ui::Serveur *ui;
    ServeurConnexions* serveur;
    typeCarte quicestquicommence;
    typeCarte quicestquicontinue;
    std::vector<data_carte>* liste_cartes = new std::vector<data_carte>(25);
    ExtIPGet* ip_ext_getter;
private slots:
    void creerNouvellePartie();
    void nbAgents(int);
    void nbEspions(int);
    void setExtIP(QString);
};

static void setIPLocal(QLabel*);
#endif // MAINWINDOW_H
