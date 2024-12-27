#include "Game.h"

Game::Game():player_field(2, 2), player_ships(0, {}), enemy_field(2, 2), enemy_ships(0, {}), unattacked_cells({}){
	string loading;
	cout << "Do you want to load the game? Yes - enter yes, No - enter any line: " << endl;
	cin >> loading;
	if (loading == "yes"){
		GameState game_state(this);
		game_state.load("saving");
		cout << "Your game is succesfully load!" << endl;
	}
	
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
	
	player_field = Field(sizes[0], sizes[1], FieldType::Users);
	cout << "The creation of the field was successful!" << endl;
	
	cout << "Enter the number of ships:" << endl;
	int quantity = InputInts(1)[0];
	while((quantity<1)||(quantity>(player_field.GetWidth()*player_field.GetHeight()/2))){
		cout << "The quantity of the ships cannot be negative or zero, please try again: ";
		quantity = InputInts(1)[0];
	}
	player_field.Draw();
	
	cout << "Enter sizes of ships. After each size, determine the position of the ship: enter v if you want the ship to be positioned vertically or h if horizontally" << endl;
	vector<int> ships(quantity);
	vector<char> orient(quantity);
	int i = 0; 
	player_ships = ShipManager(0, {});
	
	while (i < quantity){
		ships[i] = InputInts(1)[0];
		if (ships[i] < 1){cout << "The length of the ship cannot be negative or zero, please try again: " << endl;}
		else {
			while(!(cin >> orient[i])){
    				cout << "The entered data is incorrect, please try again: ";
    				cin.clear();
    				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			if (orient[i]=='v'){player_ships.AddShip(ships[i]);}
			if (orient[i]=='h'){player_ships.AddShip(ships[i], Orientation::Horizontal);}
			if ((orient[i]!='v')&&(orient[i]!='h')){
				cout << "You entered an incorrect orientation symbol, so the ship is set vertically by default." << endl;
				player_ships.AddShip(ships[i]);
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
			player_field.PutShip(coordinates[0], coordinates[1], player_ships.GetShip(i));
			i++;
		}
		catch (const IncorrectPlacementException& ex){
			cout << ex.getMessage() << endl;
			flag++;
			if (flag == 3){
				player_ships.DeleteShip(i);
				cout << "You have placed the ship incorrectly three times. Perhaps it was too big, so it was deleted. You can continue to place the remaining ships." << endl;
				flag = 0;
				i++;
			}
		}
	}
	player_field.Draw();
	abilities = AbilityManager();
}

void Game::StartGame(){
	cout << "The new game has started!" << endl;
	while (this->Round() != RoundOutcome::Lose){
		this->Round();
	}
	//Game new_game;
	//new_game.StartGame();
}

RoundOutcome Game::Round(){
	Field new_field(player_field.GetHeight(), player_field.GetWidth(), FieldType::Enemy);
	enemy_field = new_field;
	enemy_field.Draw();
	vector<int> enemy_ships_size(player_ships.GetShipsNumber());
	for (int i = 0; i < player_ships.GetShipsNumber(); i++){enemy_ships_size[i] = (*player_ships.GetShip(i)).GetLenght();}
	ShipManager new_ships = ShipManager(player_ships.GetShipsNumber(), enemy_ships_size);
	enemy_ships = new_ships;
	int x;
	int y;
	int i = 0; 
	int flag = 0;
	while (i < enemy_ships.GetShipsNumber()){
		x = rand() % enemy_field.GetHeight();
		y = rand() % enemy_field.GetWidth(); 
		try{
			enemy_field.PutShip(x, y, enemy_ships.GetShip(i));
			i++;
			flag = 0;
		}
		catch (const IncorrectPlacementException& ex){
			flag++;
			if (flag == 3){
				enemy_ships.DeleteShip(i);
				flag = 0;
				i++;
			}
		}
	}
	vector<vector<int>> cells(player_field.GetHeight(), vector<int>(player_field.GetWidth(), 1));
	unattacked_cells = cells;
	while (true){
		PlayerMove();
		enemy_field.Draw();
		string saving;
		cout << "Do you want to save the game? Yes - enter yes, No - enter any line: " << endl;
		cin >> saving;
		if (saving == "yes"){
			GameState game_state(this);
			game_state.save("saving");
		}
		if (!enemy_ships.GetFleetState()) {return RoundOutcome::Win;}
		EnemyMove();
		player_field.Draw();
		if (!player_ships.GetFleetState()) {return RoundOutcome::Lose;}
	}
	return RoundOutcome::Unfinished;
}

void Game::PlayerMove(){
	string ability;
	cout << "Do you want to apply the ability? Yes - enter yes, No - enter any line: " << endl;
	cin >> ability;
	if (ability == "yes"){
		try{
			if (abilities.ApplicationOfAbilities(enemy_field, enemy_ships)) abilities.AddAbility();
		}
		catch (const NoAbilityException& ex){cout << ex.getMessage() << endl;}
	}
	cout << "Enter the coordinates of the cell you want to attack:" << endl;
	vector<int> coordinates = InputInts(2);
	int x = coordinates[0];
	int y = coordinates[1];
	try{
		if(enemy_field.Attack(x, y)){
			if(enemy_field.FieldGetShipStatus(x, y)==ShipStatus::Destroyed) abilities.AddAbility();
		}
	}
	catch (const OutsideAttackException& ex){cout << ex.getMessage() << endl;}
}

void Game::EnemyMove(){
	int attack_y = rand() % player_field.GetHeight();
	int attack_x = rand() % player_field.GetWidth();
	while (unattacked_cells[attack_y][attack_x] != 1){
		attack_y = rand() % player_field.GetHeight();
		attack_x = rand() % player_field.GetWidth();
	}
	if (player_field.Attack(attack_x, attack_y)){
		if (player_field.FieldGetShipStatus(attack_x, attack_y) == ShipStatus::Destroyed){unattacked_cells[attack_y][attack_x] = 0;}
	}
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
