#include "move.h"

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "dungeon.h"
#include "heap.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "character.h"
#include "utils.h"
#include "path.h"
#include "event.h"
#include "io.h"
#include "npc.h"
#include "object.h"
#include "dave.h"

void do_combat(dungeon_t *d, character *atk, character *def)
{
  uint32_t damage, i;
  const char *organs[] = {
    "liver",
    "pancreas",
    "heart",
    "brain",
    "eye",
    "arm",
    "leg",
    "intestines",
    "gall bladder",
    "lungs",
    "hand",
    "foot",
    "spinal cord",
    "pituitary gland",
    "thyroid",
    "tongue",
    "bladder",
    "diaphram",
    "frontal lobe",
    "hippocampus",
    "stomach",
    "pharynx",
    "esophagus",
    "trachea",
    "urethra",
    "spleen",
    "cerebellum",
    "ganglia",
    "ear",
    "subcutaneous tissue",
    "prefrontal cortex"
  };
  const char *attacks[] = {
    "punches",
    "kicks",
    "stabs",
    "impales",
    "slashes",
    "massages",
    "soothes",
    "bites",
    "jabs",
    "coerces",
    "threatens",
    "manipulates",
    "arm locks",
    "conquers",
    "buries the hatchet in",
    "indicates displeasure with",
    "quarrels with",
    "scrimmages with",
    "tickles",
    "engages in fisticuffs with",
    "strikes",
    "belts",
    "wallops",
    "gives the old one-two to",
    "bumps into",
    "behaves inappropriately with",
    "smacks",
    "body slams",
    "fondues",
    "flambes",
    "pokes",
    "anoints",
  };

  character *defender = def;


  if (character_is_alive(defender)) {
    if (atk != d->PC && (defender == d->PC)) {
      if (d->PC->dave_following.size() == 0) {
	damage = atk->damage->roll();
	io_queue_message("%s%s %s your %s for %d.", is_unique(atk) ? "" : "The ",
			 atk->name, attacks[rand() % (sizeof(attacks) /
						      sizeof(attacks[0]))],
			 organs[rand() % (sizeof(organs) /
					  sizeof(organs[0]))], damage);
      }
      else {
	damage = atk->damage->roll();
	
	io_queue_message("Dave heroically");
	io_queue_message("jumps in front of you");
	io_queue_message("to block %d damage!", damage);
	defender = d->PC->dave_following.back();
      }
    }
    else if (atk != d->PC && defender != d->PC) {
      damage = atk->damage->roll();
      io_queue_message("Dave is taking %d damage!", damage);
    }else {
      for (i = damage = 0; i < num_eq_slots; i++) {
        if (i == eq_slot_weapon && !d->PC->eq[i]) {
          damage += atk->damage->roll();
        } else if (d->PC->eq[i]) {
          damage += d->PC->eq[i]->roll_dice();
        }
      }
      io_queue_message("You hit %s%s for %d.", is_unique(def) ? "" : "the ",
                       def->name, damage);
    }
    
    if (damage >= defender->hp) {
      if (atk != d->PC && defender == d->PC) {
        io_queue_message("You die.");
        io_queue_message("As %s%s eats your %s,", is_unique(atk) ? "" : "the ",
                         atk->name, organs[rand() % (sizeof (organs) /
                                                     sizeof (organs[0]))]);
        io_queue_message("   ...you wonder if there is an afterlife.");
        /* Queue an empty message, otherwise the game will not pause for *
         * player to see above.                                          */
        io_queue_message("");
      }
      else if(atk != d->PC && defender != d->PC) {
        dave *exploding_dave = (dave *) defender;
        io_queue_message("Dave takes a killing blow,");
	io_queue_message("setting off a massive explosion!");
        exploding_dave->explode(d);
      }
      else {
        io_queue_message("%s%s dies.", is_unique(def) ? "" : "The ", def->name);
      }
      defender->hp = 0;
      defender->alive = 0;
      character_increment_dkills(atk);
      character_increment_ikills(atk, (character_get_dkills(def) +
                                       character_get_ikills(def)));
      if (defender != d->PC && (defender->name != (char *) "Dave")) {
        d->num_monsters--;
      }
      else if(pc_is_alive(d) && (defender->name == (char *) "Dave")) {
        int i = 0;
        //Accounting for the deaths of multiple Daves
        while(i < (int) d->PC->dave_following.size()) {
          if(character_is_alive(d->PC->dave_following[i]) == 0) {
            d->PC->dave_following.erase(d->PC->dave_following.begin() + i);
            i = 0;
          }
          else {
            i++;
          }
        }
      }
      charpair(defender->position) = NULL;
    } else {
      defender->hp -= damage;
    }
  }
}

