#include "serveurconnexions.h"


auto ServeurConnexions::findTypeJoueur(SocketCommun* socket, typeJoueur* joueur){
    auto iterator = std::find(connections_agents.begin(),connections_agents.end(),socket);
    auto iterator2 = std::find(connections_espions.begin(),connections_espions.end(),socket);
    if(iterator==connections_agents.end()){
        if(iterator2==connections_espions.end()){
            //ça ne devrait pas arriver
            //TODO : raise erreur ?
            qDebug() << "Pb : pas trouvé socket";
            *joueur = ErreurJoueur;
        } else {
            *joueur = Espion;
        }
        return iterator2;
    } else {
        *joueur = Agent;
        return iterator;
    }
}


ServeurConnexions::ServeurConnexions(QObject* parent):
    QObject(parent)
{
    creerNouvellePartie();

    //initialisation connexion
    server = new QTcpServer();
    connect(server,&QTcpServer::newConnection,this,&ServeurConnexions::nouvCo);
    if(!server->listen(QHostAddress::Any,PORT_SERVEUR)){
        //TODO : raise error
        qDebug() << "échec connexion : ne peut pas écouter les connexions entrantes";
    }
}

ServeurConnexions::~ServeurConnexions(){
    for(auto i = connections_agents.begin();i<connections_agents.end();i++){
        delete *i;
    }
    for(auto i = connections_espions.begin();i<connections_espions.end();i++){
        delete *i;
    }
    delete liste_cartes;
}

void ServeurConnexions::nouvCo(){
    qDebug() << "nouvelle co";
    SocketCommun* new_socket = new SocketCommun(server->nextPendingConnection());
    connect(new_socket,&SocketCommun::erreur,this,&ServeurConnexions::gererErreur);
    connect(new_socket,&SocketCommun::guessRecu,this,&ServeurConnexions::gererGuess);
    connect(new_socket,&SocketCommun::typeRecu,this,&ServeurConnexions::typeRecu);
    new_socket->sendAskType();
}

void ServeurConnexions::sendBoard(SocketCommun* sock){
    typeJoueur joueur = ErreurJoueur;
    findTypeJoueur(sock,&joueur);

    if(joueur==ErreurJoueur){
        qDebug() << "Joueur non enregistré ou inconnu";
        return;
    }

    std::vector<data_carte>* plateau = new std::vector<data_carte>(25);
    for(int i=0; i<25; i++){
        plateau->at(i) = liste_cartes->at(i);
        if(joueur == Agent && liste_cartes->at(i).guessed == false){
            plateau->at(i).type = SaisPas;
        }
    }
    sock->sendBoard(plateau);
    delete plateau;
}

void ServeurConnexions::gererGuess(SocketCommun* origine, char nb){
    typeJoueur joueur;
    findTypeJoueur(origine, &joueur);
    if(joueur != Agent){
        return;
    }
    liste_cartes->at(nb).guessed = true;
    for(auto i = connections_agents.begin(); i<connections_agents.end(); i++){
        (*i)->sendUpdate(nb,liste_cartes->at(nb).type);
    }
    for(auto i = connections_espions.begin(); i<connections_espions.end(); i++){
        (*i)->sendUpdate(nb,liste_cartes->at(nb).type);
    }
}

void ServeurConnexions::nouvellePartie(){
    creerNouvellePartie();
    for(auto i = connections_agents.begin(); i<connections_agents.end(); i++){
        (*i)->sendAskType();
    }
    for(auto i = connections_espions.begin(); i<connections_espions.end(); i++){
        (*i)->sendAskType();
    }
    connections_espions.clear();
    connections_agents.clear();
    emit nbAgents(0);
    emit nbEspions(0);
}

void ServeurConnexions::gererErreur(SocketCommun* origine, QAbstractSocket::SocketError erreur){
    qDebug() << erreur;
    //Gestion simple : on droppe sans réflechir
    typeJoueur joueur;
    auto iterator = findTypeJoueur(origine,&joueur);
    switch(joueur){
    case(Agent):
        connections_agents.erase(iterator);
        break;
    case(Espion):
        connections_espions.erase(iterator);
        break;
    case(ErreurJoueur):
        break;
    }
    origine->deleteLater();

    emit nbAgents(connections_agents.size());
    emit nbEspions(connections_espions.size());
}

void ServeurConnexions::typeRecu(SocketCommun* socket, typeJoueur joueur){
    typeJoueur tmp;
    auto it = findTypeJoueur(socket,&tmp);

    switch(tmp){
    case(Agent):
        connections_agents.erase(it);
        break;
    case(Espion):
        connections_espions.erase(it);
    default:
        break;
    }

    if(joueur==Agent){
        connections_agents.emplace_back(socket);
    } else if(joueur==Espion) {
        connections_espions.emplace_back(socket);
    }

    emit nbAgents(connections_agents.size());
    emit nbEspions(connections_espions.size());

    sendBoard(socket);
}

void ServeurConnexions::creerNouvellePartie(){

    bool found;
    std::vector<int> pos;
    int nb;
    QRandomGenerator *generator = QRandomGenerator::system();
    std::vector<QString> liste_mots;
    typeCarte quicestquicommence,quicestquicontinue;

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
}
