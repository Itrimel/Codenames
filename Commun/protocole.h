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

enum message_type : uint32_t {
    MSG_TYPE_NOP        = 0,
    MSG_TYPE_BOARD      = 1,
    MSG_TYPE_GUESS      = 2,
    MSG_TYPE_UPDATE     = 3,
    MSG_TYPE_PING       = 4,
    MSG_TYPE_GET_BOARD  = 5
};

struct message_header {
    uint32_t length;
    uint32_t type;
} __attribute__((packed));

#endif // PROTOCOLE_H
