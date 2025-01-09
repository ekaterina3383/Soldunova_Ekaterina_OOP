#include <iostream>
#include <string>
#include <vector>
#include <limits> 
#include <queue>
#include <memory>
using namespace std;
#include "Game.h"
#include "GameState.h"

int main(){
	GameState game_state;
	Game game(game_state);
	game.StartGame();
}
