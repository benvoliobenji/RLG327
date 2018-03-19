#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "character.h"

# include "dims.h"

# ifdef __cplusplus
class pc: public character {
	terrain_type_t memory[21][80];
};

extern "C"{
# else
typedef void pc;
# endif

	typedef struct dungeon dungeon_t;

	void pc_delete(pc *pc);
	uint32_t pc_is_alive(dungeon_t *d);
	void config_pc(dungeon_t *d);
	uint32_t pc_next_pos(dungeon_t *d, pair_t dir);
	void place_pc(dungeon_t *d);
	uint32_t pc_in_room(dungeon_t *d, uint32_t room);

	int update_memory(dungeon_t *d, pc *pc);
	int reset_memory(pc *pc);

# ifdef __cplusplus
}
# endif

#endif
