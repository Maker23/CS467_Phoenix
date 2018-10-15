#ifndef _CONTAINER_H
#define _CONTAINER_H

#include <iostream> // for debugging
#include <list>
#include <string>

class Room; // Forward declaration
class Container; // Forward declaration
class House; // Forward declaration

#include "actions.hpp"
#include "utilities.hpp"
#include "actions.hpp"
#include "house.hpp"

class Thing {
public:
	std::string Name;
	std::string Story;
	bool isContainer;
	bool Open;
	int Weight;	// We may not use weight and capacity - TBD. 
	int Capacity;

	Thing (std::string Na, std::string St); // constructor
	Thing (std::string Na);	// constructor
	virtual ~Thing();				// destructor

  /* ********************************************************** */
	//std::list<validVerbs> Verbs; 		
	std::list<int> Verbs; 		
	std::vector<Thing*> Contents;   // Objects held by container are also Things
	void Print();

	// Don't know if we're going to use these
	bool (*UseFunc)(Room *, Thing *);	//Does this thing do something? Most do not
	bool (*OpenFunc)(Thing *); 				// Does this thing open? Most do not
	virtual std::vector<Thing*> Examine(bool reCursive=true, bool verbose=true, bool silent=false); 
	virtual int getWeight() {return Weight;};
	virtual Thing * FindByName(std::string);
};


/*
 * The GameState is just a Feature with a few special options.
 *
 * It's a container, and contains the things that the Player is carrying.
 *
 */
class GameState : public Thing {
public:
	GameState(std::string Na);
	~GameState();

	House * housePtr;
	bool GameTask[numGameTasks]; // GameTasks are defined in utilities.h

	int getGameTaskStatus();
	int getAvailableCapacity(); // Capacity - ThingsCarried = Available
	int getCapacity() { return Capacity;}
	std::vector<Thing*> Examine(int &counter, bool verbose=false, bool silent=false);
};


#endif // _CONTAINER_H
