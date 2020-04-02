#ifndef QGLOBAIPDIAG_H
#define QGLOBAIPDIAG_H

#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
#include "serveur_internet_infos.h"

class ExtIPGet : public QObject
{
    Q_OBJECT

public:
    ExtIPGet(QObject*, QLabel*);
    ~ExtIPGet(){
        if(!gotReply){
            update_add->stop();
        }
        manager->deleteLater();
        delete update_add;
    }
signals:
    void setExtIP(QString);

private:
    int nb_points=0;
    QTimer* update_add;
    QNetworkReply* reply;
    QNetworkAccessManager* manager;
    bool gotReply=false;
    QLabel* label;
private slots:
    void update_text();
};

#endif // QGLOBAIPDIAG_H
