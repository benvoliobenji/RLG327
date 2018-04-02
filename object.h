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
	bool seen;
	uint32_t rarity;
	bool picked_up;
	int xPos;
	int yPos;

public:
	//object(object_description obj, int xPos, int yPos);

	string get_name();

	int set_name(string name);

	string get_description();

	int set_description(string description);

	object_type_t get_type();

	int set_type(object_type_t type);

	uint32_t get_color();

	int set_color(uint32_t color);

	int get_hp();

	int set_hp(dice hp);

	int get_dodge();

	int set_dodge(dice dodge);

	int get_defence();

	int set_defence(dice defence);

	int get_weight();

	int set_weight(dice weight);

	int get_speed();

	int set_speed(dice speed)

	int get_attribute();

	int set_attribute(dice attribute);

	int get_value();

	int set_value(dice value);

	int32_t get_damage_base();

	uint32_t get_damage_number();

	uint32_t get_damage_side();

	int set_damage(dice damage);

	bool get_artifact();

	int set_artifact(bool artifact);

	uint32_t get_rarity();

	int set_rarity(uint32_t rarity);

	bool get_picked_up();

	bool pick_up();

	int get_xPos();

	int get_yPos();

	int set_xPos(int xPos);

	int set_yPos(int yPos);

	int roll_damage();

	bool get_seen();

	bool seen();

	pair_t get_pos();
};
#endif
