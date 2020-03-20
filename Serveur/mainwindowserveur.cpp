#include "mainwindowserveur.h"
#include "ui_serveur.h"

#define NB_MOTS 398

extern char _binary____Codenames_Commun_liste_mots_txt_start;
extern char _binary____Codenames_Commun_liste_mots_txt_end;

MainWindowServeur::MainWindowServeur(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Serveur)
{
    ui->setupUi(this);

    label_qui_commence = new QLabel(parent);
    ui->gridLayout->addWidget(label_qui_commence,0,0,1,5,Qt::AlignCenter);
    ui->gridLayout->setRowStretch(0,1);
    for(int i=1;i<6;ui->gridLayout->setRowStretch(i++,4));

    creerNouvellePartie();

    WidgetConnexion *label = new WidgetConnexion(parent,ui->menubar,liste_cartes);
    ui->menubar->setCornerWidget(label);

    connect(ui->actionNouvelle_partie,&QAction::triggered,this,&MainWindowServeur::nouvPartie);
    connect(ui->actionObtenir_adresse_IP,&QAction::triggered,this,&MainWindowServeur::adresseIPLocale);
    connect(ui->actionObtenir_adresse_IP_globale,&QAction::triggered,this,&MainWindowServeur::adresseIPGlobale);
}

void MainWindowServeur::creerNouvellePartie(){
    //Génération de la liste de mots
    //Commande pour convertir le fichier .txt en .o : objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 liste_mots.txt liste_mots.o
    QByteArray liste_mots_data(&_binary____Codenames_Commun_liste_mots_txt_start,_binary____Codenames_Commun_liste_mots_txt_end-_binary____Codenames_Commun_liste_mots_txt_start);
    QTextStream liste_mots_stream(liste_mots_data);
    std::vector<QString> liste_mots;
    for(int i=0; i<NB_MOTS; i++){
        liste_mots.emplace_back(liste_mots_stream.readLine());
        liste_mots.back().front()=liste_mots.back().front().toUpper();
    }

    bool found;
    std::vector<int> pos;
    int nb;
    QRandomGenerator *generator = QRandomGenerator::system();

    //Qui c'est qui commence
    quicestquicommence = generator->bounded(2) ? Rouge : Bleu ;
    quicestquicontinue = quicestquicommence==Rouge ? Bleu : Rouge;
    if(quicestquicommence==Rouge){
        label_qui_commence->setText("<font color=red>Les rouges commencent</font>");
    } else {
        label_qui_commence->setText("<font color=blue>Les bleus commencent</font>");
    }

    //Choix des mots
    for(int i = 0; i<25; i++){
        label1:nb = generator->bounded(NB_MOTS);
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

    //Construction de l'affichage
    liste_cartes->clear();
    for(int i=0; i<25; i++){
        liste_cartes->emplace_back(new QCard(i,lay[i],liste_mots[pos[i]],liste_cartes,ui->centralwidget));
        ui->gridLayout->addWidget(liste_cartes->back(),(i%5)+1,i/5);
    }
}

void MainWindowServeur::supprimerPartieEnCours(){
    for(auto i = liste_cartes->begin(); i < liste_cartes->end(); i++){
        ui->gridLayout->removeWidget(*i);
        delete *i;
    }
}

void MainWindowServeur::nouvPartie(){
    supprimerPartieEnCours();
    creerNouvellePartie();
    reinterpret_cast<WidgetConnexion*>(ui->menubar->cornerWidget())->resendBoard();
}

void MainWindowServeur::adresseIPLocale(){
    QMessageBox infoIP(ui->centralwidget);
    auto list_adresses = QNetworkInterface::allAddresses();
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    QString adresse_loc;

    for(int nIter=0; nIter<list.count(); nIter++){
        if(!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
                adresse_loc = list[nIter].toString();
    }

    infoIP.setText("L'adresse IP locale du serveur est : "+adresse_loc+QString(":%1").arg(PORT_SERVEUR));
    infoIP.exec();
}

void MainWindowServeur::adresseIPGlobale(){
    QGlobaIPDiag infoIP("L'adresse IP externe du serveur est : ",ui->centralwidget);

}

MainWindowServeur::~MainWindowServeur()
{
    delete ui;
    delete liste_cartes;
    delete label_qui_commence;
}

