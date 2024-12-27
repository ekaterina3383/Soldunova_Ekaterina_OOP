#include "ShipManager.h"
#include "Exceptions.h"
#include "Ship.h"

ShipManager::ShipManager(int user_ships_number, vector<int> user_ships_size, vector<int> segments){
	ships_number = user_ships_number;
	ships_size = user_ships_size;
	vector<Ship> new_ships(ships_number, {0}); 
	ships = new_ships;
	for (int i = 0; i < user_ships_number; ++i){
		Ship new_ship{ships_size[i]};
		ships[i] = new_ship;
	}
}
void ShipManager::AddShip(int ship_size, Orientation orientation, vector<int> segments){
	Ship new_ship{ship_size, orientation, segments};
	ships.push_back(new_ship);
	ships_number += 1;
	cout << "The new ship has been added successfully!" << endl;
}
void ShipManager::DeleteShip(int ship_index){
	ships.erase(ships.begin()+ship_index);
	ships_number -= 1;
}
Ship* ShipManager::GetShip(int index){
	return &ships[index];
}
int ShipManager::GetShipsNumber(){return ships_number;}
bool ShipManager::GetFleetState(){
	for (int i = 0; i < ships_number; ++i){
		if (ships[i].GetShipStatus() != ShipStatus::Destroyed){return 1;}
	}
	return 0;
}
