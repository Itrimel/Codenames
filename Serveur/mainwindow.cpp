#include "mainwindow.h"
#include "ui_mainwindow.h"

#define NB_MOTS 398

extern char _binary_liste_mots_txt_start;
extern char _binary_liste_mots_txt_end;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    WidgetConnexion *label = new WidgetConnexion(parent,ui->menubar);
    ui->menubar->setCornerWidget(label);

    creerNouvellePartie();

    connect(ui->actionNouvelle_partie,&QAction::triggered,this,&MainWindow::nouvPartie);
    connect(ui->actionObtenir_adresse_IP,&QAction::triggered,this,&MainWindow::adresseIPLocale);
    connect(ui->actionObtenir_adresse_IP_globale,&QAction::triggered,this,&MainWindow::adresseIPGlobale);
}

void MainWindow::creerNouvellePartie(){
    //Génération de la liste de mots
    //Commande pour convertir le fichier .txt en .o : objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 liste_mots.txt liste_mots.o
    QByteArray liste_mots_data(&_binary_liste_mots_txt_start,_binary_liste_mots_txt_end-_binary_liste_mots_txt_start);
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
        liste_cartes->emplace_back(new QCard(lay[i],liste_mots[pos[i]],liste_cartes,ui->centralwidget));
        ui->gridLayout->addWidget(liste_cartes->back(),i%5,i/5);
    }
}

void MainWindow::supprimerPartieEnCours(){
    for(auto i = liste_cartes->begin(); i < liste_cartes->end(); i++){
        ui->gridLayout->removeWidget(*i);
        delete *i;
    }
}

void MainWindow::nouvPartie(){
    supprimerPartieEnCours();
    creerNouvellePartie();
}

void MainWindow::adresseIPLocale(){
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

void MainWindow::adresseIPGlobale(){
    QGlobaIPDiag infoIP("L'adresse IP externe du serveur est : ",ui->centralwidget);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete liste_cartes;
}

