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

public:
	dave();
	void explode(dungeon *d);
	int give_reward();
	void follow();
	bool is_following();
};

#endif
