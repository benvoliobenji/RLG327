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

public:
	dave();
	int explode();
	int give_reward();
};

#endif
