#ifndef _GAMEENGINE_H
#define _GAMEENGINE_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>

#define CRYPTED_STRING "Muaf o whxxwd psoub fog krupx xid mwofd lihzi whyixp xid wofs xa udqdow xid yofd."
#define CLEAR_STRING   "From a little spark may burst the flame which lights the lamp to reveal the game."

// TODO : Is this the right set of verbs
//save and load added to list
enum validVerbs { look, go, use, take, drop, open, shut, hurl, hit, unlock, inventory, hint, LastAction, help, save, load, quit, unknown, LastVerb};
//                0     1   2    3     4     5     6     7     8    9       10         11    12          13    14    15    16    17
                 

#include "utilities.hpp"
#include "house.hpp"
#include "feature.hpp"
#include "puzzle.hpp"


class Feature; // Forward declaration
class Doorway; // Forward declaration
class Room; // Forward declaration
class House; // Forward declaration
struct Choice; // Forward declaration

#define numGameTasks 5


/*
 * The GameState 
 *
 */
class GameState 
{
private:
	std::string Name;
	int Capacity; // How much the player can carry
	std::string strToLowercase(std::string);

public:
	std::vector<Feature *> Holding;  // Features by name
	bool GameTask[numGameTasks]; // GameTasks are defined in utilities.h
	bool GameTest;
	std::string   GameTestFilename;
	std::ifstream GameTestFile;
	std::string   GameDirectory;
	House * housePtr;
	unsigned short winCols;
	unsigned short winRows;
	Puzzle * puzzle;

	GameState(std::string Na);
	~GameState();
	void Print();
	void Examine();
	Room * playerTurn (Room *);
	Room * actInRoom (Room *, Choice *);
	Room * actOnFeature (Room *, Choice *);
	void printHelp();
	void printHelp(Room *);
	void printHelp(bool);
	void unlockRoom (Room *, Choice *);
	void unlockFeature (Room *, Choice *);
	bool featureInHand (Feature *);
	bool featureInRoom (Room *, std::string);
	bool featureWithinReach (Room *, std::string);
	bool featureDependenciesSolved(Feature *);
	int getGameTaskStatus();
	int getAvailableCapacity(); // Capacity - FeaturesCarried = Available
	int getCapacity() { return Capacity;}
	void UpdateGameState(int &GameClock, Room* currentRoom);
	void getOverrideVerb(Choice *);
	void saveGame(Room *);
	Room * loadGame(Room *);
};

class LongString
{
private:
	std::string Text;
	std::string WrapText;
	unsigned short WrapLength;
	void Wrap ();

public:
	LongString(std::string inPut) {
		Text = inPut;
		WrapText = "";
		WrapLength=77; // default
	};

	std::string getWrappedText() {
		if (SILENCE_DESCRIPTIONS) return "";
		Wrap();
		return WrapText;
	};
	
	friend std::ostream& operator<< (std::ostream &out, LongString &lstring)
	{
		lstring.Wrap();
		out << lstring.WrapText;
		return out;
	};
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
