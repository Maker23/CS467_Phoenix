#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "room.hpp"
#include "feature.hpp"
#include "engine.hpp"
#include "utilities.hpp"
#include "house.hpp"
#include "parser.hpp"

using namespace std;


/* ********************************************************* */
std::string Choice::printVerb()
{
	std::string verbPrint[] = {"look", "go", "use", "pick up or take", "drop", "open", "close", "throw", "hit", "eat", "examine"};

	return verbPrint[(int)Verb];
}

/* ********************************************************* */
GameState::GameState(std::string Na)
{

	housePtr = NULL;

	GameTask[0] = false;
	GameTask[1] = false;
	GameTask[2] = false;
	GameTask[3] = false;
	GameTask[4] = false;
}

GameState::~GameState() 
{
	// Should delete Contents and whatever else
}

/* ***********************************************************
 * GameState functions
 *
 *
 * GameState::Examine is different in that:
 *
 * 	 It returns the GameState in the vector (container examine does not return itself)
 * 	 It doesn't need to handle open tests or OpenFunc functions
 *
 * ********************************************************* */
void GameState::Print () 
{
	std::cout << Name << ". ";
}

int GameState::getGameTaskStatus()
{
	int points = 0;
	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::getGameTaskStatus" << std::endl;
	/*
	if (GameTask[0])
	{
		std::cout << "Game goal zero is met." << std::endl;
		points ++;
	}
	*/
	return points;
}

int GameState::getAvailableCapacity()
{
	int GameStateIsCarrying = 0;
	std::vector<Feature*> backpackContents;
	std::vector<Feature*>::iterator iterFeature;

	backpackContents = this->Holding;
	for (iterFeature=backpackContents.begin(); iterFeature != backpackContents.end(); iterFeature++)
	{
		GameStateIsCarrying += (*iterFeature)->getWeight();
	}
	return (this->Capacity - GameStateIsCarrying);
}
/* ***************************************************************
 * UpdateGameState - runs after every player turn
 *************************************************************** */
void GameState::UpdateGameState(int &GameClock, Room* currentRoom)
{
  int points = getGameTaskStatus();

	if (DEBUG_FUNCTION) std::cout << "===== begin Utilities::UpdateGameState" << std::endl;
	if(points) {} // TODO... silence compile-time warnings...
	GameClock++;
	// Can check on or update various GameTasks here
}

/*
 * TODO: Function info goes here
 */
Room * GameState::playerTurn(Room * currentRoom)
{
	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::playerTurn" << std::endl;
	// TODO: all the magic
	// PlayerState: What the player is holding and any state of the game
	// this:  The room we're in
	// Doorways : the array of doorways
	// Features : the features, a vector of Features
	//          : Features, with a map of verb->Action
	// And now we do stuff.
	Room * nextRoom = currentRoom;
	Choice * userChoice;
	Parser parse;

	userChoice = parse.ParseLine();

	// If verb = go, choice should be a door
	// otherwise look for noun in PlayerState and then in Room
	// If no noun - limited choices
	if (userChoice->Verb == (validVerbs)quit)
	{
		return (Room *) NULL;
	}
	if (userChoice->Verb == (validVerbs)help)
	{
		std::cout << "THERE IS NO HELP FOR YOU" << std::endl;
		return currentRoom;
	}
	if( (userChoice->Verb == (validVerbs)save) ||
			(userChoice->Verb == (validVerbs)load) )
	{
		std::cout << "Game does not yet support Save or Load" << std::endl;
		return currentRoom;
	}

	if ( userChoice->Noun == "" ) {
		if (userChoice->Verb == (validVerbs)look) 
		{
			currentRoom->Examine(); // Examine this room
		}
		else if (userChoice->Verb == (validVerbs)go) 
		{
			std::cout << "Where do you want to " << userChoice->printVerb()<< "?" << std::endl;
		}
		else if (userChoice->Verb < (validVerbs)LastAction)
		{
			std::cout << "What do you want to " << userChoice->printVerb()<< "?" << std::endl;
		}
	}
	else if (userChoice->Verb == (validVerbs)go) {
		nextRoom = currentRoom->goRoom(userChoice->Noun, this);
		if ( nextRoom != currentRoom )  // that's right, we're comparing pointers now
		{
			nextRoom->Examine();
		}
	}

	if (DEBUG_FUNCTION) std::cout << "===== end   GameState::playerTurn" << std::endl;

	return nextRoom;
}

