#include "socketcommun.h"

SocketCommun::SocketCommun(QHostAddress adresse, quint16 port, QObject* parent = nullptr):
    QObject(parent),
    adresse(adresse),
    port(port)
{
    socket = new QTcpSocket(parent);
}

SocketCommun::SocketCommun(QTcpSocket* sock):
    adresse(sock->peerAddress()),
    port(sock->peerPort())
{
    socket = sock;
    if(socket->state()!=QAbstractSocket::ConnectedState){
        qDebug() << "Constructeur non valable";
        //TODO :Faire un raise error ici
    } else {
        coEtablie();
    }
}

void SocketCommun::lancerCo(){
    socket->connectToHost(adresse.toString(),port);
    connect(socket,&QTcpSocket::connected,this,&SocketCommun::coEtablie);
}

void SocketCommun::coEtablie(){
    connect(socket,&QTcpSocket::readyRead,this,&SocketCommun::readMessage);
    connect(socket,qOverload<QAbstractSocket::SocketError>(&QTcpSocket::error),this,&SocketCommun::gererErreur);
    emit coPrete();
}

void SocketCommun::getNewBoard(){
    message_header header;
    header.type = MSG_TYPE_GET_BOARD;
    header.length = 1;
    char buffer[sizeof(header)+1];
    memcpy(buffer,reinterpret_cast<char*>(&header),sizeof(header));
    buffer[sizeof(header)] = 0; //Message non vide, histoire d'écrire qqchose
    socket->write(buffer,sizeof(header)+1);
}

void SocketCommun::readMessage(){
    message_header header;
    while(socket->bytesAvailable()){
        socket->read(reinterpret_cast<char*>(&header), sizeof(message_header));
        char buffer[header.length];
        socket->read(buffer,header.length);
        switch(header.type){
        case(MSG_TYPE_NOP):
            break;
        case(MSG_TYPE_GUESS):
            if(header.length!=1){
                qDebug() << "Pas bonne longueur : guess";
            }
            emit guessRecu(this,buffer[0]);
            break;
        case(MSG_TYPE_BOARD):
            if(gererNewBoard(buffer, header.length)){
                emit newBoard();
            } else {
                //Erreur
                qDebug() << "Message plateau pas assez long !";
            }
            break;
        case(MSG_TYPE_UPDATE):
            if(header.length!=2){
                qDebug() << "Mauvaise taille de message : update";
            }
            emit carteUpdate(buffer[0],(typeCarte)buffer[1]);
            break;
        case(MSG_TYPE_GET_BOARD):
            emit askForBoard(this);
            break;
        default:
            //Implement pas normal
            qDebug() << "Message pas bon";
            break;
        }
    }
}

bool SocketCommun::gererNewBoard(char* message, uint32_t length){
    data_carte carte_courante;
    int pos=0, curr_len=0,curr_mot=0;
    int mots[25];
    while(true){
        if(message[pos]!=0){
            curr_len++;
            if(++pos>= length){
                return false;
            }
        } else {
            mots[curr_mot]=curr_len;
            curr_len=0;
            pos+=2;
            if(pos>length){
                return false;
            }
            if(++curr_mot==25){
                break;
            }
        }
    }
    pos=0;
    for(curr_mot=0;curr_mot<25;curr_mot++){
        carte_courante.carte = QString(message+pos);
        pos+=mots[curr_mot]+1;
        carte_courante.type = (typeCarte)(message[pos]);
        pos+=1;
        plateau_courant[curr_mot]=carte_courante;
    }
    return true;
}

void SocketCommun::sendBoard(std::vector<data_carte>* liste_cartes){
    char buffer[1000]; //Ca devrait aller, il y a pas de mots de plus de 35 caractères en français
    int position=0,len;
    std::string mot;
    message_header header;
    header.type=MSG_TYPE_BOARD;
    for(int i=0; i<25; i++){
        mot = liste_cartes->at(i).carte.toStdString();
        len = mot.length();
        memcpy(buffer+position+sizeof(header),mot.data(),len+1);
        position+=len+1;
        buffer[position-1+sizeof(header)]=0;
        buffer[position+sizeof(header)]=(char)liste_cartes->at(i).type;
        position+=1;
    }
    header.length=position;
    memcpy(buffer,&header,sizeof(header));
    socket->write(buffer,position+sizeof(header));
}

void SocketCommun::sendUpdate(char nb,typeCarte type){
    message_header header;
    char buffer[sizeof(header)+2];
    header.type = MSG_TYPE_UPDATE;
    header.length = 2;
    memcpy(buffer,&header,sizeof(header));
    buffer[sizeof(header)]=nb;
    buffer[sizeof(header)+1]=type;
    socket->write(buffer,sizeof(header)+2);
}

void SocketCommun::sendGuess(int nb){
    message_header header;
    header.type = MSG_TYPE_GUESS;
    header.length = 1;
    char buffer[sizeof(header)+1];
    memcpy(buffer,&header,sizeof(header));
    buffer[sizeof(header)]=nb;
    socket->write(buffer,sizeof(header)+1);
}

void SocketCommun::gererErreur(QAbstractSocket::SocketError err){
    emit erreur(this,err);
}
