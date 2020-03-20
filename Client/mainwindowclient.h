#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "Commun/socketcommun.h"
#include "Commun/qcard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client1; class Client2; }
QT_END_NAMESPACE

class MainWindowClient : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowClient(QWidget *parent = nullptr);
    ~MainWindowClient();
private:
    Ui::Client1 *ui1;
    Ui::Client2 *ui2;
    SocketCommun* communication;
    bool comm_exists = false;
    bool ui1_exists = true;
    bool premier_plateau=true;
    std::vector<QCard*>* liste_cartes = new std::vector<QCard*>;
    QHostAddress addr;
    quint16 prt;
    QLabel* label_co = new QLabel();
    SocketCommun* reco_socket;
    int num_reco=0;
private slots:
    void connexion();
    void reconnexion();
    void connexionEtab();
    void changerBoard();
    void guessCarte(char,typeCarte);
    void erreur(SocketCommun*,QAbstractSocket::SocketError);
};
#endif // MAINWINDOW_H
