#include "Field.h"
#include "Exceptions.h"
#include <limits> 

Field::Field(int H, int W, FieldType type){
	if((H<2)||(W<2)){
		throw FieldSizeException{};
	}
	height = H;
	width = W;
	field_type = type;
	vector<vector<Cell>> new_user_cells(height, vector<Cell>(width, {Status::Empty, 0, 0}));
	vector<vector<Cell>> new_enemy_cells(height, vector<Cell>(width, {Status::Unknown, 0, 0}));
	if (field_type == FieldType::Users){cells = new_user_cells;}
	if (field_type == FieldType::Enemy){cells = new_enemy_cells;}
	vector<vector<int>> ships_heads(0, vector<int>(0));
	heads = ships_heads;
}
void Field::PutShip(int x, int y, Ship* ship){
	if ((x < 0)||(y < 0)){
		throw IncorrectPlacementException{"The coordinates cannot be negative, please try again: "};
	}
	if ((x > width)||(y > height)){
		throw IncorrectPlacementException{"The entered values go beyond the boundaries of the field."};
	}
	int start_x = x-1;
	int start_y = y-1;
	int end_x = x+1;
	int end_y = y+1;
	if ((*ship).GetOrientation() == Orientation::Vertical){end_y += (*ship).GetLenght()-1;}
	if ((*ship).GetOrientation() == Orientation::Horizontal){end_x += (*ship).GetLenght()-1;}
	if (start_x<0){start_x = 0;}
	if (start_y<0){start_y = 0;}
	if (end_x>width-1){end_x -= 1;}
	if (end_y>height-1){end_y -= 1;}
	if ((end_x > width-1)||(end_y > height-1)){throw IncorrectPlacementException{};}
	for (int i = start_y; i<end_y+1; ++i){
		for(int j = start_x; j<end_x+1; ++j){
			if (cells[i][j].ship){throw IncorrectPlacementException{};}
		}
	}
	if ((*ship).GetOrientation() == Orientation::Vertical){
		for (int i = 0; i < (*ship).GetLenght(); ++i){
			cells[y+i][x].index_segment = i;
			if (field_type == FieldType::Users){cells[y+i][x].status = Status::Ship;}
			cells[y+i][x].ship = ship;
		}
	}
	if ((*ship).GetOrientation() == Orientation::Horizontal){
		for (int i = 0; i < (*ship).GetLenght(); ++i){
			cells[y][x+i].index_segment = i;
			if (field_type == FieldType::Users){cells[y][x+i].status = Status::Ship;}
			cells[y][x+i].ship = ship;
		}
	}
	if (field_type == FieldType::Users){cout << "The ship is installed on the field successfully!" << endl;}
	heads.push_back({x, y});
}
bool Field::Attack(int x, int y){
	if ((x<0)||(y<0)||(x>=width)||(y>=height)){
		throw OutsideAttackException{};
	}
	if (cells[y][x].status == Status::Ship){
		cout << "A ship was attacked! " ; 
		cells[y][x].status = Status::Ship;
		return (*cells[y][x].ship).Hit(cells[y][x].index_segment);
	}
	cout << "This cell is empty." << endl;
	cells[y][x].status = Status::Empty;
	return 0;
}
int Field::GetHeight(){return height;}
int Field::GetWidth(){return width;}
Ship* Field::GetShip(int x, int y){return cells[y][x].ship;}
int Field::GetIndex(int x, int y){return cells[y][x].index_segment;}
Status Field::GetCellStatus(int x, int y){return cells[y][x].status;}
void Field::ChangeStatusShip(int x, int y){cells[y][x].status = Status::Ship;}
void Field::ChangeStatusEmpty(int x, int y){cells[y][x].status = Status::Empty;}
void Field::ChangeStatusUnknown(int x, int y){cells[y][x].status = Status::Unknown;}
void Field::ChangeFieldType(){
	if (field_type == FieldType::Enemy){field_type = FieldType::Users;}
	if (field_type == FieldType::Users){field_type = FieldType::Enemy;}	
}
ShipStatus Field::FieldGetShipStatus(int x, int y){
	if (cells[y][x].ship){return (*cells[y][x].ship).GetShipStatus();}
	return ShipStatus::Intact;
}
void Field::Draw(){
	cout << "    ";
	for (int i = 0; i < width; i++) {cout << " " << i << " ";}
	cout << endl; 
	for (int i = 0; i < width; i++) {cout << "----";}
	cout << endl; 
	for (int i = 0; i < height; i++){
		cout << " " << i << " |";
		for(int j = 0; j < width; j++){
			if (cells[i][j].status == Status::Ship){cout << " " << (*cells[i][j].ship).GetSegmentStatus(cells[i][j].index_segment) << " ";}
			if (cells[i][j].status == Status::Empty){cout << " - ";}
			if (cells[i][j].status == Status::Unknown){cout << " ? ";}
		}
		cout << endl; 
	}
}
	
Field::Field(Field& field): height(field.height), width(field.width), field_type(field.field_type), cells(field.cells){}
Field& Field::operator = (Field& field){
	Field temp(field);
	swap(height, temp.height);
	swap(width, temp.width);
	swap(field_type, temp.field_type);
	swap(cells, temp.cells);
	return *this;
}
	
Field::Field(Field&& field): height(0), width(0), field_type(), cells(0){}
Field& Field::operator = (Field&& field){
	if(this != &field){
		swap(height, field.height);
		swap(width, field.width);
		swap(field_type, field.field_type);
		swap(cells, field.cells);
	}
	return *this;
}
