#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <iostream>
#include <string>
#include <limits> 
#include <queue>
#include <memory>
using namespace std;

enum class Orientation {Horizontal, Vertical};
enum class Status {Ship, Empty, Unknown};
enum class ShipStatus {Destroyed, Intact, Damaged, LastDamagedSegment};

class Ship{
	int lenght;
	Orientation orientation;
	vector<int> segments_status;
public:
	Ship(int user_lenght, Orientation user_orientation = Orientation::Vertical, vector<int> segments = {});
	void TurnShip();
	bool Hit(int index);
	ShipStatus GetShipStatus();
	int GetLenght();
	Orientation GetOrientation();
	int GetShipLenght(){return lenght;}
	int GetSegmentStatus(int i);
};
#endif
