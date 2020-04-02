#include "extipget.h"

ExtIPGet::ExtIPGet(QObject* parent,QLabel* label):
    QObject(parent),
    label(label)
{
    update_text();
    update_add = new QTimer(this);
    connect(update_add,&QTimer::timeout,this,&ExtIPGet::update_text);
    update_add->start(800);

    //Largement copié de https://stackoverflow.com/questions/47844479/qt-get-external-ip-address-using-qnetworkreply
    manager = new QNetworkAccessManager(this);
    QUrl url("https://api.ipify.org");
    QUrlQuery query;
    query.addQueryItem("format", "json");
    url.setQuery(query);

    reply = manager->get(QNetworkRequest(url));

    QObject::connect(reply, &QNetworkReply::finished,
                         [&](){
            gotReply=true;
            update_add->stop();
            if(reply->error() != QNetworkReply::NoError) {
                //failure
                emit setExtIP(QString("erreur de connexion : ")+reply->error());
                qDebug() << "error : " << reply->error();
            } else { //success
                //parse the json reply to extract the IP address
                emit setExtIP(QJsonDocument::fromJson(reply->readAll()).object().value("ip").toString()+QString(":%1").arg(PORT_SERVEUR));
            }
            //delete reply later to prevent memory leak
            reply->deleteLater();
            this->deleteLater();
        });
}

void ExtIPGet::update_text(){
    QString temp="";
    for(int i=0;i<nb_points+1;i++){
        temp+=".";
    }
    label->setText(temp);
    nb_points=(nb_points+1)%4;
}

