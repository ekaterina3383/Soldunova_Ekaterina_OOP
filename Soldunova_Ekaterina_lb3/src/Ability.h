#ifndef ABILITY_H
#define ABILITY_H

#include "Field.h"
#include "ShipManager.h"

class Ability{
public:
	virtual bool ApplyingAbility(Field field, ShipManager manager) = 0;
};

class DoubleDamage: public Ability{
public:
	bool ApplyingAbility(Field field, ShipManager manager);
};

class Scanner: public Ability{
public:
	bool ApplyingAbility(Field field, ShipManager manager);
};

class Bombing: public Ability{
public:
	bool ApplyingAbility(Field field, ShipManager manager);
};
#endif
