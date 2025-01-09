#ifndef GAME_H
#define GAME_H

#include "GameState.h"

enum class RoundOutcome {Lose, Win, Exit};

class Game{
public:
	GameState game_state;
	Game(GameState game_state);
	void EnemyInitialization();
	RoundOutcome Round();
	void EnemyMove();
	void PlayerMove();
	void StartGame();
	vector<int> InputInts(int num);
	bool StartCheck();
};
#endif
