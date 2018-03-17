#ifndef CHARACTER_H
# define CHARACTER_H

# include <stdint.h>

# include "dims.h"

# ifdef __cplusplus


class character
{
  char symbol;
  pair_t position;
  int32_t speed;
  uint32_t alive;
  /* Characters use to have a next_turn for the move queue.  Now that it is *
   * an event queue, there's no need for that here.  Instead it's in the    *
   * event.  Similarly, sequence_number was introduced in order to ensure   *
   * that the queue remains stable.  Also no longer necessary here, but in  *
   * this case, we'll keep it, because it provides a bit of interesting     *
   * metadata: locally, how old is this character; and globally, how many   *
   * characters have been created by the game.                              */
  uint32_t sequence_number;
  /*
  npc_t *npc;
  pc_t *pc;
  */
};

extern "C"{
# else
typedef void character;
# endif

typedef struct dungeon dungeon_t;

typedef enum kill_type {
  kill_direct,
  kill_avenged,
  num_kill_types
} kill_type_t;

int32_t compare_characters_by_next_turn(const void *character1,
                                        const void *character2);
uint32_t can_see(dungeon_t *d, character_t *voyeur, character_t *exhibitionist);
void character_delete(void *c);

char get_symbol(const character &c);
int set_symbol(character &c, char symbol);

int *get_position(const character &c);
int set_position(character &c, int xPos, int yPos);

int get_speed(const character &c);
int set_speed(character &c, int speed);

int get_life(const character &c);
int set_life(character &c, int life);

# ifdef __cplusplus
}
# endif

#endif
