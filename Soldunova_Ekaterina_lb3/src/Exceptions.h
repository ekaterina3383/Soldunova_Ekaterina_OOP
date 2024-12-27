#ifndef EXCEPTION_H
#define SHIP_H

class NoAbilityException{
	string message;
public:
	NoAbilityException(string message = "You have run out of abilities, you can't use them."): message{message}{}
	string getMessage() const {return message;}
};

class IncorrectPlacementException{
	string message;
public:
	IncorrectPlacementException(string message = "You are trying to place the ship incorrectly, please enter other coordinates: "): message{message}{}
	string getMessage() const {return message;}
};

class OutsideAttackException{
	string message;
public:
	OutsideAttackException(string message = "You are trying to attack outside the field, please enter the correct coordinates: "): message{message}{}
	string getMessage() const {return message;}
};

class FieldSizeException{
	string message;
public:
	FieldSizeException(string message = "The entered field sizes are incorrect, please try again: "): message{message}{}
	string getMessage() const {return message;}
};

#endif
