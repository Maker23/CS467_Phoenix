#ifndef _UTE_H
#define _UTE_H

#define ROOM_DIRECTORY "rooms/"
#define FEATURE_DIRECTORY "rooms/features/"

//  Global debugging flags
#define DEBUG_EXAMINE false
#define DEBUG_FUNCTION false
#define DEBUG_PARSER false
#define DEBUG_TERM false
#define DEBUG_FIND false
#define DEBUG_FEATURES false
#define DEBUG_HOUSE false
#define DEBUG_PUZZLE false
#define DEBUG_ROOM false
#define DEBUG_USE false
#define DEBUG_BRENT false
#define DEBUG_LOCK false

#ifndef TEST
#define TEST false
#endif

#define NOTFOUND "not found"

class Container; // Forward declaration
class Room; // Forward declaration
class Feature; // Forward declaration
class GameState; // Forward declaration

struct lockDoorStruct
{
	std::string doorFrom;
	std::string doorTo;
};

#endif // _UTE_H
