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
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
//#include <map>
//#include <stack>
//#include <vector>
//#include <typeindex>
//#include <sstream>
//#include <algorithm>

#include "room.hpp"
#include "feature.hpp"
#include "engine.hpp"
#include "utilities.hpp"
#include "house.hpp"
#include "parser.hpp"
#include "puzzle.hpp"

using namespace std;


/* ********************************************************* */
std::string Choice::printVerb()
{

	std::string printVerb(); // converts validVerbs enum into a string
	std::string verbPrint[] = {"look", "go", "use", "pick up or take", "drop", "open", "close", "throw", "hit", "unlock", "inventory"};

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

	Room * nextRoom = currentRoom;
	Choice * userChoice;
	Parser parse (this);

	if ( GameTest) {
		userChoice = parse.TestLine();
	}
	else {
		userChoice = parse.ParseLine();
	}

	// Look for Reserved words - Help, Quit, Load, Save
	if (userChoice->Verb == (validVerbs)quit)
	{
		return (Room *) NULL;
	}
	if (userChoice->Verb == (validVerbs)help)
	{
		printHelp(true);
		//std::cout << "THERE IS NO HELP FOR YOU" << std::endl;
		return currentRoom;
	}
	if (userChoice->Verb == (validVerbs)save)
	{
		saveGame(currentRoom);
		return currentRoom;

	}
	if (userChoice->Verb == (validVerbs)load) 
	{
		loadGame();
		return currentRoom;
	}

	getOverrideVerb(userChoice); 
	nextRoom = actInRoom(currentRoom, userChoice);

	if (DEBUG_FUNCTION) std::cout << "===== end   GameState::playerTurn" << std::endl;

	return nextRoom;
}

void GameState::getOverrideVerb(Choice * userChoice)
{
	Feature * thisFeature;

	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::getOverrideVerb with noun " << userChoice->Noun << std::endl;
	if ( housePtr->hasFeature(strToLowercase(userChoice->Noun)) )
	{
		thisFeature = housePtr->getFeaturePtr(userChoice->Noun);
		if (DEBUG_FUNCTION) {
			std::cout << "      found feature "<< userChoice->Noun << ", looking for " << userChoice->inputVerb << std::endl;

			for ( auto iter=thisFeature->actions.begin(); iter != thisFeature->actions.end(); iter++)
			{
				std::cout << iter->first << ":" << iter->second << std::endl;
			}
		}
		if ( thisFeature->actions.find(strToLowercase(userChoice->inputVerb)) != thisFeature->actions.end() )
		{
			if (DEBUG_FUNCTION) std::cout << "      found alternate verb " << userChoice->inputVerb << std::endl;
			userChoice->Verb = thisFeature->actions.find(strToLowercase(userChoice->inputVerb))->second;
		}
	}
}

