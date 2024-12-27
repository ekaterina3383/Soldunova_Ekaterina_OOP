#ifndef FIELD_H
#define FIELD_H

#include "Ship.h"
#include "ShipManager.h"
enum class FieldType {Users, Enemy};

class Field{
	class Cell{
	public:
		Status status;
		Ship* ship;
		int index_segment;
	};
	int height;
	int width;
	FieldType field_type;
	vector<vector<Cell>> cells;
public:
	vector<vector<int>> heads;
	vector<vector<int>> player_heads;
	Field(int H, int W, FieldType type = FieldType::Users);
	void PutShip(int x, int y, Ship* ship);
	bool Attack(int x, int y);
	int GetHeight();
	int GetWidth();
	Ship* GetShip(int x, int y);
	int GetIndex(int x, int y);
	Status GetCellStatus(int x, int y);
	void ChangeStatusShip(int x, int y);
	void ChangeStatusEmpty(int x, int y);
	void ChangeStatusUnknown(int x, int y);
	void ChangeFieldType();
	ShipStatus FieldGetShipStatus(int x, int y);
	void Draw();
	
	Field(Field& field);
	Field& operator = (Field& field);
	
	Field(Field&& field);
	Field& operator = (Field&& field);
};
#endif
