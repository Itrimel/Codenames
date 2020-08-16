#ifndef PROTOCOLE_H
#define PROTOCOLE_H

#include <stdint.h>

enum typeCarte : char {
    Rouge   = 1,
    Bleu    = 2,
    Noir    = 3,
    Neutre  = 4,
    SaisPas = 5
};

enum typeJoueur : char {
    Agent           = 1,
    Espion          = 2,
    ErreurJoueur    = 0
};

enum message_type : uint8_t {
    MSG_TYPE_NOP            = 0,
    MSG_TYPE_BOARD          = 1, // Format : 1/0 si blue commence + nom + 0 + typeCarte + 0/1 si guess déjà + nom ....
    MSG_TYPE_GUESS          = 2, // Format : un char, position de la carte
    MSG_TYPE_UPDATE         = 3,
    MSG_TYPE_PING           = 4,
    MSG_TYPE_GET_BOARD      = 5,
    MSG_TYPE_ASK_TYPE       = 6,
    MSG_TYPE_TYPE_JOUEUR    = 7 // Format : typeJoueur
};

struct message_header {
    uint8_t length;
    uint8_t type;
} __attribute__((packed));

#endif // PROTOCOLE_H
