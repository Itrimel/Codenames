#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include <QRandomGenerator>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkInterface>
#include "Commun/qcard.h"
#include "qglobaipdiag.h"
#include "widgetconnexion.h"
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

private:
    Ui::Serveur *ui;
    typeCarte quicestquicommence;
    typeCarte quicestquicontinue;
    void creerNouvellePartie();
    void supprimerPartieEnCours();
    std::vector<QCard*>* liste_cartes = new std::vector<QCard*>;
    QLabel* label_qui_commence;
private slots:
    void nouvPartie();
    void adresseIPLocale();
    void adresseIPGlobale();
};
#endif // MAINWINDOW_H