void move_character(dungeon_t *d, character *c, pair_t next)
{
  int can_see_atk, can_see_def;
  pair_t displacement;
  pair_t prev;
  uint32_t found_cell;
  pair_t order[9] = {
    { -1, -1 },
    { -1,  0 },
    { -1,  1 },
    {  0, -1 },
    {  0,  0 },
    {  0,  1 },
    {  1, -1 },
    {  1,  0 },
    {  1,  1 },
  };
  uint32_t s, i;

  if (charpair(next) &&
      ((next[dim_y] != c->position[dim_y]) ||
       (next[dim_x] != c->position[dim_x]))) {
    if (c == d->PC && (d->character_map[next[dim_y]][next[dim_x]]->name == (char *)"Dave")) {
	    dave *dungeon_dave = (dave *)d->character_map[next[dim_y]][next[dim_x]];

	    //Adding a new Dave to the party
	    if (!dungeon_dave->is_following()) {
	      dungeon_dave->follow();
	      d->PC->dave_following.push_back(dungeon_dave);
	    }

	    prev[dim_y] = c->position[dim_y];
	    prev[dim_x] = c->position[dim_x];

	    d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
	    c->position[dim_y] = next[dim_y];
	    c->position[dim_x] = next[dim_x];
	    d->character_map[c->position[dim_y]][c->position[dim_x]] = c;
    }
    else if ((charpair(next) == d->PC) ||
	     c == d->PC) {
      do_combat(d, c, charpair(next));
    }
    else if (d->character_map[next[dim_y]][next[dim_x]]->name == (char *) "Dave" && c != d->PC) {
      do_combat(d, c, charpair(next));
    } else {
      /* Easiest way for a monster to displace another monster is *
       * to swap them.  This could lead to some strangeness where *
       * two monsters of the exact same speed continually         *
       * displace each other and never make progress, but I don't *
       * have any real problem with that.  When we have better    *
       * game balance, weaker monsters should not be able to      *
       * displace stronger monsters.                              */
      /* Turns out I don't like swapping them after all.  We'll   *
       * instead select a random square from the 8 surrounding    *
       * the target cell.  Keep doing it until either we swap or  *
       * find an empty one for the displacement.                  */
      for (s = rand() % 9, found_cell = i = 0;
           i < 9 && !found_cell; i++) {
        displacement[dim_y] = next[dim_y] + order[s % 9][dim_y];
        displacement[dim_x] = next[dim_x] + order[s % 9][dim_x];
        if (((npc *) charpair(next))->characteristics & NPC_PASS_WALL) {
          if (!charpair(displacement) ||
              (charpair(displacement) == c)) {
            found_cell = 1;
          }
        } else {
          if ((!charpair(displacement) &&
               (mappair(displacement) >= ter_floor)) ||
              (charpair(displacement) == c)) {
            found_cell = 1;
          }
        }
      }

      if (!found_cell) {
        return;
      }

      assert(charpair(next));

      can_see_atk = can_see(d, character_get_pos(d->PC),
                            character_get_pos(c), 1, 0);
      can_see_def = can_see(d, character_get_pos(d->PC),
                            character_get_pos(charpair(next)), 1, 0);

      if (can_see_atk && can_see_def) {
        io_queue_message("%s%s pushes %s%s out of the way.  How rude.",
                         is_unique(c) ? "" : "The ", c->name,
                         is_unique(charpair(next)) ? "" : "the ",
                         charpair(next)->name);
      } else if (can_see_atk) {
        io_queue_message("%s%s angrily shoves something out of the way.",
                         is_unique(c) ? "" : "The ", c->name);
      } else if (can_see_def) {
        io_queue_message("Something slams %s%s out of the way.",
                          is_unique(charpair(next)) ? "" : "the ",
                         charpair(next)->name);
      }

      charpair(c->position) = NULL;
      charpair(displacement) = charpair(next);
      charpair(next) = c;
      charpair(displacement)->position[dim_y] = displacement[dim_y];
      charpair(displacement)->position[dim_x] = displacement[dim_x];
      c->position[dim_y] = next[dim_y];
      c->position[dim_x] = next[dim_x];
    }
  } else {
    /* No character in new position. */

    d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character_map[c->position[dim_y]][c->position[dim_x]] = c;
  }

  if (c == d->PC) {
    pc_reset_visibility((pc *) c);
    pc_observe_terrain((pc *) c, d);

    if (d->PC->dave_following.size() != 0) {
      d->PC->update_dave_positions(d, prev);
    }
  }
}