Room * GameState::actInRoom(Room * currentRoom, Choice * userChoice)
{
	Room * nextRoom = currentRoom;
	std::string nextroomKeyName = currentRoom->getExitRoomByKey(userChoice->Noun, false);

	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::actInRoom" << std::endl;
	if (DEBUG_FEATURES) std::cout << "      Noun: '"<< userChoice->Noun << "'  Verb: '" << userChoice->Verb << "'" << std::endl;
	// If no noun - limited choices
	if ( userChoice->Noun == "" || userChoice->Noun.compare(NOTFOUND) == 0 )
	{
		if (userChoice->Verb == (validVerbs)look) 
		{
			currentRoom->Examine(this); // Examine this room
		}
		else if (userChoice->Verb == (validVerbs)inventory) 
		{
			Examine(); // Examine the GameState (player inventory)
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
	else if (userChoice->Verb == (validVerbs)go) 
	{
		nextRoom = currentRoom->goRoom(userChoice->Noun, this);
		if ( nextRoom != currentRoom )  // that's right, we're comparing pointers now
		{
			nextRoom->Examine(this);
		}
	}
	else if (userChoice->Verb == (validVerbs)unlock && nextroomKeyName.length() > 0)
	{
		if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK][159] calling unlockRoom '" << userChoice->Noun << "'" << std::endl;
		unlockRoom(currentRoom, userChoice);
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
	Feature * solvesFeature = NULL;
	std::string nounUses = "";
	std::string textToSolve = "";
	std::string solvesHere = "";
	std::string solvesAnywhere = "";
	bool inHand = false;
	bool inRoom = false;
	bool dependenciesSolved = false;


	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::actOnFeature,"
		<< "verb = " << userChoice->printVerb()<< "(" << userChoice->Verb  << ")"
		<<", Noun = " << userChoice->Noun 
		<< ", Subject = " << userChoice->Subject << std::endl;

	theNoun = housePtr->getFeaturePtr(userChoice->Noun);
	if ( theNoun == NULL )
	{
		// Feature doesn't exist in the game
		std::cout << "There doesn't seem to be a " << userChoice->inputNoun << " anywhere nearby" << std::endl;
		return nextRoom;
	}
	inHand = featureInHand (theNoun);
	inRoom = featureInRoom (currentRoom, userChoice->Noun);
	dependenciesSolved = featureDependenciesSolved(theNoun);

  if ( ! inHand && ! inRoom ) 
	{
		std::cout << "There doesn't seem to be a " << userChoice->inputNoun << " anywhere nearby" << std::endl;
		return nextRoom;
	}
	if ( ! dependenciesSolved ) {
		std::cout << "That isn't possible right now. Keep playing the game." <<  std::endl;
		return nextRoom;
	}


  switch (userChoice->Verb)
	{
		case look:
			if (DEBUG_FUNCTION) std::cout << "      matched look" << std::endl;
			std::cout << theNoun->getExamineText() << std::endl;
			break;
		case unlock:
			if (DEBUG_FUNCTION) std::cout << "      matched unlock" << std::endl;
			unlockFeature(currentRoom, userChoice);
			break;
		case use:
			if (DEBUG_FUNCTION) std::cout << "      matched use " << std::endl;
			nounUses = theNoun->getUses();
			textToSolve = theNoun->getStringByKey("textToSolve");
			if (DEBUG_FUNCTION) std::cout << "      nounUses: " << nounUses << std::endl;
			if (DEBUG_FUNCTION) std::cout << "      theSubject: " << theSubject << std::endl;

			// Is there a USES, and if so is it nearby?
			if (  nounUses.compare("") == 0 ||(featureWithinReach(currentRoom,nounUses))) {
				if (  textToSolve.compare("") == 0 || textToSolve.compare(userChoice->Subject) == 0 ) {
					if (DEBUG_FUNCTION) std::cout << "      UsingFeature " << theSubject << std::endl;
					theNoun->useFeature(this, theSubject); 

					solvesHere = theNoun->getStringByKey("solvesHere");
					if ( solvesHere.compare("") != 0 && featureWithinReach(currentRoom,solvesHere)) {
						// Need "find one of the featuers on this list that is in reach"
						solvesFeature = housePtr->getFeaturePtr(solvesHere);
						if (solvesFeature) {
							solvesFeature->setSolved(true);
							solvesFeature->useFeature(this, theSubject);
						}
					}
				}
				else {
					// Needed "textToSolve" but didn't get it :)
					std::cout << "You're doing it wrong. Try again." << std::endl; 
				}
			}
			else {
				std::cout << "Can't find a way to " << userChoice->inputVerb 
									<< " the " << userChoice->inputNoun << " right now." << std::endl; 
			}
			break;
		case take:
			if (DEBUG_FUNCTION) std::cout << "      matched take " << std::endl;
			if ( ! inHand ) { 
				theNoun->takeFeature(this, currentRoom, theSubject); } 
			else { 
				std::cout << "You're already holding the " << userChoice->inputNoun << std::endl; }
			break;
		case drop:
			if (DEBUG_FUNCTION) std::cout << "      matched drop " << std::endl;
			if ( inHand ) { 
				theNoun->dropFeature(this, currentRoom, theSubject); }
			else { 
				std::cout << "You aren't holding the " << userChoice->inputNoun << std::endl; }
			break;
		case hit:
			if (DEBUG_FUNCTION) std::cout << "      matched hit " << std::endl;
			theNoun->hitFeature(theSubject);
			break;
		case hurl:
			if (DEBUG_FUNCTION) std::cout << "      matched throw " << std::endl;
			if ( inHand ) { 
				theNoun->hurlFeature(this, currentRoom, theSubject); }
			else { 
				std::cout << "You aren't holding the " << userChoice->inputNoun << std::endl; }
			break;
		case inventory:
			if (DEBUG_FUNCTION) std::cout << "      matched inventory " << std::endl;
			if ( inHand ) { 
				theNoun->examineFeature(); }
			else {
				Examine();
			}
			break;
		default:
			if (DEBUG_FUNCTION) std::cout << "      ERROR: fell through to default " << std::endl;
	}

	if (DEBUG_FUNCTION) std::cout << "===== end   GameState::actOnFeature" << std::endl;
	return nextRoom;
}

bool GameState::featureWithinReach(Room * currentRoom, std::string nounUses ) 
{
	Feature * theNoun;
	bool inHand = false;
	bool inRoom = false;

	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::featureWithinReach" << std::endl;
	theNoun = housePtr->getFeaturePtr(nounUses);
	if ( theNoun == NULL ) {
		if (DEBUG_FUNCTION) std::cout << "      theNoun is NULL"  << std::endl;
		return false;
	}

	inHand = featureInHand(theNoun);
	inRoom = featureInRoom(currentRoom, nounUses);

	if (DEBUG_FUNCTION) std::cout << "inHand = "<< inHand << ", inRoom = "<< inRoom<< ", OR = " << (inHand || inRoom)  << std::endl;
	return inHand || inRoom;
}

bool GameState::featureInHand(Feature * theNoun)
{
	bool inHand = false;
  // Are we carrying the Feature ?
	for ( std::vector<Feature *>::iterator iter = Holding.begin(); iter != Holding.end(); iter++) {
		if ( theNoun == *iter ) {
			inHand = true;
			break;
		}
	}
	return inHand;
}

bool GameState::featureInRoom(Room * currentRoom, std::string FName)
{
	Feature * theNoun = NULL;
	bool inRoom = false;
	
	std::string LName = strToLowercase(FName);


	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::featureInRoom, LName = " << LName << std::endl;
  // Does the Feature exist?
	theNoun = housePtr->getFeaturePtr(LName);
  if ( ! theNoun ) 
	{
		if (DEBUG_FUNCTION) std::cout << "***** Did not find " << LName << " in house->getFeaturePtr" << std::endl;
		return false;
	}

	for ( std::vector<std::string>::iterator iterStr = currentRoom->roomFeatures.begin(); 
				iterStr != currentRoom->roomFeatures.end(); iterStr++) 
	{
		if (DEBUG_FUNCTION) std::cout << "***** Comparing '" << LName << "' to '" << (*iterStr) << "'" << std::endl;
		if ( LName.compare(*iterStr) == 0 ) 
		{
			inRoom = true;
			return inRoom;
		}
	}
	if (DEBUG_FUNCTION) std::cout << "***** featureInRoom returns false "<< std::endl;
	return inRoom;
}


/* *************************************************************** */
bool GameState::featureDependenciesSolved(Feature * theNoun)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::canAccessFeature()" << std::endl;}

	Feature * checkDependency=NULL;

	// If depends_on, check the state of that object - must be solved, or we can't "use" the thing
	// Triggers = not currently used
  if ( (theNoun->getDependsOn()).compare("") != 0 ) 
	{
		// If depends_on, check the state of that object - 
		// must be solved, or we can't "use" the thing
	
		if (DEBUG_FEATURES) { std::cout << "      Checking dependency "<< theNoun->getDependsOn() << std::endl;}
		checkDependency = housePtr->getFeaturePtr(theNoun->getDependsOn());
		if (checkDependency != NULL){
			if (! checkDependency->isSolved()) {
				return false;
			}
		}
		//Missing a test case here - if checkFeature is NULL that's an error...
	}
	// Depends on nothing, or dependency is solved
	return true;
}


/* ***********************************************************
 * Constructor and destructor
 * ********************************************************* */
GameState::GameState(std::string Na)
{
	Name = Na;
	housePtr = NULL;
	puzzle = NULL;
	GameTest = false;

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
/*  Print(): This can be used for debugging, if needed */
void GameState::Print () 
{
	std::cout << Name << ". ";
}

/*  getGameTaskStatus(): If the game has more than one goal to achieve
 *  they can be tracked in the GameTask array */
int GameState::getGameTaskStatus()
{
	int points = 0;
	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::getGameTaskStatus" << std::endl;
	/*
	// Probably won't use this for CS467-Phoenix
	if (GameTask[0])
	{
		std::cout << "Game goal zero is met." << std::endl;
		points ++;
	}
	*/
	return points;
}

/*  Examine(): prints out the player's inventory */
void GameState::Examine()
{
	if ( Holding.size() == 0 ) {
		std::cout << "You aren't carrying anything." << std::endl;
		return;
	}

	std::cout << "You're carrying the following items in " << this->Name << ":" << std::endl;
	for ( std::vector<Feature *>::iterator iter=Holding.begin(); iter != Holding.end(); iter++) 
	{
		std::cout << "\t" << (*iter)->getName() << std::endl;
	}
	return;
}

int GameState::getAvailableCapacity()
{
	int GameStateIsCarrying = 0;
	std::vector<Feature *> backpackContents;
	std::vector<Feature *>::iterator iterFeature;

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

// returns lowercase string
// http://www.cplusplus.com/reference/locale/tolower/
std::string GameState::strToLowercase(std::string mixedStr)
{
	std::locale loc;

  for (std::string::size_type i=0; i<mixedStr.length(); ++i)
    mixedStr[i] = std::tolower(mixedStr[i],loc);
	return mixedStr;
}

void LongString::Wrap() {
	struct winsize WS;
	size_t searchPos;
	size_t Pos;

  // First get the current window size
	ioctl(0, TIOCGWINSZ, &WS);
	WrapLength=WS.ws_col;
	searchPos = WrapLength - 12; // Hardcoded.  Sad.  TODO.
	WrapText = Text;

	if (DEBUG_TERM) {
		std::cout << "++++ LongString::Wrap: WrapLength = " << WrapLength << ", TextLength = " << Text.length() << std::endl;
	}
	// If short, don't wrap
	if ( Text.length() < WrapLength )
	{
		return;
	}

	// Wrap text if necessary
	Pos = WrapText.find(" ", searchPos);
	if (DEBUG_TERM) { std::cout << "searchPos = "<< searchPos <<", Pos = "<< Pos << std::endl;}
	while ( Pos != string::npos ){
		if (DEBUG_TERM) { std::cout << "looooooping, pos="<< Pos << std::endl;}
		WrapText.replace(Pos, (size_t) 1, "\n");
		searchPos = Pos + WrapLength - 12;
		Pos = WrapText.find(" ", searchPos);
		if (DEBUG_TERM) { std::cout << "searchPos = "<< searchPos <<", Pos = "<< Pos << std::endl;}
	}
	if (DEBUG_TERM) {
		std::cout << "Text: " << Text << std::endl;
		std::cout << "Wrap: " << WrapText << std::endl;
		std::cout << "-------------------- END DEBUG ------------------" << std::endl;
	}
	

	//std::string Text;
	//std::string WrapText;
	//int WrapLength;
}

void GameState::unlockRoom(Room * currentRoom, Choice * userChoice) 
{
	Room * nextRoom = NULL; 
	Feature *feature = NULL;
	std::string unlockFromThisRoom = "";
	std::string nextroomKeyName = "";

	if (DEBUG_FUNCTION || DEBUG_LOCK ) std::cout << "===== begin GameState::unlockRoom" << std::endl;

	// see what room we are trying to unlock.
	nextroomKeyName = currentRoom->getExitRoomByKey(userChoice->Noun, false);
	if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] nextroomKeyName: " << unlockFromThisRoom << std::endl;

	if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] Noun (" << nextroomKeyName << ") is a room." << std::endl;
	// iterate through the features to see if one has a UNLOCK for what we want, if so, return it.
	feature = currentRoom->findFeatureByUnlocksString(nextroomKeyName, this);	
	if(DEBUG_LOCK && feature) {
		std::cout << "    [DEBUG_LOCK] Found feature that is locking this: " << feature->getStringByKey("name") << std::endl;
	}
	else if (DEBUG_LOCK)
	{
		std::cout << "    [DEBUG_LOCK] Did not find Feature " << std::endl;
	}

	if(feature) {

		if ( ! feature->isSolved() )
		{
			std::cout << "Looks like you need to unlock the " << feature->getName() << "." << std::endl;
		}
		else
		{
			unlockFromThisRoom = currentRoom->getKeyName(); // Why do we need this? TODO
	
			if(unlockFromThisRoom.length() > 0 && nextroomKeyName.length() > 0)
			{
				if(DEBUG_LOCK) std::cout << "    unlockFromThisRoom.length() > 0 && nextroomKeyName.length() > 0" << std::endl;
				nextRoom = currentRoom->getRoomOtherSideOfDoor(nextroomKeyName, this);
					if (nextRoom != NULL)
				{
					if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] Unlocking doors" << std::endl;
					// TODO: need to check and make sure we did what we needed to do in order to unlock the door"
					// unlock door to requested room
					currentRoom->unlockExitDoorByKey(nextRoom->getKeyName());
					// unlock door from requested room to this room.
					nextRoom->unlockExitDoorByKey(currentRoom->getKeyName());
					nextRoom = currentRoom;   // so we dont't actually move to the next room.
					if(currentRoom->getUnlockText().length() > 0)
					{
						std::cout << currentRoom->getUnlockText() << std::endl;
					}
				}
				else
				{
					std::cout << "GameState::ActInRoom ERROR: received NULL when retreiving door pointer." << std::endl;
					exit(1);
				}
			}
		}
	}
	else {
		std::cout << "You don't seem to be able to unlock the " << userChoice->Noun << " right now\n" << std::endl;
	}
}

