#ifndef DAVE_H
# define DAVE_H

# include "character.h"
# include "dungeon.h"
# include "dims.h"
# include "dice.h"

class dave : public character {
private:
	int gold;
	dice *explosives;
	bool following;
	pair_t next_pos;

public:
	dave();
	int explode();
	int give_reward();
	void set_next_pos(pair_t position);
	void follow();
};

#endif
