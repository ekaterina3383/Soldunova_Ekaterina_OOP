#ifndef ABILITYMANAGER_H
#define ABILITYMANAGER_H

#include "Ship.h"
#include "Ability.h"
#include <queue>
#include <memory>
using namespace std;

enum class AbilityType {DoubleDamage, Scanner, Bombing};

class AbilityManager{
	queue<unique_ptr<Ability>> queue_of_abilities;
public:
	AbilityManager();
	void AddAbility();
	int GetNumberOfAbilities();
	bool ApplicationOfAbilities(Field field, ShipManager manager);
	int GetLength();
};
#endif
