#ifndef _UTE_H
#define _UTE_H

#define ROOM_DIRECTORY "rooms/"
#define FEATURE_DIRECTORY "rooms/features/"

//  Global debugging flags
#define DEBUG_EXAMINE false
#define DEBUG_FUNCTION true
#define DEBUG_TERM false
#define DEBUG_FIND false
#define DEBUG_ROOM false
#define DEBUG_USE false
#define DEBUG_FEATURES true
#define DEBUG_BRENT false
#define DEBUG_HOUSE false

#ifndef TEST
#define TEST false
#endif

#define NOTFOUND "not found"

class Container; // Forward declaration
class Room; // Forward declaration
class Feature; // Forward declaration
class GameState; // Forward declaration



#endif // _UTE_H
