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
};
#endif
 
