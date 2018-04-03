#ifndef OBJECT_H
# define OBJECT_H

# include <string>
# include "descriptions.h"
# include "dims.h"
# include "dice.h"

class dungeon;

class object {
private:
	std::string name;
	std::string description;
	char type;
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

	pair_t position;

	std::string get_name();

	int set_name(std::string name);

	std::string get_description();

	int set_description(std::string description);

	char get_type();

	int set_type(char type);

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

	int set_speed(dice speed);

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

	bool pick_up(dungeon *d);

	bool on_floor();

	int get_xPos();

	int get_yPos();

	int set_xPos(int xPos);

	int set_yPos(int yPos);

	int roll_damage();

	bool get_seen();

	bool is_seen();

	bool not_seen();

	//pair_t get_pos();
};

	void create_objects(dungeon *d);

#endif
