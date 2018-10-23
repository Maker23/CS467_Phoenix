/*
 * File name:  engine.cpp
 *
 * Overview:
 *   Functions and variables that control the game state
 *
 * GameState::playerTurn    The central command loop of the game.
 *                          Run it indefinitely until quit
 *
 * Choice::printVerb        Print a human-readable string for a given
 *                          verb enum.
 *
 *
 */
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


/* ***********************************************************
 *
 *  playerTurn      
 *
 *  The central command loop of the game. Takes one Room* as an
 *  argument, which is the room the player is currently in at the
 *  beginning of the turn. Returns a pointer to the room (same or new) 
 *  that the player is in at the end of the turn.
 *
 * ********************************************************* */
Room * GameState::playerTurn(Room * currentRoom)
{
	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::playerTurn" << std::endl;
	// TODO: all the magic
	// GameState: What the player is holding and any state of the game
	// currentRoom:  The room we're in
	// Doorways : the array of doorways in currentRoom
	// Features : the features in currentRoom, a vector of Features
	//          : Features, with a map of verb->Action
	// And now we do stuff.
	Room * nextRoom = currentRoom;
	Choice * userChoice;
	Parser parse;

	userChoice = parse.ParseLine();

	// Look for Reserved words - Help, Quit, Load, Save
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

	// If not a reserved word, take action
	nextRoom = actInRoom(currentRoom, userChoice);

	if (DEBUG_FUNCTION) std::cout << "===== end   GameState::playerTurn" << std::endl;

	return nextRoom;
}

Room * GameState::actInRoom(Room * currentRoom, Choice * userChoice)
{
	Room * nextRoom = currentRoom;

	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::actInRoom" << std::endl;
	// If no noun - limited choices
	if ( userChoice->Noun == "" ) {
		if (userChoice->Verb == (validVerbs)look) 
		{
			currentRoom->Examine(this); // Examine this room
		}
		else if (userChoice->Verb == (validVerbs)go) 
		{
			std::cout << "Where do you want to " << userChoice->printVerb()<< "?" << std::endl;
		}
		else if (userChoice->Verb < (validVerbs)LastAction)
		{
			std::cout << "What do you want to " << userChoice->printVerb()<< "?" << std::endl;
		}
		else 
		{
			// Unknown  ;)
			std::cout << "I didn't catch that - try again." << std::endl;
		}
	}
	//
	// If noun - go or act
	//
	else if (userChoice->Verb == (validVerbs)go) {
		nextRoom = currentRoom->goRoom(userChoice->Noun, this);
		if ( nextRoom != currentRoom )  // that's right, we're comparing pointers now
		{
			nextRoom->Examine(this);
		}
	}
	else if (userChoice->Verb < (validVerbs)LastAction)
	{
		// Pass the verb and noun(s) to ActInRoom
		nextRoom = actOnFeature(currentRoom, userChoice);
	}
	else 
	{
		// Unknown  ;)
		std::cout << "I have no idea what you just said." << std::endl;
	}

	if (DEBUG_FUNCTION) std::cout << "===== end   GameState::actInRoom" << std::endl;
	return nextRoom;

}

/* ***********************************************************
 *
 * Wow, well, this is what I got right now.
 *
 * ********************************************************* */
Room * GameState::actOnFeature(Room * currentRoom, Choice * userChoice)
{
	Room * nextRoom = currentRoom;
	Feature * theNoun = NULL;
	Feature * theSubject = NULL;
	std::vector<Feature *>::iterator iter;
	std::vector<std::string>::iterator iterStr;
	bool inHand = false;
	bool inRoom = false;

	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::actOnFeature,"
		<< "verb = " << userChoice->printVerb()<< "(" << userChoice->Verb  << ")"
		<<", Noun = " << userChoice->Noun 
		<< ", Subject = " << userChoice->Subject << std::endl;

	theNoun = housePtr->getFeaturePtr(userChoice->Noun);
	theSubject = housePtr->getFeaturePtr(userChoice->Subject);
  if ( ! theNoun ) 
	{
		std::cout << "There doesn't seem to be a " << userChoice->Noun << " anywhere nearby" << std::endl;
		return nextRoom;
	}
  if ( (userChoice->Subject.compare("") != 0 )&& ! theSubject ) 
	{
		// TODO:  Look for Room names as well as feature names
		std::cout << "Didn't find subject '" << userChoice->Subject << "' in the house" << std::endl;
		return nextRoom;
	}
	if ( ! theNoun && ! theSubject )  return nextRoom;


  // Are we carrying the Noun ?
	
	for ( iter = Holding.begin(); iter != Holding.end(); iter++) {
		if ( theNoun == *iter ) {
			inHand = true;
			break;
		}
	}
	// Is the Noun in the room ?
	if (! inHand ) {
		for ( iterStr = currentRoom->roomFeatures.begin(); iterStr != currentRoom->roomFeatures.end(); iterStr++) 
		{
			if ( userChoice->Noun.compare(*iterStr) == 0 ) 
			{
				inRoom = true;
				break;
			}
			// TODO: Pick the thing up? Maybe not - depends on the verb
		}
	}
	if ( ! inHand && ! inRoom )  
	{
		// Otherwise - error
		std::cout << "There doesn't seem to be a " << userChoice->Noun << " at hand." << std::endl;
		return nextRoom;
	}
	

  switch (userChoice->Verb)
	{
		case look:
			if (DEBUG_FUNCTION) std::cout << "      matched look" << std::endl;
			theNoun->Examine(this);
			break;
		case hurl:
			if (DEBUG_FUNCTION) std::cout << "      matched throw " << std::endl;
			theNoun->hurlFeature(this, theSubject);
			break;
		default:
			if (DEBUG_FUNCTION) std::cout << "      ERROR: fell through to default " << std::endl;
	}

	if (DEBUG_FUNCTION) std::cout << "===== end   GameState::actOnFeature" << std::endl;
	return nextRoom;
}

/* ***********************************************************
 * Constructor and destructor
 * ********************************************************* */
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
		//GameStateIsCarrying += (*iterFeature)->getWeight();
	}
	return (this->Capacity - GameStateIsCarrying);
}
/* ************************************************************
 *
 * UpdateGameState - runs after every player turn. 
 * TODO: move this into the playerTurn now that we have shared state
 *
 ************************************************************ */
void GameState::UpdateGameState(int &GameClock, Room* currentRoom)
{
  int points = getGameTaskStatus();

	if (DEBUG_FUNCTION) std::cout << "===== begin Utilities::UpdateGameState" << std::endl;
	if(points) {} // TODO... silence compile-time warnings...
	GameClock++;
	// Can check on or update various GameTasks here
}
