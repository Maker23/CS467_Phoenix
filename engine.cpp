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
	std::string verbPrint[] = {"look", "go", "use", "pick up or take", "drop", "open", "close", "throw", "hit", "unlock", "examine"};

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
		std::cout << "THERE IS NO HELP FOR YOU" << std::endl;
		return currentRoom;
	}
	if( (userChoice->Verb == (validVerbs)save) ||
			(userChoice->Verb == (validVerbs)load) )
	{
		std::cout << "Game does not yet support Save or Load" << std::endl;
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
		if (DEBUG_FUNCTION) std::cout << "      found feature "<< userChoice->Noun << ", looking for " << userChoice->inputVerb << std::endl;
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
	/*
	else if (userChoice->Verb == (validVerbs)unlock)
	{
		// Pass the verb and noun(s) to ActInRoom
		//if(DEBUG_BRENT) std::cout << "[DEBUG_BRENT] this room: " << currentRoom->getRoomName() << "  door to: " << nextRoom->getRoomName() << std::endl;
		//nextRoom = actOnFeature(currentRoom, userChoice);
		nextRoom = NULL;

		std::string unlockFromThisRoom = "";
		std::string nextroomKeyName = "";
		// see what room we are trying to unlock.
		nextroomKeyName = currentRoom->getExitRoomByKey(userChoice->Noun, false);
		if(DEBUG_FUNCTION) std::cout << "    [DEBUG_FUNCTION] unlockFromThisRoom: " << unlockFromThisRoom << std::endl;
		if(nextroomKeyName.length() > 0)  // noun is a room, so get feature by the hard way.
		{
			if(DEBUG_FUNCTION) std::cout << "    [DEBUG_FUNCTION] Noun (" << nextroomKeyName << ") is a room." << std::endl;
			// iterate through the features to see if one has a UNLOCK for what we want, if so, return it.
			feature = currentRoom->findFeatureByUnlocksString(nextroomKeyName, this);	
			if(DEBUG_FUNCTION) std::cout << "    [DEBUG_FUNCTION] Found feature that is locking this: " << feature->getStringByKey("name") << std::endl;
			if(feature)
				unlockFromThisRoom = currentRoom->getKeyName();
		}
		else // it's a pointer.
		{
			if(DEBUG_FUNCTION) std::cout << "    [DEBUG_FUNCTION] Noun (" << userChoice->Noun << ") is a feature." << std::endl;
			if(currentRoom->isFeatureInThisRoom(userChoice->Noun))
			{
				feature = housePtr->getFeaturePtr(userChoice->Noun);
				if(DEBUG_FUNCTION)
				{
					if(feature)
					{
						std::cout << "    [DEBUG_FUNCTION] feature pointer found" << std::endl;
					}
					else
						std::cout << "ERR    [DEBUG_FUNCTION] feature pointer NULL!!!!!" << std::endl;
				}
			}
		}

		if(feature->solveFeature(this, strToLowercase(userChoice->Subject)))
		{
			if(DEBUG_FUNCTION) std::cout << "    [DEBUG_FUNCTION] Next room is  " << feature->getStringByKey("unlocks") << std::endl;
			nextRoom = housePtr->getRoomPtr(feature->getStringByKey("unlocks"));
			nextroomKeyName = nextRoom->getKeyName();
			unlockFromThisRoom = currentRoom->getKeyName();
			if(DEBUG_FUNCTION) std::cout << "    [DEBUG_FUNCTION] feature complete go ahead and unlock " << nextRoom->getRoomName() << std::endl;
		}
		else
		{
			if(DEBUG_FUNCTION) std::cout << "    [DEBUG_FUNCTION] feature not solved, can't unlock" << std::endl;
			unlockFromThisRoom = "";
			nextroomKeyName = "";
		}

//		if(DEBUG_BRENT) std::cout << "     [DEBUG_BRENT] this room: " << currentRoom->getRoomName() << "  door to: " << nextRoom->getRoomName() << std::endl;

		if(unlockFromThisRoom.length() > 0)
		{
			if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] unlockFromThisRoom: " << unlockFromThisRoom << std::endl;
		}
		else
			if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] unlockFromThisRoom: NULL" << std::endl;


		if(nextroomKeyName.length() > 0)
		{
			if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] nextroomKeyName: " << nextroomKeyName << std::endl;
		}
		else
			if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] nextroomKeyName: NULL" << std::endl;
		
		if(unlockFromThisRoom.length() > 0 && nextroomKeyName.length() > 0)
		{
			if(DEBUG_BRENT) std::cout << "    unlockFromThisRoom.length() > 0 && nextroomKeyName.length() > 0" << std::endl;
			nextRoom = currentRoom->getRoomOtherSideOfDoor(nextroomKeyName, this);
			if (nextRoom != NULL)
			{
				if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] Unlocking doors" << std::endl;
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
		else
			nextRoom = currentRoom;
	}
	*/
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
	std::string nounUses = "";
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
			if (DEBUG_FUNCTION) std::cout << "      nounUses: " << nounUses << std::endl;
			if (DEBUG_FUNCTION) std::cout << "      theSubject: " << theSubject << std::endl;
			if (  nounUses.compare("") == 0 
				 ||(featureWithinReach(currentRoom,nounUses))) {
				theNoun->useFeature(this, theSubject); }
			else {
				std::cout << "Can't find a way to use the " << userChoice->inputNoun << " right now." << std::endl; }
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

	if (DEBUG_FUNCTION) std::cout << "inHand = "<< inHand << ", inRoom = "<< inRoom<< ", or equals" << (inHand || inRoom)  << std::endl;
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

	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::featureInRoom, FName = " << FName << std::endl;
  // Does the Feature exist?
	theNoun = housePtr->getFeaturePtr(FName);
  if ( ! theNoun ) 
	{
		if (DEBUG_FUNCTION) std::cout << "***** Did not find " << FName << " in house->getFeaturePtr" << std::endl;
		return false;
	}

	for ( std::vector<std::string>::iterator iterStr = currentRoom->roomFeatures.begin(); 
				iterStr != currentRoom->roomFeatures.end(); iterStr++) 
	{
		if (DEBUG_FUNCTION) std::cout << "***** Comparing '" << FName << "' to '" << (*iterStr) << "'" << std::endl;
		if ( FName.compare(*iterStr) == 0 ) 
		{
			inRoom = true;
			return inRoom;
		}
	}
	if (DEBUG_FUNCTION) std::cout << "***** Fell through to false :( "<< std::endl;
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

void GameState::unlockFeature(Room * currentRoom, Choice * userChoice) 
{
	Room * nextRoom = NULL; 
	Feature *feature = NULL;
	std::string unlockFromThisRoom = "";
	std::string nextroomKeyName = "";

		// Pass the verb and noun(s) to ActInRoom
		//if(DEBUG_BRENT) std::cout << "[DEBUG_BRENT] this room: " << currentRoom->getRoomName() << "  door to: " << nextRoom->getRoomName() << std::endl;
		//nextRoom = actOnFeature(currentRoom, userChoice);
		nextRoom = NULL;

		// see what room we are trying to unlock.
		nextroomKeyName = currentRoom->getExitRoomByKey(userChoice->Noun, false);
		if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] unlockFromThisRoom: " << unlockFromThisRoom << std::endl;
		if(nextroomKeyName.length() > 0)  // noun is a room, so get feature by the hard way.
		{
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] Noun (" << nextroomKeyName << ") is a room." << std::endl;
			// iterate through the features to see if one has a UNLOCK for what we want, if so, return it.
			feature = currentRoom->findFeatureByUnlocksString(nextroomKeyName, this);	
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] Found feature that is locking this: " << feature->getStringByKey("name") << std::endl;
			if(feature)
				unlockFromThisRoom = currentRoom->getKeyName();
		}
		else // it's a pointer.
		{
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
		}

		if(feature->solveFeature(this, strToLowercase(userChoice->Subject)))
		{
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] Next room is  " << feature->getStringByKey("unlocks") << std::endl;
			nextRoom = housePtr->getRoomPtr(feature->getStringByKey("unlocks"));
			nextroomKeyName = nextRoom->getKeyName();
			unlockFromThisRoom = currentRoom->getKeyName();
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] feature complete go ahead and unlock " << nextRoom->getRoomName() << std::endl;
		}
		else
		{
			if(DEBUG_LOCK) std::cout << "    [DEBUG_LOCK] feature not solved, can't unlock" << std::endl;
			unlockFromThisRoom = "";
			nextroomKeyName = "";
		}

//		if(DEBUG_BRENT) std::cout << "     [DEBUG_BRENT] this room: " << currentRoom->getRoomName() << "  door to: " << nextRoom->getRoomName() << std::endl;

		if(unlockFromThisRoom.length() > 0)
		{
			if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] unlockFromThisRoom: " << unlockFromThisRoom << std::endl;
		}
		else
			if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] unlockFromThisRoom: NULL" << std::endl;


		if(nextroomKeyName.length() > 0)
		{
			if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] nextroomKeyName: " << nextroomKeyName << std::endl;
		}
		else
			if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] nextroomKeyName: NULL" << std::endl;
		
		if(unlockFromThisRoom.length() > 0 && nextroomKeyName.length() > 0)
		{
			if(DEBUG_BRENT) std::cout << "    unlockFromThisRoom.length() > 0 && nextroomKeyName.length() > 0" << std::endl;
			nextRoom = currentRoom->getRoomOtherSideOfDoor(nextroomKeyName, this);
			if (nextRoom != NULL)
			{
				if(DEBUG_BRENT) std::cout << "    [DEBUG_BRENT] Unlocking doors" << std::endl;
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
