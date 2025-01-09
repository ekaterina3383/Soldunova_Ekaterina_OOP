#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <fstream>
#include "Ship.h"
#include "ShipManager.h"
#include "Field.h"
#include "Ability.h"
#include "AbilityManager.h"
#include "Exceptions.h"

class GameState{
public:
	string saving;
	ShipManager player_manager;
	ShipManager enemy_manager;
	Field player_field;
	Field enemy_field;
	AbilityManager* abilities;
	vector<vector <int>> unattacked_cells;
	string filename = "saving";
    
	GameState();
	void save(const string& filename);
	void load(const string& filename);
	friend ostream& operator<<(ostream& os, GameState& gameState);
	friend istream& operator>>(istream& is, GameState& gameState);
	bool EmptyChecking();
};

#endif
