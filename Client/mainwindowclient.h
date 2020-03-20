#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    bool premier_plateau=true;
    std::vector<QCard*>* liste_cartes = new std::vector<QCard*>;
private slots:
    void connexion();
    void connexionEtab();
    void changerBoard();
    void guessCarte(char,typeCarte);
};
#endif // MAINWINDOW_H
