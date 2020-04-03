#include "dialogip.h"
#include "ui_client1.h"


DialogIP::DialogIP(QWidget* parent, QHostAddress* address,quint16* port,SocketCommun** socket):
    QDialog(parent,Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    ui(new Ui::Client1),
    address(address),
    port(port),
    socket_in_main_window(socket)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&DialogIP::connexion);
}

void DialogIP::connexion(){
    ui->pushButton->setEnabled(false);

    *address = QHostAddress(ui->ip1->text()+"."+ui->ip2->text()+'.'+ui->ip3->text()+"."+ui->ip4->text());
    *port = ui->port_l->text().toInt();

    if(QAbstractSocket::IPv4Protocol != address->protocol()){
        ui->label_status->setText("l'adresse IP n'est pas valide");
        ui->pushButton->setEnabled(true);
        return;
    }
    if(ui->port_l->text().toInt()>65535){
        ui->label_status->setText("Port non valide");
        ui->pushButton->setEnabled(true);
        return;
    }

    *socket_in_main_window = new SocketCommun(*address,*port,parent());
    connect(*socket_in_main_window,&SocketCommun::demandeType,this,&DialogIP::finDiag);
    connect(*socket_in_main_window,&SocketCommun::erreurCo,this,&DialogIP::erreur);
    (*socket_in_main_window)->lancerCo();
    ui->label_status->setText("Connexion en cours");
}

void DialogIP::erreur(SocketCommun*,QAbstractSocket::SocketError err){
    (*socket_in_main_window)->deleteLater();
    ui->label_status->setText("Erreur "+QVariant::fromValue(err).toString());
    ui->pushButton->setEnabled(true);
}

void DialogIP::finDiag(){
    ui->pushButton->setEnabled(true);
    ui->label_status->setText("Attente de l'entrée de l'adresse");
    disconnect(*socket_in_main_window,&SocketCommun::demandeType,this,&DialogIP::finDiag);
    disconnect(*socket_in_main_window,&SocketCommun::erreurCo,this,&DialogIP::erreur);
    accept();
}

void DialogIP::closeEvent(QCloseEvent* event){
    event->ignore();
}

DialogIP::~DialogIP(){
    delete ui;
}
