#ifndef GAME_H
#define GAME_H

#include "Ship.h"
#include "ShipManager.h"
#include "Field.h"
#include "Ability.h"
#include "AbilityManager.h"
#include "Exceptions.h"
#include "GameState.h"

enum class RoundOutcome {Lose, Win, Unfinished};

class Game{
public:
	Field player_field; 
	ShipManager player_ships;
	AbilityManager abilities;
	Field enemy_field;
	ShipManager enemy_ships;
	vector<vector<int>> unattacked_cells;
	Game();
	RoundOutcome Round();
	void EnemyMove();
	void PlayerMove();
	void StartGame();
	vector<int> InputInts(int num);
};
#endif
