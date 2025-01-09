#include "Game.h"

Game::Game(GameState game):game_state(game){
	if (StartCheck()){
		cout << "Hello! Before starting the new game, set the height and width of the fields:" << endl;
		vector<int> sizes = InputInts(2);
		while (true){
			try{
				Field new_field(sizes[0], sizes[1]);
				break;
			}
			catch (const FieldSizeException& ex){
				cout << ex.getMessage() << endl;
				sizes = InputInts(2);
			}
		}
	
		game_state.player_field = Field(sizes[0], sizes[1], FieldType::Users);
		cout << "The creation of the field was successful!" << endl;
		
		cout << "Enter the number of ships:" << endl;
		int quantity = InputInts(1)[0];
		while((quantity<1)||(quantity>(game_state.player_field.GetWidth()*game_state.player_field.GetHeight()/2))){
			cout << "The quantity of the ships cannot be negative or zero, please try again: ";
			quantity = InputInts(1)[0];
		}
		game_state.player_field.Draw();
		
		cout << "Enter sizes of ships. After each size, determine the position of the ship: enter v if you want the ship to be positioned vertically or h if horizontally" << endl;
		vector<int> ships(quantity);
		vector<char> orient(quantity);
		int i = 0; 
		game_state.player_manager = ShipManager(0, {});
	
		while (i < quantity){
			ships[i] = InputInts(1)[0];
			if (ships[i] < 1){cout << "The length of the ship cannot be negative or zero, please try again: " << endl;}
			else {
				while(!(cin >> orient[i])){
    					cout << "The entered data is incorrect, please try again: ";
    					cin.clear();
    					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				if (orient[i]=='v'){game_state.player_manager.AddShip(ships[i]);}
				if (orient[i]=='h'){game_state.player_manager.AddShip(ships[i], Orientation::Horizontal);}
				if ((orient[i]!='v')&&(orient[i]!='h')){
					cout << "You entered an incorrect orientation symbol, so the ship is set vertically by default." << endl;
					game_state.player_manager.AddShip(ships[i]);
				}
				i++;
			}
		}
	
		cout << "For each of the created ships, set the coordinates of its heads:" << endl;
		i = 0; 
		int flag = 0;
		vector<int> coordinates;
		while (i < quantity){
			coordinates = InputInts(2);
			try{
				game_state.player_field.PutShip(coordinates[0], coordinates[1], game_state.player_manager.GetShip(i));
				i++;
			}
			catch (const IncorrectPlacementException& ex){
				cout << ex.getMessage() << endl;
				flag++;
				if (flag == 3){
					game_state.player_manager.DeleteShip(i);
					cout << "You have placed the ship incorrectly three times. Perhaps it was too big, so it was deleted. You can continue to place the remaining ships." << endl;
					flag = 0;
					i++;
				}
			}
		}
		game_state.player_field.Draw();
		game_state.abilities = new AbilityManager;
		this->EnemyInitialization();
	}
}

void Game::StartGame(){
	cout << "The new game has started!" << endl;
	cout << "Before each round, you will be asked to enter a command:" << endl;
	cout << "-if you want to end the game with saving, enter 'save_exit';" << endl;
	cout << "-if you want to exit without saving, enter 'exit';" << endl;
	cout << "-if you want to save the game and continue, enter 'save';" << endl;
	cout << "-if you want to load a previous save, enter 'load';" << endl;
	cout << "-if you want to continue the game, enter any line." << endl;
	RoundOutcome outcome;
	outcome = this->Round();
	while (outcome != RoundOutcome::Exit){
		if (outcome == RoundOutcome::Win){
			this->EnemyInitialization();
			outcome = this->Round();
		} else if (outcome == RoundOutcome::Lose){
			GameState new_state;
			Game new_game(new_state);
			new_game.StartGame();
		}
	}
}

void Game::EnemyInitialization(){
	Field new_field(game_state.player_field.GetHeight(), game_state.player_field.GetWidth(), FieldType::Enemy);
	game_state.enemy_field = new_field;
	game_state.enemy_field.Draw();
	vector<int> enemy_ships_size(game_state.player_manager.GetShipsNumber());
	for (int i = 0; i < game_state.player_manager.GetShipsNumber(); i++){enemy_ships_size[i] = (*game_state.player_manager.GetShip(i)).GetLenght();}
	ShipManager new_ships = ShipManager(game_state.player_manager.GetShipsNumber(), enemy_ships_size);
	game_state.enemy_manager = new_ships;
	int x;
	int y;
	int i = 0; 
	int flag = 0;
	while (i < game_state.enemy_manager.GetShipsNumber()){
		x = rand() % game_state.enemy_field.GetHeight();
		y = rand() % game_state.enemy_field.GetWidth(); 
		try{
			game_state.enemy_field.PutShip(x, y, game_state.enemy_manager.GetShip(i));
			i++;
			flag = 0;
		}
		catch (const IncorrectPlacementException& ex){
			flag++;
			if (flag == 10){
				game_state.enemy_manager.DeleteShip(i);
				flag = 0;
				i++;
			}
		}
	}
	vector<vector<int>> cells(game_state.player_field.GetHeight(), vector<int>(game_state.player_field.GetWidth(), 1));
	game_state.unattacked_cells = cells;
}

RoundOutcome Game::Round(){
	while (true){
		PlayerMove();
		game_state.enemy_field.Draw();
		string command;
		cout << "Enter a command: " << endl;
		cin >> command;
		if (command == "save_exit"){
			game_state.save("saving");
			return RoundOutcome::Exit;
		} 
		if (command == "exit"){return RoundOutcome::Exit;}
		if (command == "save"){game_state.save("saving");}
		if (command == "load"){game_state.load("saving");} 
		if (!game_state.enemy_manager.GetFleetState()) {return RoundOutcome::Win;}
		EnemyMove();
		game_state.player_field.Draw();
		if (!game_state.player_manager.GetFleetState()) {return RoundOutcome::Lose;}
	}
}

void Game::PlayerMove(){
	string ability;
	cout << "Do you want to apply the ability? Yes - enter yes, No - enter any line: " << endl;
	cin >> ability;
	if (ability == "yes"){
		try{
			if ((*game_state.abilities).ApplicationOfAbilities(game_state.enemy_field, game_state.enemy_manager)) (*game_state.abilities).AddAbility();
		}
		catch (const NoAbilityException& ex){cout << ex.getMessage() << endl;}
	}
	cout << "Enter the coordinates of the cell you want to attack:" << endl;
	vector<int> coordinates = InputInts(2);
	int x = coordinates[0];
	int y = coordinates[1];
	try{
		if(game_state.enemy_field.Attack(x, y)){
			if(game_state.enemy_field.FieldGetShipStatus(x, y)==ShipStatus::Destroyed){(*game_state.abilities).AddAbility();}
		}
	}
	catch (const OutsideAttackException& ex){cout << ex.getMessage() << endl;}
}

void Game::EnemyMove(){
	int attack_y = rand() % game_state.player_field.GetHeight();
	int attack_x = rand() % game_state.player_field.GetWidth();
	while (game_state.unattacked_cells[attack_y][attack_x] != 1){
		attack_y = rand() % game_state.player_field.GetHeight();
		attack_x = rand() % game_state.player_field.GetWidth();
	}
	if (game_state.player_field.Attack(attack_x, attack_y)){
		if (game_state.player_field.FieldGetShipStatus(attack_x, attack_y) == ShipStatus::Destroyed){game_state.unattacked_cells[attack_y][attack_x] = 0;}
	}
	if (game_state.player_field.GetCellStatus(attack_x, attack_y) == Status::Empty){game_state.unattacked_cells[attack_y][attack_x] = 0;}
}

vector<int> Game::InputInts(int num){
	vector<int> values(num);
	if (num == 2){
		while(!(cin >> values[0] >> values[1])){
				cout << "The entered data is incorrect, please try again: ";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	if (num == 1){
		while(!(cin >> values[0])){
				cout << "The entered data is incorrect, please try again: ";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return values;
}

bool Game::StartCheck(){
	string loading;
	cout << "Do you want to load the game? Yes - enter yes, No - enter any line: " << endl;
	cin >> loading;
	if (loading == "yes"){
		game_state.load("saving");
		if (game_state.EmptyChecking()){
			cout << "Your game is succesfully load!" << endl;
			return 0;
		}
	}
	return 1;
}
