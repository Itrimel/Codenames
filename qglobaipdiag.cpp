#include "qglobaipdiag.h"

QGlobaIPDiag::QGlobaIPDiag(QString texte, QWidget* parent= nullptr):
    QMessageBox(parent),
    texte(texte)
{
    update_text();
    update_add = new QTimer(this);
    connect(update_add,&QTimer::timeout,this,&QGlobaIPDiag::update_text);
    update_add->start(800);

    //Largement copié de https://stackoverflow.com/questions/47844479/qt-get-external-ip-address-using-qnetworkreply
    manager = new QNetworkAccessManager(this);
    QUrl url("https://api.ipify.org");
    QUrlQuery query;
    query.addQueryItem("format", "json");
    url.setQuery(query);

    QNetworkReply* reply = manager->get(QNetworkRequest(url));

    QObject::connect(reply, &QNetworkReply::finished,
                         [&](){
            gotReply=true;
            update_add->stop();
            if(reply->error() != QNetworkReply::NoError) {
                //failure
                setText("erreur de connexion ! ");
                qDebug() << "error : " << reply->error();
            } else { //success
                //parse the json reply to extract the IP address
                setText(texte+QJsonDocument::fromJson(reply->readAll()).object().value("ip").toString()+QString(":%1").arg(PORT_SERVEUR));
            }
            //delete reply later to prevent memory leak
            reply->deleteLater();
        });

    this->exec();
}

void QGlobaIPDiag::update_text(){
    QString temp=texte;
    for(int i=0;i<nb_points+1;i++){
        temp+=".";
    }
    setText(temp);
    nb_points=(nb_points+1)%4;
}

void QGlobaIPDiag::manageReply(QNetworkReply* networkReply){
    std::cout << "hey" << std::endl;
    gotReply=true;
    update_add->stop();
    setText(texte+QJsonDocument::fromJson(networkReply->readAll()).object().value("ip").toString()+QString(":%1").arg(PORT_SERVEUR));
    networkReply->deleteLater();
}

