#include "GameState.h"

GameState::GameState(): saving("empty"), player_field(2, 2), player_manager(0, {}), enemy_field(2, 2), enemy_manager(0, {}), unattacked_cells({}), abilities(new AbilityManager){}

void GameState::save(const string& filename) {
	ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << *this;
            outFile.close();
        } 
        else {
            cerr << "File not open!" << endl;
        }
}

void GameState::load(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            inFile >> *this;
            inFile.close();
        } else {
            cerr << "File not open!" << endl;
        }
}

ostream& operator<<(ostream& os, GameState& gameState) {
	os << "saving" << "\n";
	os << gameState.player_field.GetHeight() << " " << gameState.player_field.GetWidth() << "\n";
	os << gameState.player_manager.GetShipsNumber() << "\n";
	for (int i = 0; i < gameState.player_manager.GetShipsNumber(); i++){
		os << gameState.player_field.heads[i][0] << " " << gameState.player_field.heads[i][1] << " ";
		if ((*gameState.player_manager.GetShip(i)).GetOrientation() == Orientation::Vertical){os << "v ";}
		else {os << "h ";}
		os << (*gameState.player_manager.GetShip(i)).GetLenght() << " ";
		for (int j = 0; j < (*gameState.player_manager.GetShip(i)).GetLenght(); j++){
			os << (*gameState.player_manager.GetShip(i)).GetSegmentStatus(j) << " ";
		}
		os << "\n";
	}
	os << (*gameState.abilities).GetLength() << "\n";
	os << gameState.enemy_manager.GetShipsNumber() << "\n";
	for (int i = 0; i < gameState.enemy_manager.GetShipsNumber(); i++){
		os << gameState.enemy_field.heads[i][0] << " " << gameState.enemy_field.heads[i][1] << " ";
		if ((*gameState.enemy_manager.GetShip(i)).GetOrientation() == Orientation::Vertical){os << "v ";}
		else {os << "h ";}
		os << (*gameState.enemy_manager.GetShip(i)).GetLenght() << " ";
		for (int j = 0; j < (*gameState.enemy_manager.GetShip(i)).GetLenght(); j++){os << (*gameState.enemy_manager.GetShip(i)).GetSegmentStatus(j) << " ";}
		os << "\n";
	}
	for (int i = 0; i < gameState.enemy_field.GetHeight(); i++){
		for (int j = 0; j < gameState.enemy_field.GetWidth(); j++){
			if (gameState.enemy_field.GetCellStatus(j, i) == Status::Unknown){os << "? ";}  
			else {os << "+ ";}
		}
		os << "\n";
	} 
	for (int i = 0; i < gameState.unattacked_cells.size(); i++){
		for (int j = 0; j < gameState.unattacked_cells[0].size(); j++){os << gameState.unattacked_cells[i][j] << " ";}
	}
	return os;
}

istream& operator>>(istream& is, GameState& gameState) {
        is >> gameState.saving;
	int height;
	int width;
	is >> height >> width;
	gameState.player_field = Field(height, width, FieldType::Users); 
        gameState.enemy_field = Field(height, width, FieldType::Users);
       	
       	int player_ships_number;
	is >> player_ships_number;
	
	gameState.player_manager = ShipManager({0, {}});
	char orientation;
	int ship_len;
	vector<int> head(2);
	
	for (int i = 0; i < player_ships_number; i++){
		is >> head[0] >> head[1];
		is >> orientation;
		is >> ship_len;
		vector<int> ship_segments(ship_len);
		for (int j = 0; j < ship_len; j++){is >> ship_segments[j];}
		if (orientation == 'v'){gameState.player_manager.AddShip(ship_len, Orientation::Vertical, ship_segments);}     //сегменты копируются как то треш
		else{gameState.player_manager.AddShip(ship_len, Orientation::Horizontal, ship_segments);}
		gameState.player_field.PutShip(head[0], head[1], gameState.player_manager.GetShip(i));
	}
	
	int ability_quantity;
	is >> ability_quantity;
	gameState.abilities = new AbilityManager;
        if (ability_quantity < 3){
        	for (size_t i = 3; i > ability_quantity; i--){(*gameState.abilities).DeleteAbility();}
        }
        if (ability_quantity > 3){
        	for (int i = 3; i < ability_quantity; i++){(*gameState.abilities).AddAbility();}
        }

	int enemy_ships_number;
	is >> enemy_ships_number;
	
	gameState.enemy_manager = ShipManager({0, {}});
	
	for (int i = 0; i < enemy_ships_number; i++){
		is >> head[0] >> head[1];
		is >> orientation;
		is >> ship_len;
		vector<int> ship_segments(ship_len);
		for (int j = 0; j < ship_len; j++){is >> ship_segments[j];}
		if (orientation == 'v'){gameState.enemy_manager.AddShip(ship_len, Orientation::Vertical, ship_segments);}
		else{gameState.enemy_manager.AddShip(ship_len, Orientation::Horizontal, ship_segments);}
		gameState.enemy_field.PutShip(head[0], head[1], gameState.enemy_manager.GetShip(i));
	}
	
	for (int i = 0; i < gameState.enemy_field.GetHeight(); i++){
		for (int j = 0; j < gameState.enemy_field.GetWidth(); j++){
			char status;
			is >> status;
			if (status == '?'){gameState.enemy_field.ChangeStatusUnknown(j, i);}
		}
	} 
	gameState.enemy_field.ChangeFieldType();
	
	vector<vector<int>> new_unattacked_cells(gameState.enemy_field.GetHeight(), vector<int>(gameState.enemy_field.GetWidth()));
	for (int i = 0; i < gameState.enemy_field.GetHeight(); i++){
		for (int j = 0; j < gameState.enemy_field.GetWidth(); j++){is >> new_unattacked_cells[i][j];}
	}
	gameState.unattacked_cells = new_unattacked_cells;
	
	return is;
}

bool GameState::EmptyChecking(){
	if (saving == "empty"){
        	cout << "Your don't have any saves.\n";
        	return 0;
        }
        return 1;
}
