#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include <QRandomGenerator>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkInterface>
#include "qcard.h"
#include "qglobaipdiag.h"
#include "widgetconnexion.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    typeCarte quicestquicommence;
    typeCarte quicestquicontinue;
    void creerNouvellePartie();
    void supprimerPartieEnCours();
    std::vector<QCard*>* liste_cartes = new std::vector<QCard*>;
private slots:
    void nouvPartie();
    void adresseIPLocale();
    void adresseIPGlobale();
};
#endif // MAINWINDOW_H
