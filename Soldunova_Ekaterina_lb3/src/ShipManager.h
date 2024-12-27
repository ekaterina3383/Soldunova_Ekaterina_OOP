#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include "Ship.h"
class ShipManager{
	int ships_number;
	vector<int> ships_size;
	vector<Ship> ships;
public:
	ShipManager(int user_ships_number, vector<int> user_ships_size, vector<int> segments = {});
	void AddShip(int ship_size, Orientation orientation = Orientation::Vertical, vector<int> segments = {});
	void DeleteShip(int ship_index);
	Ship* GetShip(int index);
	int GetShipsNumber();
	bool GetFleetState();
};
#endif
