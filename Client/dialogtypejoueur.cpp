#include "dialogtypejoueur.h"
#include "ui_dialogtypejoueur.h"

DialogTypeJoueur::DialogTypeJoueur(QWidget* parent, SocketCommun* socket):
    QDialog(parent,Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    socket(socket),
    ui(new Ui::Dialog)
{
    setModal(true);
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    connect(ui->radioButton_2,&QRadioButton::toggled,this,&DialogTypeJoueur::radioClicked);
    connect(ui->radioButton,&QRadioButton::toggled,this,&DialogTypeJoueur::radioClicked);
    connect(ui->pushButton,&QPushButton::clicked,this,&DialogTypeJoueur::choixFait);
}

void DialogTypeJoueur::radioClicked(){
    ui->pushButton->setEnabled(true);
}

void DialogTypeJoueur::choixFait(){
    ui->pushButton->setEnabled(false);
    ui->radioButton->setEnabled(false);
    ui->radioButton_2->setEnabled(false);
    if(ui->radioButton_2->isChecked()){
        joueur = Espion;
    }
    else {
        joueur = Agent;
    }
    ui->label->setText("Récupération du plateau en cours");
    connect(socket,&SocketCommun::newBoard,this,&DialogTypeJoueur::finDiag);
    socket->sendJoueurType(joueur);
}

void DialogTypeJoueur::finDiag(){
    done(joueur);
}

DialogTypeJoueur::~DialogTypeJoueur(){
    delete ui;
}
