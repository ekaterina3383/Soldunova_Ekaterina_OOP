#include "Ability.h"
#include "Exceptions.h"
#include <limits> 

bool DoubleDamage::ApplyingAbility(Field field, ShipManager manager){
	int x;
	int y;
	cout << "To use the double damage ability, enter the coordinates of the attacked cell:" << endl;
	while(!(cin >> x >> y)){
		cout << "The entered data is incorrect, please try again: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	bool x_right = (x>=0)&&(x+1<field.GetWidth());
	bool y_right = (y>=0)&&(y+1<field.GetHeight());
	while(!(x_right||y_right)){
		cout << "The entered coordinates go beyond the boundaries of the field, please try again: ";
		while(!(cin >> x >> y)){
			cout << "The entered data is incorrect, please try again: ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		x_right = (x>=0)&&(x+1<field.GetWidth());
		y_right = (y>=0)&&(y+1<field.GetHeight());
	}
	Ship* ship = field.GetShip(x, y);
	if (field.GetShip(x, y)){
		cout << "A ship was attacked! "; 
		if (field.GetCellStatus(x, y) == Status::Unknown){field.ChangeStatusShip(x, y);}
		bool add = (*ship).Hit(field.GetIndex(x, y));
		(*ship).Hit(field.GetIndex(x, y));
		if(add && (field.FieldGetShipStatus(x, y)==ShipStatus::Destroyed)) return 1;
	}
	else {
		cout << "This cell is empty." << endl;
		if (field.GetCellStatus(x, y) == Status::Unknown){field.ChangeStatusEmpty(x, y);}
	}
	field.Draw();
	return 0;
}

bool Scanner::ApplyingAbility(Field field, ShipManager manager){
	int x;
	int y;
	cout << "To use the scanner ability, enter the coordinates of the upper-left corner of the area you want to scan:" << endl;
	while(!(cin >> x >> y)){
		cout << "The entered data is incorrect, please try again: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	bool x_right = (x>=0)&&(x+1<field.GetWidth());
	bool y_right = (y>=0)&&(y+1<field.GetHeight());
	while(!(x_right||y_right)){
		cout << "The entered coordinates go beyond the boundaries of the field, please try again: ";
		while(!(cin >> x >> y)){
			cout << "The entered data is incorrect, please try again: ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		x_right = (x>=0)&&(x+1<field.GetWidth());
		y_right = (y>=0)&&(y+1<field.GetHeight());
	}
	for (int i = x; i<=x+1; i++){			
		for (int j = y; j<=y+1; j++){
			if (field.GetShip(i,j)){
				cout << "A ship has been found in the scanned area!";
				return 0;
			}
		}
	}
	cout << "There are no ships in the scanned area.";
	return 0;
}

bool Bombing::ApplyingAbility(Field field, ShipManager manager){
	for (int i = 0; i<manager.GetShipsNumber(); i++){
		if (((*manager.GetShip(i)).GetShipStatus() == ShipStatus::Intact)||((*manager.GetShip(i)).GetShipStatus() == ShipStatus::Damaged)){
			vector<int> undestroyed_segments;
			for (int j = 0; j<(*manager.GetShip(i)).GetLenght(); j++){
				if ((*manager.GetShip(i)).GetSegmentStatus(j) != 0){undestroyed_segments.push_back(j);}
			}
			int segment_index = rand() % undestroyed_segments.size();
			(*manager.GetShip(i)).Hit(segment_index);
			if((*manager.GetShip(i)).GetShipStatus()==ShipStatus::Destroyed) return 1;
			else return 0;
		}
	}
	for (int i = 0; i<manager.GetShipsNumber(); i++){
		if ((*manager.GetShip(i)).GetShipStatus() == ShipStatus::LastDamagedSegment){
			int index = 0;
			while((*manager.GetShip(i)).GetSegmentStatus(index) == 0){index++;}
			(*manager.GetShip(i)).Hit(index);
			return 1;
		}
	}
	return 0;
}
