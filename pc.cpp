#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "io.h"
#include "object.h"

uint32_t pc_is_alive(dungeon_t *d)
{
  return d->PC && d->PC->alive;
}

void place_pc(dungeon_t *d)
{
  d->PC->position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->PC->position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));

  pc_init_known_terrain(d->PC);
  pc_observe_terrain(d->PC, d);

  io_display(d);
}

void config_pc(dungeon_t *d)
{
  static dice pc_dice(0, 1, 4);

  d->PC = new pc;

  d->PC->symbol = '@';

  place_pc(d);

  d->PC->speed = PC_SPEED;
  d->PC->alive = 1;
  d->PC->sequence_number = 0;
  d->PC->kills[kill_direct] = d->PC->kills[kill_avenged] = 0;
  d->PC->color.push_back(COLOR_WHITE);
  d->PC->damage = &pc_dice;
  d->PC->name = "Isabella Garcia-Shapiro";

  for (int i = 0; i < 11; i++) {
	  object *o = NULL;

	  d->PC->equipment.push_back(o);
  }

  for (int i = 0; i < 9; i++) {
	  object *o = NULL;

	  d->PC->inventory.push_back(o);
  }

  d->character_map[character_get_y(d->PC)][character_get_x(d->PC)] = d->PC;

  dijkstra(d);
  dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon_t *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;

  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, d->PC)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  /* First, eat anybody standing next to us. */
  if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    /* Head to a corner and let most of the NPCs kill each other off */
    if (count) {
      count++;
    }
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir_nearest_wall(d, d->PC, dir);
    }
  }else {
    /* And after we've been there, let's head toward the center of the map. */
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir[dim_x] = ((d->PC->position[dim_x] > DUNGEON_X / 2) ? -1 : 1);
      dir[dim_y] = ((d->PC->position[dim_y] > DUNGEON_Y / 2) ? -1 : 1);
    }
  }

  /* Don't move to an unoccupied location if that places us next to a monster */
  if (!charxy(d->PC->position[dim_x] + dir[dim_x],
              d->PC->position[dim_y] + dir[dim_y]) &&
      ((charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  return 0;
}

uint32_t pc_in_room(dungeon_t *d, uint32_t room)
{
  if ((room < d->num_rooms)                                     &&
      (d->PC->position[dim_x] >= d->rooms[room].position[dim_x]) &&
      (d->PC->position[dim_x] < (d->rooms[room].position[dim_x] +
                                d->rooms[room].size[dim_x]))    &&
      (d->PC->position[dim_y] >= d->rooms[room].position[dim_y]) &&
      (d->PC->position[dim_y] < (d->rooms[room].position[dim_y] +
                                d->rooms[room].size[dim_y]))) {
    return 1;
  }

  return 0;
}

void pc_learn_terrain(pc *p, pair_t pos, terrain_type_t ter)
{
  p->known_terrain[pos[dim_y]][pos[dim_x]] = ter;
  p->visible[pos[dim_y]][pos[dim_x]] = 1;
}

void pc_reset_visibility(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->visible[y][x] = 0;
    }
  }
}

terrain_type_t pc_learned_terrain(pc *p, int16_t y, int16_t x)
{
  if (y < 0 || y >= DUNGEON_Y || x < 0 || x >= DUNGEON_X) {
    io_queue_message("Invalid value to %s: %d, %d", __FUNCTION__, y, x);
  }

  return p->known_terrain[y][x];
}

void pc_init_known_terrain(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->known_terrain[y][x] = ter_unknown;
      p->visible[y][x] = 0;
    }
  }
}

void pc_observe_terrain(pc *p, dungeon_t *d)
{
  pair_t where;
  int16_t y_min, y_max, x_min, x_max;

  y_min = p->position[dim_y] - PC_VISUAL_RANGE;
  if (y_min < 0) {
    y_min = 0;
  }
  y_max = p->position[dim_y] + PC_VISUAL_RANGE;
  if (y_max > DUNGEON_Y - 1) {
    y_max = DUNGEON_Y - 1;
  }
  x_min = p->position[dim_x] - PC_VISUAL_RANGE;
  if (x_min < 0) {
    x_min = 0;
  }
  x_max = p->position[dim_x] + PC_VISUAL_RANGE;
  if (x_max > DUNGEON_X - 1) {
    x_max = DUNGEON_X - 1;
  }

  for (where[dim_y] = y_min; where[dim_y] <= y_max; where[dim_y]++) {
    where[dim_x] = x_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_x] = x_max;
    can_see(d, p->position, where, 1, 1);
  }
  /* Take one off the x range because we alreay hit the corners above. */
  for (where[dim_x] = x_min - 1; where[dim_x] <= x_max - 1; where[dim_x]++) {
    where[dim_y] = y_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_y] = y_max;
    can_see(d, p->position, where, 1, 1);
  }       
}

int32_t is_illuminated(pc *p, int16_t y, int16_t x)
{
  return p->visible[y][x];
}

