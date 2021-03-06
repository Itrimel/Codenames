#include "dialogtypejoueur.h"
#include "ui_dialogtypejoueur.h"
#include <QCloseEvent>

DialogTypeJoueur::DialogTypeJoueur(QWidget* parent, SocketCommun** socket):
    QDialog(parent,Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    socket(socket),
    ui(new Ui::Dialog)
{
    setModal(true);
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    ui->pushButton->setDefault(true);
    connect(ui->radioButton_agent,&QRadioButton::toggled,this,&DialogTypeJoueur::radioClicked);
    connect(ui->radioButton_espion,&QRadioButton::toggled,this,&DialogTypeJoueur::radioClicked);
    connect(ui->pushButton,&QPushButton::clicked,this,&DialogTypeJoueur::choixFait);
}

void DialogTypeJoueur::radioClicked(){
    ui->pushButton->setEnabled(true);
}

void DialogTypeJoueur::choixFait(){
    ui->pushButton->setEnabled(false);
    ui->radioButton_espion->setEnabled(false);
    ui->radioButton_agent->setEnabled(false);
    if(ui->radioButton_espion->isChecked()){
        joueur = Espion;
    }
    else {
        joueur = Agent;
    }
    ui->label->setText("Récupération du plateau en cours");
    connect(*socket,&SocketCommun::newBoard,this,&DialogTypeJoueur::finDiag);
    (*socket)->sendJoueurType(joueur);
}

void DialogTypeJoueur::finDiag(){
    disconnect(*socket,&SocketCommun::newBoard,this,&DialogTypeJoueur::finDiag);
    ui->radioButton_espion->setEnabled(true);
    ui->radioButton_agent->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->label->setText("Choix du camp");
    done(joueur);
}

void DialogTypeJoueur::closeEvent(QCloseEvent* event){
    event->ignore();
}

void DialogTypeJoueur::closeFromParent(){
    ui->radioButton_espion->setEnabled(true);
    ui->radioButton_agent->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->label->setText("Choix du camp");
    hide();
}

DialogTypeJoueur::~DialogTypeJoueur(){
    delete ui;
}