void do_moves(dungeon *d)
{
  pair_t next;
  character *c;
  event_t *e;

  /* Remove the PC when it is PC turn.  Replace on next call.  This allows *
   * use to completely uninit the heap when generating a new level without *
   * worrying about deleting the PC.                                       */

  if (pc_is_alive(d)) {
    /* The PC always goes first one a tie, so we don't use new_event().  *
     * We generate one manually so that we can set the PC sequence       *
     * number to zero.                                                   */
    e = (event_t *) malloc(sizeof (*e));
    e->type = event_character_turn;
    /* Hack: New dungeons are marked.  Unmark and ensure PC goes at d->time, *
     * otherwise, monsters get a turn before the PC.                         */
    if (d->is_new) {
      d->is_new = 0;
      e->time = d->time;
    } else {
      e->time = d->time + (1000 / d->PC->speed);
    }
    e->sequence = 0;
    e->c = d->PC;
    heap_insert(&d->events, e);
  }

  while (pc_is_alive(d) &&
         (e = (event_t *) heap_remove_min(&d->events)) &&
         ((e->type != event_character_turn) || (e->c != d->PC))) {
    d->time = e->time;
    if (e->type == event_character_turn) {
      c = e->c;
    }
    if (!c->alive) {
      if (d->character_map[c->position[dim_y]][c->position[dim_x]] == c) {
        d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
      }
      if (c != d->PC) {
        event_delete(e);
      }
      continue;
    }

    npc_next_pos(d, (npc *) c, next);
    move_character(d, c, next);

    heap_insert(&d->events, update_event(d, e, 1000 / c->speed));
  }

  io_display(d);
  if (pc_is_alive(d) && e->c == d->PC) {
    c = e->c;
    d->time = e->time;
    /* Kind of kludgey, but because the PC is never in the queue when   *
     * we are outside of this function, the PC event has to get deleted *
     * and recreated every time we leave and re-enter this function.    */
    e->c = NULL;
    event_delete(e);
    io_handle_input(d);
  }
}

void dir_nearest_wall(dungeon *d, character *c, pair_t dir)
{
  dir[dim_x] = dir[dim_y] = 0;

  if (c->position[dim_x] != 1 && c->position[dim_x] != DUNGEON_X - 2) {
    dir[dim_x] = (c->position[dim_x] > DUNGEON_X - c->position[dim_x] ? 1 : -1);
  }
  if (c->position[dim_y] != 1 && c->position[dim_y] != DUNGEON_Y - 2) {
    dir[dim_y] = (c->position[dim_y] > DUNGEON_Y - c->position[dim_y] ? 1 : -1);
  }
}

uint32_t against_wall(dungeon *d, character *c)
{
  return ((mapxy(c->position[dim_x] - 1,
                 c->position[dim_y]    ) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x] + 1,
                 c->position[dim_y]    ) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x]    ,
                 c->position[dim_y] - 1) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x]    ,
                 c->position[dim_y] + 1) == ter_wall_immutable));
}

uint32_t in_corner(dungeon *d, character *c)
{
  uint32_t num_immutable;

  num_immutable = 0;

  num_immutable += (mapxy(c->position[dim_x] - 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x] + 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] - 1) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] + 1) == ter_wall_immutable);

  return num_immutable > 1;
}

static void new_dungeon_level(dungeon *d, uint32_t dir)
{
  /* Eventually up and down will be independantly meaningful. *
   * For now, simply generate a new dungeon.                  */

  switch (dir) {
  case '<':
    io_queue_message("You go up the stairs.");
    io_queue_message(""); /* To force "more" */
    io_display(d); /* To force queue flush */
    new_dungeon(d);
    break;
  case '>':
    io_queue_message("You go down the stairs.");
    io_queue_message(""); /* To force "more" */
    io_display(d); /* To force queue flush */
    new_dungeon(d);
    break;
  default:
    break;
  }
}


uint32_t move_pc(dungeon *d, uint32_t dir)
{
  pair_t next;
  uint32_t was_stairs = 0;

  next[dim_y] = d->PC->position[dim_y];
  next[dim_x] = d->PC->position[dim_x];


  switch (dir) {
  case 1:
  case 2:
  case 3:
    next[dim_y]++;
    break;
  case 4:
  case 5:
  case 6:
    break;
  case 7:
  case 8:
  case 9:
    next[dim_y]--;
    break;
  }
  switch (dir) {
  case 1:
  case 4:
  case 7:
    next[dim_x]--;
    break;
  case 2:
  case 5:
  case 8:
    break;
  case 3:
  case 6:
  case 9:
    next[dim_x]++;
    break;
  case '<':
    if (mappair(d->PC->position) == ter_stairs_up) {
      was_stairs = 1;
      new_dungeon_level(d, '<');
    }
    break;
  case '>':
    if (mappair(d->PC->position) == ter_stairs_down) {
      was_stairs = 1;
      new_dungeon_level(d, '>');
    }
    break;
  }

  if (was_stairs) {
    if (d->PC->dave_following.size() != 0) {
      for (int i = 0; i < (int)d->PC->dave_following.size(); i++) {
	d->PC->wallet += d->PC->dave_following[i]->give_reward();
      }
      
      d->PC->dave_following.clear();
    }
    return 0;
  }

  if ((dir != '>') && (dir != '<') && (mappair(next) >= ter_floor)) {
    move_character(d, d->PC, next);
    dijkstra(d);
    dijkstra_tunnel(d);
    d->PC->pick_up(d);

    return 0;
  }

  return 1;
}
