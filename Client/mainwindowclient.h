#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "dialogtypejoueur.h"
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
    DialogTypeJoueur* dialogueType = new DialogTypeJoueur(this,&communication);
    QLabel* label_qui_commence = new QLabel(this);
    bool comm_exists = false;
    bool ui1_exists = true;
    std::vector<QCard*>* liste_cartes = new std::vector<QCard*>;
    typeJoueur joueur;
    QHostAddress address;
    quint16 port;
private slots:
    void connexion();
    void demandeType();
    void finDemandeType(int);
    void changerBoard();
    void guessCarte(char,typeCarte);
    void erreur(SocketCommun*,QAbstractSocket::SocketError);
    void erreurCo(SocketCommun*,QAbstractSocket::SocketError);
};
#endif // MAINWINDOW_H
