#ifndef DIALOGIP_H
#define DIALOGIP_H

#include <QDialog>
#include <QCloseEvent>
#include "Commun/socketcommun.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client1;}
QT_END_NAMESPACE

class DialogIP : public QDialog
{
    Q_OBJECT
public:
    DialogIP(QWidget*, QHostAddress*, quint16*,SocketCommun**);
    ~DialogIP();
private:
    Ui::Client1* ui;
    QHostAddress* address;
    quint16* port;
    SocketCommun** socket_in_main_window;
    void closeEvent(QCloseEvent *) override;
private slots:
    void connexion();
    void erreur(SocketCommun*,QAbstractSocket::SocketError);
    void finDiag();

};

#endif // DIALOGIP_H