void GameState::unlockFeature(Room * currentRoom, Choice * userChoice) 
{
	Room * nextRoom = NULL; 
	Feature *feature = NULL;
	std::string unlockFromThisRoom = "";
	std::string nextroomKeyName = "";

	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::unlockFeature" << std::endl;

	// Just confirm the object is in the house and in the room. 
	// We can probably delete this actually, since it's handled in
	// actOnFeature above.
	if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] Noun (" << userChoice->Noun << ") is a feature." << std::endl;
	if(currentRoom->isFeatureInThisRoom(userChoice->Noun))
	{
		feature = housePtr->getFeaturePtr(userChoice->Noun);
		if(DEBUG_LOCK)
		{
			if(feature)
			{
				std::cout << "    [DEBUG_LOCK] feature pointer found" << std::endl;
			}
			else
				std::cout << "ERR    [DEBUG_LOCK] feature pointer NULL!!!!!" << std::endl;
		}
	}

	if( ! feature->solveFeature(this, strToLowercase(userChoice->Subject)))
	{
		if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] feature not solved, can't unlock" << std::endl;
		std::cout << "That didn't seem to do the trick. Are you missing something?" << std::endl;
		unlockFromThisRoom = "";
		nextroomKeyName = "";
		return;
		// Can we just return here?
	}
		if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] Next room is  " << feature->getStringByKey("unlocks") << std::endl;
		nextRoom = housePtr->getRoomPtr(feature->getStringByKey("unlocks"));
		nextroomKeyName = nextRoom->getKeyName();
		unlockFromThisRoom = currentRoom->getKeyName();
		if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] feature complete go ahead and unlock " << nextRoom->getRoomName() << std::endl;

		//		if(DEBUG_LOCK) std::cout << "     [DEBUG_LOCK] this room: " << currentRoom->getRoomName() << "  door to: " << nextRoom->getRoomName() << std::endl;

		if(unlockFromThisRoom.length() > 0)
		{
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] unlockFromThisRoom: " << unlockFromThisRoom << std::endl;
		}
		else
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] unlockFromThisRoom: NULL" << std::endl;


		if(nextroomKeyName.length() > 0)
		{
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] nextroomKeyName: " << nextroomKeyName << std::endl;
		}
		else
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] nextroomKeyName: NULL" << std::endl;
		
		if(unlockFromThisRoom.length() > 0 && nextroomKeyName.length() > 0)
		{
			if(DEBUG_LOCK) std::cout << "    unlockFromThisRoom.length() > 0 && nextroomKeyName.length() > 0" << std::endl;
			nextRoom = currentRoom->getRoomOtherSideOfDoor(nextroomKeyName, this);
			if (nextRoom != NULL)
			{
				if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] Unlocking doors" << std::endl;
				// TODO: need to check and make sure we did what we needed to do in order to unlock the door"
				// unlock door to requested room
				currentRoom->unlockExitDoorByKey(nextRoom->getKeyName());
				// unlock door from requested room to this room.
				nextRoom->unlockExitDoorByKey(currentRoom->getKeyName());
				nextRoom = currentRoom;   // so we dont't actually move to the next room.
				if(currentRoom->getUnlockText().length() > 0)
				{
					std::cout << currentRoom->getUnlockText() << std::endl;
				}
			}
			else
			{
				std::cout << "GameState::ActInRoom ERROR: received NULL when retreiving door pointer." << std::endl;
				// exit(1);
			}
		}
		else
		nextRoom = currentRoom;
}
void GameState::printHelp(Room * currentRoom)
{
	bool Long = false;
	printHelp(Long);
	// TODO: special in-room help here
}
void GameState::printHelp() {
	bool Long = false;
	printHelp(Long);
}

