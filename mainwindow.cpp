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
    for(int i=0; i<25; i++){
        ui->gridLayout->addWidget(new QCard(lay[i],liste_mots[pos[i]],ui->centralwidget),i%5,i/5);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

