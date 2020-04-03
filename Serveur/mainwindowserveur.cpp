#include "mainwindowserveur.h"
#include "ui_serveur.h"

MainWindowServeur::MainWindowServeur(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Serveur)
{
    serveur = new ServeurConnexions(parent);

    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,serveur,&ServeurConnexions::nouvellePartie);

    setIPLocal(ui->label_ip_loc);
    ip_ext_getter = new ExtIPGet(parent,ui->label_ip_ext);
    connect(ip_ext_getter,&ExtIPGet::setExtIP,this,&MainWindowServeur::setExtIP);
    nbAgents(0);
    nbEspions(0);

    connect(serveur,&ServeurConnexions::nbAgents,this,&MainWindowServeur::nbAgents);
    connect(serveur,&ServeurConnexions::nbEspions,this,&MainWindowServeur::nbEspions);
}

static void setIPLocal(QLabel* label){
    auto list_adresses = QNetworkInterface::allAddresses();
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    QString adresse_loc;

    for(int nIter=0; nIter<list.count(); nIter++){
        if(!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
                adresse_loc = list[nIter].toString();
    }

    label->setText(adresse_loc+QString(":%1").arg(PORT_SERVEUR));
}

void MainWindowServeur::nbAgents(int nb){
    if(nb==0){
        ui->label_agents->setStyleSheet("font-weight: bold; color: red");
        ui->label_agents->setText("0");
    } else {
        ui->label_agents->setStyleSheet("font-weight: bold; color: green");
        ui->label_agents->setText(QString(" %1").arg(nb));
    }
}

void MainWindowServeur::nbEspions(int nb){
    if(nb==0){
        ui->label_espions->setStyleSheet("font-weight: bold; color: red");
        ui->label_espions->setText("0");
    } else {
        ui->label_espions->setStyleSheet("font-weight: bold; color: green");
        ui->label_espions->setText(QString(" %1").arg(nb));
    }
}

void MainWindowServeur::setExtIP(QString texte){
    ui->label_ip_ext->setText(texte);
}


MainWindowServeur::~MainWindowServeur()
{
    delete ui;
}

