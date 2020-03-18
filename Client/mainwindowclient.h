#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commclass.h"

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
    CommClass* communication;
    bool comm_exists = false;
private slots:
    void connexion();
    void connexionEtab();
    void changerBoard();
};
#endif // MAINWINDOW_H
