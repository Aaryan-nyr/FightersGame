#include "project.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// TODO:: implement your project here!
// creating a pointer for players in game
void create_characters(Game *game, unsigned int num_players) {
  if (game == NULL) {
    fprintf(stderr, "Error: Game is NULL\n");
    return;
  }
  game->arr.data = (Character *)malloc(num_players * sizeof(Character));
  game->arr.size = 0;
  game->arr.capacity = num_players;
}
// freeing memory
void free_characters(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "Error: Game is NULL\n");
    return;
  }
  free(game->arr.data);
  game->arr.data = NULL;
  game->arr.size = 0;
  game->arr.capacity = 0;
}
// allocating memory for character when added
void mem_allo_char(Players *characters, Character character) {
  if (characters->size == characters->capacity)  // check if max = size
  {
    characters->capacity++;  // increment capacity
    characters->data = (Character *)realloc(
        characters->data,
        characters->capacity * sizeof(Character));  // realloc for new character
    if (characters->data == NULL) {
      free(characters->data);
    }
  }
  characters->data[characters->size] = character;  // place character to positon
  characters->size++;                              // increase current size
}
int count_words_command(const char *command) {
  int count = 0;
  int i = 0;
  while (command[i] != '\0') {
    if (command[i] == ' ' && command[i + 1] != ' ') {
      count++;
    }
    i++;
  }
  return count;
}
// setting values for character added
void add_character(const char *command, Character *new_character) {
  char comm[2];  // for command
  if (count_words_command(command) == 4) {
    new_character->experience = 0;
    new_character->HP = 0;
    new_character->weapon.damage = 0;
    sscanf(command, "%s %s %d %s %d", comm, new_character->name,
           &new_character->HP, new_character->weapon.name,
           &new_character->weapon.damage);
    if (strlen(new_character->weapon.name) == 0) {
      printf("Weapon name cannot be empty or space.\n");
    }
    if (new_character->weapon.damage < 1) {
      printf("Max damage cannot be lower than 1\n");
    }
    if (new_character->HP < 1) {
      printf("HP cannot be lower than 1\n");
    }
  } else if (count_words_command(command) < 4) {
    printf("A should be followed by exactly 4 arguments.\n");
  }
}
// check if name already exists
int character_exists(Players *arr, Character *to_check) {
  for (unsigned int i = 0; i < arr->size; i++) {
    if (strcmp(arr->data[i].name, to_check->name) == 0) {
      // printf("ERROR\n");
      return 1;
    }
  }
  return 0;
}
// helper function to find a character
Character *find_character(Players *arr, const char *name) {
  for (unsigned int i = 0; i < arr->size; i++) {
    if (strcmp(arr->data[i].name, name) == 0) {
      return &arr->data[i];
    }
  }
  return NULL;
}
// attack function
void attack(const char *command, const Game *game) {
  char comm[2];
  char attacker[512];  // store attacker name
  char target[512];    // store target name
  if (count_words_command(command) == 2) {
    sscanf(command, "%s %s %s", comm, attacker, target);
    Character *attacker_character =
        find_character(&((Game *)game)->arr, attacker);
    Character *target_character = find_character(&((Game *)game)->arr, target);
    if (attacker_character == NULL || target_character == NULL) {
      printf("Player doesn't exist in database\n");
    } else {
      if (!character_exists(&((Game *)game)->arr, target_character) ||
          !character_exists(&((Game *)game)->arr, attacker_character)) {
        printf("Player doesn't exist in database\n");
      } else if (attacker_character != target_character &&
                 target_character->HP > 1) {
        target_character->HP -= attacker_character->weapon.damage;
        attacker_character->experience += attacker_character->weapon.damage;
        printf(
            "%s attacked %s with %s by %d damage.\n%s has %d hit points "
            "remaining.\n%s gained %d experience points.\n",
            attacker_character->name, target_character->name,
            attacker_character->weapon.name, attacker_character->weapon.damage,
            target_character->name, target_character->HP,
            attacker_character->name, attacker_character->weapon.damage);
        printf("SUCCESS\n");
      } else if (strcmp(attacker_character->name, target_character->name) ==
                 0) {
        printf("Attacker \"%s\" cannot attack to itself.\n",
               attacker_character->name);
      } else {
        printf("ERROR\n");
      }
    }
  } else {
    printf("H should be followed by exactly 2 arguments.\n");
  }
}
// compare function for sorting
int compare_exp(const void *p1, const void *p2) {
  const Character *play1 = p1;
  const Character *play2 = p2;
  return play2->experience - play1->experience;
}
// load current game
void print_game(const Game *game) {
  qsort((void *)(&game->arr.data[0]), game->arr.size, sizeof(Character),
        compare_exp);
  for (unsigned int i = 0; i < game->arr.size; i++) {
    printf("%s %d %d %s %d\n", game->arr.data[i].name, game->arr.data[i].HP,
           game->arr.data[i].experience, game->arr.data[i].weapon.name,
           game->arr.data[i].weapon.damage);
  }
}
// write to file
void write_file(const char *command, const Game *game) {
  if (count_words_command(command) == 1) {
    char comm[2];
    char filename[1024];
    sscanf(command, "%s %s ", comm, filename);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL || ferror(fp)) {
      printf("ERROR opening file %s\n", filename);
    }
    unsigned int i = 0;
    // char buff[1000];
    while (i < game->arr.size) {
      //<name> <hit-points> <experience> <weapon-name> <weapon-damage>
      fprintf(fp, "%s %d %d %s %d\n", game->arr.data[i].name,
              game->arr.data[i].HP, game->arr.data[i].experience,
              game->arr.data[i].weapon.name,
              game->arr.data[i].weapon.damage);  // writing into buffer
      i++;
    }
    fclose(fp);
    printf("SUCCESS\n");
  } else if (count_words_command(command) < 1) {
    printf("ERROR\n");
  }
}
// read from file
void load_file(const char *command, const Game *game) {
  char comm[2];
  char filename[1024];
  if (count_words_command(command) == 1) {
    sscanf(command, "%s %s", comm, filename);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL || ferror(fp)) {
      printf("Cannot open file %s for reading.\n", filename);
    }
    // char line[1024];
    else {
      unsigned int size = 0;
      if (((Game *)game->arr.data != NULL)) {
        free_characters((Game *)game);
      }
      ((Game *)game)->arr.data = calloc(1, sizeof(Character));
      while (fscanf(fp, "%s %d %d %s %d\n", game->arr.data[size].name,
                    &game->arr.data[size].HP, &game->arr.data[size].experience,
                    game->arr.data[size].weapon.name,
                    &game->arr.data[size].weapon.damage) == 5) {
        //<name> <hit-points> <experience> <weapon-name> <weapon-damage>
        ((Game *)game)->arr.data =
            realloc(((Game *)game)->arr.data, (size + 2) * sizeof(Character));
        size++;
      }
      printf("SUCCESS\n");
      fclose(fp);
    }
  } else {
    printf("ERROR\n");
  }
}
int main(void) {
  printf("Welcome to the game");
  Game game;
  game.arr.size = 0;
  game.arr.capacity = 0;
  game.arr.data = calloc(1, sizeof(Character));
  // memset(&game, 0, sizeof(game));
  char command[1000];
  fgets(command, sizeof(command), stdin);
  int done = 0;
  while (done != 1) {
    if (command[0] == 'A') {
      Character to_add;
      to_add.HP = 0;
      to_add.weapon.damage = 0;
      add_character(command, &to_add);
      if (!character_exists(&game.arr, &to_add) && to_add.HP >= 1 &&
          to_add.weapon.damage >= 1) {
        mem_allo_char(&game.arr, to_add);
        printf("SUCCESS\n");
      } else if (character_exists(&game.arr, &to_add)) {
        printf("ERROR\n");
      }
    } else if (command[0] == 'H') {
      attack(command, &game);
    } else if (command[0] == 'L') {
      print_game(&game);
      printf("SUCCESS\n");
    } else if (command[0] == 'W') {
      write_file(command, &game);
    } else if (command[0] == 'O') {
      load_file(command, &game);
    } else if (command[0] == 'Q') {
      free_characters(&game);
      printf("SUCCESS\n");
      break;
    } else {
      printf("Invalid command %c\n", command[0]);
    }
    fgets(command, sizeof(command), stdin);
  }
  return 0;
}