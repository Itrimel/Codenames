#include "socketcommun.h"

SocketCommun::SocketCommun(QHostAddress adresse, quint16 port, QObject* parent = nullptr):
    QTcpSocket(parent),
    adresse(adresse),
    port(port)
{}

void SocketCommun::lancerCo(){
    connectToHost(adresse.toString(),port);
    connect(this,&SocketCommun::connected,this,&SocketCommun::coEtablie);
}

void SocketCommun::coEtablie(){
    connect(this,&SocketCommun::readyRead,this,&SocketCommun::readMessage,Qt::UniqueConnection);
}

void SocketCommun::getNewBoard(){
    message_header header;
    header.type = MSG_TYPE_GET_BOARD;
    header.length = 1;
    char buffer[sizeof(header)+1];
    memcpy(buffer,reinterpret_cast<char*>(&header),sizeof(header));
    buffer[sizeof(header)] = 0; //Message non vide, histoire d'écrire qqchose
    write(buffer,sizeof(header)+1);
}

void SocketCommun::readMessage(){
    message_header header;
    while(bytesAvailable()){
        read(reinterpret_cast<char*>(&header), sizeof(message_header));
        char buffer[header.length];
        read(buffer,header.length);
        switch(header.type){
        case(MSG_TYPE_NOP):
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
        case(MSG_TYPE_PING):
            //Implement
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

void SocketCommun::sendGuess(int nb){
    message_header header;
    header.type = MSG_TYPE_GUESS;
    header.length = 1;
    char buffer[sizeof(header)+1];
    memcpy(buffer,&header,sizeof(header));
    buffer[sizeof(header)]=nb;
    write(buffer,sizeof(header)+1);
}
