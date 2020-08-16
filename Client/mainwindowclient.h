#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialogtypejoueur.h"
#include "dialogip.h"
#include "Commun/socketcommun.h"
#include "Commun/qcard.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Client2; }
QT_END_NAMESPACE

class QLabel;

class MainWindowClient : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowClient(QWidget *parent = nullptr);
    ~MainWindowClient();
private:
    Ui::Client2* ui;
    SocketCommun* communication;
    DialogTypeJoueur* dialogueType;
    DialogIP* dialogueIP;
    QLabel* label_qui_commence;
    bool comm_exists = false;
    bool ui1_exists = true;
    std::vector<QCard*>* liste_cartes;
    typeJoueur joueur;
    QHostAddress address;
    quint16 port;
private slots:
    void finDiagIP(int);
    void demandeType();
    void finDemandeType(int);
    void changerBoard();
    void guessCarte(char,typeCarte);
    void erreur(SocketCommun*,QAbstractSocket::SocketError);
};
#endif // MAINWINDOW_H
