#ifndef DAVE_H
# define DAVE_H

# include "character.h"
# include "dungeon.h"
# include "dims.h"
# include "dice.h"

class dave : public character {
private:
	int gold;
	int explosives;
	bool following;

public:
	dave();
	void explode(dungeon *d);
	int give_reward();
	void follow();
	bool is_following();
	int show_reward();
	int show_explosives();
};

void gen_dave(dungeon *d);

#endif
