#ifndef _PROJECT__H_
#define _PROJECT__H_

typedef struct
{
    char name[256];
    int damage;
} Object;

typedef struct
{
    char name[512];
    int HP;
    int experience;
    Object weapon;
} Character;

typedef struct
{
    Character *data;       // array for all characters in game
    unsigned int size;     // current size of array
    unsigned int capacity; // max capacity
} Players;

typedef struct
{
    Players arr;
    unsigned int num_players;
} Game;

#endif //! _PROJECT__H_