void GameState::printHelp(bool Long) {
	if ( ! Long ) {
		std::cout << "=============================================" << std::endl;
		std::cout << "\nTo move around use verbs like 'go', 'move', 'walk', etc" << std::endl;
		std::cout << "\nTo interact with objects in the room try words like 'use','take','move', etc" << std::endl;
		std::cout << "=============================================" << std::endl;
	}
	else
	{
		std::cout << "=============================================" << std::endl;
		std::cout << "Valid commands include all of the following: " << std::endl;
		std::cout << "   go <direction or room name>               " << std::endl;
		std::cout << "   look  (with or without an object)         " << std::endl;
		std::cout << "   use   <object>                            " << std::endl;
		std::cout << "   take  <object>                            " << std::endl;
		std::cout << "   drop  <object>                            " << std::endl;
		std::cout << "   open  <object>                            " << std::endl;
		std::cout << "   close <object>                            " << std::endl;
		std::cout << "   throw <object>                            " << std::endl;
		std::cout << "   hit   <object>                            " << std::endl;
		std::cout << "   unlock <object>                           " << std::endl;
		std::cout << "   inventory                                 " << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Some commands don't work with some objects.  " << std::endl;
		std::cout << "Many synonyms and alternate commands are also" << std::endl;
		std::cout << "supported.                                   " << std::endl;
		std::cout << "=============================================" << std::endl;
	}
}

