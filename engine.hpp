#ifndef _GAMEENGINE_H
#define _GAMEENGINE_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>

enum validVerbs { look, go, use, take, drop, open, close, hurl, hit, unlock, inventory, LastAction, help, save, load, quit, unknown, LastVerb};

#include "utilities.hpp"
#include "house.hpp"
#include "feature.hpp"


class Feature; // Forward declaration
class Doorway; // Forward declaration
class Room; // Forward declaration
class House; // Forward declaration
struct Choice; // Forward declaration

#define numGameTasks 5

// TODO : Is this the right set of verbs
//save and load added to list
//                0     1   2    3     4     5     6      7     8    9    10         11          12    13    14    15    16       17

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
	std::vector<Feature *> Holding;  // Features by name
	bool GameTask[numGameTasks]; // GameTasks are defined in utilities.h
	House * housePtr;

	GameState(std::string Na);
	~GameState();
	void Print();
	Room * playerTurn (Room *);
	Room * actInRoom (Room *, Choice *);
	Room * actOnFeature (Room *, Choice *);
	bool featureInHand (Feature *);
	bool featureInRoom (Room *, std::string);
	bool featureWithinReach (Room *, std::string);
	int getGameTaskStatus();
	int getAvailableCapacity(); // Capacity - FeaturesCarried = Available
	int getCapacity() { return Capacity;}
	void UpdateGameState(int &GameClock, Room* currentRoom);
	void getOverrideVerb(Choice *);
	std::string printTheInstructions(); // Not implemented
};



struct Choice {

	validVerbs Verb;
	std::string inputVerb; // What the user typed; use this for overrides
	std::string inputNoun; // What the user typed; use this for printing
	std::string Noun;
	std::string Subject;

	std::string printVerb(); // converts validVerbs enum into a string

	// Default constructor
	Choice() {
		Verb = unknown;
		inputVerb = "";
		inputNoun = "object";
		Noun = "";
		Subject = "";
	};
};



#endif // _GameEngine_H
