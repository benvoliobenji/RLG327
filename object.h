#ifndef OBJECT_H
# define OBJECT_H

# include <string>
# include "descriptions.h"
# include "dims.h"


class object {
private:
	std::string name;
	std::string description;
	object_type_t type;
	uint32_t color;
	int hp;
	int dodge;
	int defence;
	int weight;
	int speed;
	int attribute;
	int value;
	dice damage;
	bool artifact;
	uint32_t rarity;
	bool picked_up;
	int xPos;
	int yPos;

public:
	object(object_description obj, int xPos, int yPos);

	~object();

	string get_name();

	string get_description();

	object_type_t get_type();

	uint32_t get_color();

	int get_hp();

	int get_dodge();

	int get_defence();

	int get_weight();

	int get_speed();

	int get_attribute();

	int get_value();

	int32_t get_damage_base();

	uint32_t get_damage_number();

	uint32_t get_damage_side();

	bool get_artifact();

	uint32_t get_rarity();

	bool get_picked_up();

	bool pick_up();

	int get_xPos();

	int get_yPos();

	int set_xPos();

	int set_yPos();

	int roll_damage();
};
#endif
 
