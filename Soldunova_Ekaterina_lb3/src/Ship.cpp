#include "Ship.h"

Ship::Ship(int user_lenght, Orientation user_orientation, vector<int> segments){
	lenght = user_lenght;
	orientation = user_orientation;
	if (segments.size() == 0){
		vector<int> statuses(lenght, 2);
		segments_status = statuses;
	}
	else {segments_status = segments;}
}
void Ship::TurnShip(){
	if (orientation == Orientation::Vertical){orientation = Orientation::Horizontal;}
	if (orientation == Orientation::Horizontal){orientation = Orientation::Vertical;}
}
bool Ship::Hit(int index){
	if (segments_status[index] == 0){
		cout << "This segment of the ship has already been destroyed." << endl;
		return 0;
	}
	if (segments_status[index] == 1){
		segments_status[index] -= 1;
		cout << "A segment of the ship has been destroyed!" << endl;
	}
	if (segments_status[index] == 2){
		segments_status[index] -= 1;
		cout << "A segment of the ship has been damaged!" << endl;
	}
	return 1;
}
ShipStatus Ship::GetShipStatus(){
	vector<int> num_of_segments(3);
	for (int i = 0; i < lenght; i++){
		num_of_segments[segments_status[i]] += 1;
	}
	if ((num_of_segments[0]==lenght-1)&&(num_of_segments[1]==1)){return ShipStatus::LastDamagedSegment;}
	if (num_of_segments[0]==lenght){return ShipStatus::Destroyed;}
	if (num_of_segments[2]==lenght){return ShipStatus::Intact;}
	return ShipStatus::Damaged;
}
int Ship::GetLenght(){return lenght;}
Orientation Ship::GetOrientation(){return orientation;}
int Ship::GetSegmentStatus(int i){return segments_status[i];}