void pc_see_object(character *the_pc, object *o)
{
  if (o) {
    o->has_been_seen();
  }
}

void pc::pick_up(dungeon *d)
{
	if (d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] == NULL) {
		return;
	}

	bool full_inventory = true;
	int open_slot;

	for (int i = 0; i < this->inventory.size(); i++) {
		if (this->inventory[i] == NULL) {
			full_inventory = false;
			open_slot = i;
		}
	}

	if (full_inventory) {
		return;
	}

	this->inventory[open_slot] = d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]];
	d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = NULL;

}

void pc::equip(object *o)
{
	switch (o->get_type()) {

	//For WEAPON
	case 1 :
		if (this->equipment[0] == NULL) {
			this->equipment[0] = o;
		}
		else {
			object *old_equipment = this->equipment[0];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[0] = o;
				}
			}
		}

		break;

	//For OFFHAND
	case 2:
		if (this->equipment[1] == NULL) {
			this->equipment[1] = o;
		}
		else {
			object *old_equipment = this->equipment[1];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[1] = o;
				}
			}
		}

		break;

	//For RANGED
	case 3:
		if (this->equipment[2] == NULL) {
			this->equipment[2] = o;
		}
		else {
			object *old_equipment = this->equipment[2];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[2] = o;
				}
			}
		}

		break;

	//For LIGHT
	case 4:
		if (this->equipment[3] == NULL) {
			this->equipment[3] = o;
		}
		else {
			object *old_equipment = this->equipment[3];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[3] = o;
				}
			}
		}

		break;

	//For ARMOR
	case 5:
		if (this->equipment[4] == NULL) {
			this->equipment[4] = o;
		}
		else {
			object *old_equipment = this->equipment[4];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[4] = o;
				}
			}
		}

		break;

	//For HELMET
	case 6:
		if (this->equipment[5] == NULL) {
			this->equipment[5] = o;
		}
		else {
			object *old_equipment = this->equipment[5];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[5] = o;
				}
			}
		}

		break;

	//For CLOAK
	case 7:
		if (this->equipment[6] == NULL) {
			this->equipment[6] = o;
		}
		else {
			object *old_equipment = this->equipment[6];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[6] = o;
				}
			}
		}

		break;

	//For GLOVES
	case 8:
		if (this->equipment[7] == NULL) {
			this->equipment[7] = o;
		}
		else {
			object *old_equipment = this->equipment[7];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[7] = o;
				}
			}
		}

		break;

	//For BOOTS
	case 9:
		if (this->equipment[8] == NULL) {
			this->equipment[8] = o;
		}
		else {
			object *old_equipment = this->equipment[8];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[8] = o;
				}
			}
		}

		break;

	//For AMULET
	case 10:
		if (this->equipment[9] == NULL) {
			this->equipment[9] = o;
		}
		else {
			object *old_equipment = this->equipment[9];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[9] = o;
				}
			}
		}

		break;

	//For RING
	case 11:
		if (this->equipment[10] == NULL) {
			this->equipment[10] = o;
		}
		else if (this->equipment[11] == NULL) {
			this->equipment[11] = o;
		}
		else {
			object *old_equipment = this->equipment[1];

			for (int i = 0; i < this->inventory.size(); i++) {
				if (this->inventory[i]->get_name() == o->get_name()) {
					this->inventory[i] = old_equipment;
					this->equipment[10] = o;
				}
			}
		}

		break;
	}
}


int pc::take_off(int equipment_pos)
{
	bool full_inventory = true;
	int open_slot;

	//Test to see if given position to remove is NULL
	if (this->equipment[equipment_pos] == NULL) {
		return 1;
	}

	//Test to see if there is a full inventory
	for (int i = 0; i < this->inventory.size(); i++) {
		if (this->inventory[i] == NULL) {
			full_inventory = false;
			open_slot = i;
		}
	}

	if (full_inventory) {
		return 2;
	}

	this->inventory[open_slot] = this->equipment[equipment_pos];
	this->equipment[equipment_pos] = NULL;

	return 0;
}

void pc::drop(dungeon * d, int inventory_pos)
{
	if (this->inventory[inventory_pos] == NULL) {
		return;
	}

	if (d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] == NULL) {
		d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = this->inventory[inventory_pos];
	}
	else {
		d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]]->next = this->inventory[inventory_pos];
	}

	this->inventory[inventory_pos] = NULL;
}

void pc::destroy(int inventory_pos)
{
	if (this->inventory[inventory_pos] == NULL) {
		return;
	}

	this->inventory[inventory_pos].delete();
	this->inventory[inventory_pos] = NULL;
}

int pc::damage_roll()
{
	int total_damage;

	total_damage += this->damage->roll();

	for (int i = 0; i < this->equipment.size(); i++) {
		if (this->equipment[i] != NULL) {
			total_damage += this->equipment[i]->roll_damage();
		}
	}

	return total_damage;
}


