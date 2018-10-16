#ifndef _GAMEENGINE_H
#define _GAMEENGINE_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>

#include "utilities.hpp"
#include "house.hpp"
#include "feature.hpp"

class Feature; // Forward declaration
class Doorway; // Forward declaration
class Room; // Forward declaration
class House; // Forward declaration

#define numGameTasks 5

// TODO : Is this the right set of verbs
//save and load added to list
enum validVerbs { look, go, use, take, drop, open, close, hurl, hit, eat, inventory, LastAction, help, save, load, quit, tst, LastVerb};


/*
 * The GameState 
 *
 */
class GameState 
{
private:
	std::string Name;
	int Capacity; // How much the player can carry

public:
	std::vector<Feature*> Holding;  // Objects held by the player are Features
	bool GameTask[numGameTasks]; // GameTasks are defined in utilities.h
	House * housePtr;

	GameState(std::string Na);
	~GameState();
	void Print();
	Room * playerTurn (Room *);
	int getGameTaskStatus();
	int getAvailableCapacity(); // Capacity - FeaturesCarried = Available
	int getCapacity() { return Capacity;}
	void UpdateGameState(int &GameClock, Room* currentRoom);
	std::string printTheInstructions(); // Not implemented
};



struct Choice {
	Room  * nextRoom;
	Feature * useFeature;
	Feature * useOn;
	Doorway * goDoorway;

	validVerbs Verb;
	std::string Noun;
	std::string Subject;

	std::string printVerb();

	// Default constructor
	Choice() {
		nextRoom = NULL;
		useFeature = NULL;
		useOn = NULL;
		goDoorway = NULL;
		Verb = tst;
		Noun = "";
		Subject = "";
	};
};



#endif // _GameEngine_H