// Saves the game state to saveGame.txt
void GameState::saveGame(Room *currentRoom) {

	std::vector<std::string> stringVector;
	std::string saveString = "";

	// need to save current room key
	saveString.append("CURRENT_ROOM:");
	saveString.append(currentRoom->getKeyName());
	saveString.append("\n");


	// need to save keys of curent inventory
	for (std::vector<Feature*>::iterator iter = Holding.begin(); iter != Holding.end(); iter ++ )
	{
		saveString.append("HOLDING:");
		saveString.append((*iter)->getKeyName());
		saveString.append("\n");
	}

	// need to save all the done features by key
	stringVector = housePtr->getSolvedFeatures();
	for (std::vector<std::string>::iterator iter = stringVector.begin(); iter != stringVector.end(); iter ++ )
	{
		//if(DEBUG_BRENT) std::cout << (*iter) << std::endl;
		saveString.append("SOLVED:");
		saveString.append((*iter));
		saveString.append("\n");
	}

	// room features:
	saveString.append(housePtr->getRoomFeaturesSaveString());


	// locked doors  
	saveString.append(housePtr->getRoomLockedDoorsSaveString());

	if(DEBUG_BRENT) std::cout << saveString << std::endl;
   std::ofstream of("saveGame.txt");
   of << saveString;
   of.close();

}



