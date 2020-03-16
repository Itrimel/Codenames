#ifndef QGLOBAIPDIAG_H
#define QGLOBAIPDIAG_H

#define PORT_SERVEUR 8081

#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
#include <iostream>



class QGlobaIPDiag : public QMessageBox
{
public:
    QGlobaIPDiag(QString, QWidget*);
    ~QGlobaIPDiag(){
        if(!gotReply){
            update_add->stop();
        }
        manager->deleteLater();
        delete update_add;
    }

private:
    const QString texte;
    int nb_points=0;
    QTimer* update_add;
    QNetworkAccessManager* manager;
    bool gotReply=false;
private slots:
    void update_text();
    void manageReply(QNetworkReply*);
};

#endif // QGLOBAIPDIAG_H
