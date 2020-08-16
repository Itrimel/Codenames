#ifndef QGLOBAIPDIAG_H
#define QGLOBAIPDIAG_H

#include <QLabel>

#include "serveur_internet_infos.h"

class QNetworkReply;
class QNetworkAccessManager;
class QTimer;

class ExtIPGet : public QObject
{
    Q_OBJECT

public:
    ExtIPGet(QObject*, QLabel*);
    ~ExtIPGet();
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
