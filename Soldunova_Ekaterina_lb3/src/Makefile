all: main clean
main: main.o Ship.o ShipManager.o Field.o Ability.o AbilityManager.o Game.o GameState.o
	g++ main.o Ship.o ShipManager.o Field.o Ability.o AbilityManager.o Game.o GameState.o -o lb3
main.o: main.cpp Ability.h AbilityManager.h Field.h Ship.h ShipManager.h Game.h
	g++ -c main.cpp
Ability.o: Ability.cpp Ability.h Field.h
	g++ -c Ability.cpp
AbilityManager.o: AbilityManager.cpp AbilityManager.h
	g++ -c AbilityManager.cpp
Field.o: Field.cpp Field.h Ship.h ShipManager.h
	g++ -c Field.cpp
Ship.o: Ship.cpp Ship.h
	g++ -c Ship.cpp
ShipManager.o: ShipManager.cpp ShipManager.h
	g++ -c ShipManager.cpp
Game.o: Game.cpp Game.h GameState.h
	g++ -c Game.cpp
GameState.o: GameState.cpp GameState.h Ability.h AbilityManager.h Field.h Ship.h ShipManager.h Exceptions.h 
	g++ -c GameState.cpp
clean:
	rm -rf *.o
