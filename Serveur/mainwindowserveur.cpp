#include "mainwindowserveur.h"
#include "ui_serveur.h"

MainWindowServeur::MainWindowServeur(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Serveur)
{
    creerNouvellePartie();

    serveur = new ServeurConnexions(parent,liste_cartes);

    ui->setupUi(this);

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

void MainWindowServeur::creerNouvellePartie(){

    bool found;
    std::vector<int> pos;
    int nb;
    QRandomGenerator *generator = QRandomGenerator::system();
    std::vector<QString> liste_mots;

    //Génération de la liste de mots
    for(auto i = liste_mots_brut.begin(); i< liste_mots_brut.end(); i++){
        liste_mots.emplace_back(QString::fromStdString(*i));
        liste_mots.back().front()=liste_mots.back().front().toUpper();
    }

    //Qui c'est qui commence
    quicestquicommence = generator->bounded(2) ? Rouge : Bleu ;
    quicestquicontinue = quicestquicommence==Rouge ? Bleu : Rouge;

    //Choix des mots
    for(int i = 0; i<25; i++){
        label1:nb = generator->bounded((int)liste_mots.size());
        found= (std::find(pos.begin(), pos.end(), nb) != pos.end())  ;
        if(found){goto label1;} //Je fais ce que je veux merde !
        pos.emplace_back(nb);
    }

    //Choix du layout
    std::vector<typeCarte> lay;
    lay.assign(25,Neutre);
    lay.at(generator->bounded(25)) = Noir;
    for(int i=0; i<17; i++){
        label2: nb = generator->bounded(25);
        if(lay[nb]!=Neutre){goto label2;} //Je fais ce que je veux merde ! le retour
        lay[nb] = (i%2) ? quicestquicontinue : quicestquicommence;
    }

    //Construction de la liste
    for(int i=0; i<25; i++){
        liste_cartes->at(i).carte = liste_mots[pos[i]];
        liste_cartes->at(i).type = lay[i];
    }
    emit nouvPartie();
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
    delete liste_cartes;
}

