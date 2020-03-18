#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Client1; }
QT_END_NAMESPACE

class MainWindowClient : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowClient(QWidget *parent = nullptr);
    ~MainWindowClient();

private:
    Ui::Client1 *ui;
private slots:
    void connexion();
    void connexionEtab();
};
#endif // MAINWINDOW_H
