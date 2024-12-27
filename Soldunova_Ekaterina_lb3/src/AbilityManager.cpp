#include "AbilityManager.h"
#include "Exceptions.h"
#include <queue>
using namespace std;

AbilityManager::AbilityManager(){
	queue_of_abilities.push(make_unique<Bombing>());
	queue_of_abilities.push(make_unique<Scanner>());
	queue_of_abilities.push(make_unique<DoubleDamage>());
}
void AbilityManager::AddAbility(){
	int ability_ind = rand() % 3;
	switch (ability_ind){
		case 0: queue_of_abilities.push(make_unique<DoubleDamage>());
		case 1: queue_of_abilities.push(make_unique<Scanner>());
		case 2: queue_of_abilities.push(make_unique<Bombing>());
	}
	cout << "You have gained a new ability!" << endl;
}
int AbilityManager::GetNumberOfAbilities(){return queue_of_abilities.size();}
bool AbilityManager::ApplicationOfAbilities(Field field, ShipManager manager){
	if (queue_of_abilities.size()==0){
		throw NoAbilityException{};
	}
	unique_ptr<Ability> ability = move(queue_of_abilities.back());
       	ability->ApplyingAbility(field, manager);
	queue_of_abilities.pop();
	return 1;
}
int AbilityManager::GetLength(){return queue_of_abilities.size();}