void GameState::loadGame() {
	//
	ifstream loadFile;
	std::string lineStr, currentRoomStr = "", tempStr;
	const char * nameCstr;
	char nwords[256];
	char * nptr;

	loadFile.open("saveGame.txt");  // .c_str() got from https://stackoverflow.com/questions/19531269/c-void-function-with-file-stream-error
	if (loadFile.is_open())
	{
		// https://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once
		// http://www.cplusplus.com/reference/string/string/substr/
	   while (std::getline(loadFile, lineStr))  
	   {
			if (lineStr.substr(0,1).find("#") != std::string::npos )
			{
				// Skip lines that begin with # for comments
				continue;
			}
   		if(lineStr.find("CURRENT_ROOM:") != std::string::npos)
   		{
   			currentRoomStr = lineStr.substr(13, lineStr.length()-1);
   			std::cout << "TODO: Set current room pointer to " << currentRoomStr << std::endl;
				continue;
   		}

   		if(lineStr.find("HOLDING:") != std::string::npos)
   		{
   			tempStr = lineStr.substr(8, lineStr.length()-1);
   			std::cout << "TODO: add " << tempStr << " to holding. " << std::endl;
				continue;
   		}

   		if(lineStr.find("SOLVED:") != std::string::npos)
   		{
   			tempStr = lineStr.substr(7, lineStr.length()-1);
   			std::cout << "TODO: set feature " << tempStr << " as solved. " << std::endl;
				continue;
   		}
 
			if(lineStr.find("ROOM_FEATURE:") != std::string::npos)
			{
				tempStr = lineStr.substr(13, lineStr.length()-1);
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
				{
					if ( tempStr.find("|") != std::string::npos)
					{
						nameCstr = tempStr.c_str();
						strcpy(nwords, nameCstr);
						nptr = strtok (nwords, "|");
						if (nptr != NULL) {
							tempStr.erase(tempStr.find("|"));
							nptr = strtok(NULL, ", ");
							while (nptr != NULL ) 
							{
								std::cout << "TODO: add " << nptr << " as a feature to " << tempStr << std::endl;
								nptr = strtok(NULL, ", ");
							}
						}
					}

				}
			}

			if(lineStr.find("LOCKED_DOOR:") != std::string::npos)
			{
				tempStr = lineStr.substr(12, lineStr.length()-1);
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
				{
					if ( tempStr.find("|") != std::string::npos)
					{
						nameCstr = tempStr.c_str();
						strcpy(nwords, nameCstr);
						nptr = strtok (nwords, "|");
						if (nptr != NULL) {
							tempStr.erase(tempStr.find("|"));
							nptr = strtok(NULL, ", ");
							while (nptr != NULL ) 
							{
								std::cout << "TODO: Lock door in " << tempStr << " going to " << nptr << std::endl;
								nptr = strtok(NULL, ", ");
							}
						}
					}

				}
			}

	   }
	 }
	 else {
	 	std::cout << "Error opening save file." << std::endl;
	 }

	// make sure to set currentRoom.
}
