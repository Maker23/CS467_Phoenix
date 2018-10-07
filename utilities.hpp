#ifndef _UTE_H
#define _UTE_H

//  Global debugging flags
#define DEBUG_EXAMINE false
#define DEBUG_FUNCTION true
#define DEBUG_MENU false
#define DEBUG_FIND false
#define DEBUG_ROOM false
#define DEBUG_USE false

#ifndef TEST
#define TEST false
#endif

#define numGameTasks 5
#define GameLength 52	// Number of turns the user gets TODO: remove this

#include <string>

class Container; // Forward declaration
class Room; // Forward declaration
class Thing; // Forward declaration
class GameState; // Forward declaration

bool getUserYN(const char Default);
std::string WriteTheInstructions();
void UpdateGameState(int &GameClock, GameState* PlayerState, Room* currentRoom);


#endif // _UTE_H
