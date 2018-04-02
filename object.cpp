#include <vector>
#include <cstring>

#include "dungeon.h"
#include "object.h"


/*
object(object_description *obj, int xPos, int yPos) {
	this.name = obj->get_name();
	this.description = obj->get_description();
	this.type = obj->get_type();
	this.color = obj->get_color();
	this.hp = obj->get_hit().roll();
	this.dodge = obj->get_dodge().roll();
	this.defence = obj->get_defence().roll();
	this.weight = obj->get_weight().roll();
	this.speed = obj->get_speed().roll();
	this.attribute = obj->get_attribute().roll();
	this.value = obj->get_value().roll();
	this.damage = obj->damage;
	this.artifact = obj->artifact;
	this.rarity = obj->rarity;
	this.picked_up = false;
	this.xPos = xPos;
	this.yPos = yPos;
	this.seen = false;
}
*/
string object::get_name()
{
	return this.name;
}

int object::set_name(string name)
{
	this.name = name;
	return 0;
}

string object::get_description()
{
	return this.description;
}

int object::set_description(string description)
{
	this.description = description;
	return 0;
}

object_type_t object::get_type()
{
	return this.type;
}

int object::set_type(object_type_t type)
{
	this.type = type;
	return 0;
}

uint32_t object::get_color()
{
	return this.color;
}

int object::set_color(uint32_t color)
{
	this.color = color;
	return 0;
}

int object::get_hp()
{
	return this.hp;
}

int object::set_hp(dice hp)
{
	this.hp = hp.roll();
	return 0;
}

int object::get_dodge()
{
	return this.dodge;
}

int object::set_dodge(dice dodge)
{
	this.dodge = dodge.roll();
	return 0;
}

int object::get_defence()
{
	return this.defence;
}

int object::set_defence(dice defence)
{
	this.defence = defence.roll();
	return 0;
}

int object::get_weight()
{
	return this.weight;
}

int object::set_weight(dice weight)
{
	this.weight = weight.roll();
	return 0;
}

int object::get_speed()
{
	return this.speed;
}

int object::set_speed(dice speed)
{
	this.speed = speed.roll();
	return 0;
}

int object::get_attribute()
{
	return this.attribute;
}

int object::set_attribute(dice attribute)
{
	this.attribute = attribute.roll();
	return 0;
}

int object::get_value()
{
	return this.value;
}

int object::set_value(dice value)
{
	this.value = value.roll();
	return 0;
}

int32_t object::get_damage_base()
{
	return this.damage.get_base();
}

uint32_t object::get_damage_number()
{
	return this.damage.get_number();
}

uint32_t object::get_damage_side()
{
	return this.damage.get_sides();
}

int object::set_damage(dice damage)
{
	this.damage = damage;
	return 0;
}

bool object::get_artifact()
{
	return this.artifact;
}

int object::set_artifact(bool artifact)
{
	this.artifact = artifact;
	return 0;
}

uint32_t object::get_rarity()
{
	return this.rarity;
}

int object::set_rarity(uint32_t rarity)
{
	this.rarity = rarity;
	return 0;
}

bool object::get_picked_up()
{
	return this.picked_up();
}

bool object::pick_up()
{
	this.pick_up = true;
	return true;
}

int object::get_xPos()
{
	return this.xPos;
}

int object::get_yPos()
{
	return this.yPos;
}

pair_t object::get_pos() {
	pair_t p;
	p[dim_y] = this.yPos;
	p[dim_x] = this.xPos;

	return p;
}

int object::set_xPos(int xPos)
{
	this.xPos = xPos;
	return 0;
}

int object::set_yPos(int yPos)
{
	this.yPos = yPos;
	return 0;
}

int object::roll_damage()
{
	return this.damage.roll();
}

void create_objects(dungeon *d) {
	object *o;

	for (int i = 0; i < d->num_objects; i++) {
		o = create_object(d);
		d->object_map[o.get_yPos()][o.get_xPos()] = o;
	}
}

bool get_seen() {
	return this.seen;
}

bool seen() {
	this.seen = true;
	return true;
}
