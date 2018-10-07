#include <iostream> // for debugging
#include <list>
#include <string>

class Room; // Forward declaration
class Container; // Forward declaration

#ifndef _CONTAINER_H
#define _CONTAINER_H

#include "actions.hpp"
#include "utilities.hpp"

class Thing {
public:
	std::string Name;
	std::string Story;
	int Weight;
	bool isContainer;
	bool Open;

	Thing (std::string Na, std::string St); // constructor
	virtual ~Thing();

	bool (*UseFunc)(Room *, Container *);	//Does this thing do something? Most do not
	bool (*OpenFunc)(Container *); // Does this thing open? Most do not
	virtual void Print();
	virtual std::vector<Thing*> Examine(int &counter, bool verbose=true, bool silent=false);
	virtual int getWeight() {return Weight;};
};


class Container : public Thing {
public:

	Container (std::string Na, std::string St); // constructor
	virtual ~Container (); // destructor

	bool 	Open;
	int 	Capacity; // How much can it hold
	std::list<Thing*> Contents; // Objects held by container

	bool (*OpenFunc)(Container*);
	bool (*UseFunc)(Room *, Container*);
	virtual int getCapacity() { return Capacity;}
	virtual std::vector<Thing*> Examine(int &counter, bool verbose=true, bool silent=false);
	virtual void Print();
	virtual Container * FindByPtr(Thing *);
	virtual Thing * FindByName(std::string);
};

class GameState : public Container {
public:
	GameState(std::string Na);
	~GameState();

	bool GameTask[numGameTasks]; // GameTasks are defined in utilities.h

	int getGameTaskStatus();
	int getAvailableCapacity(); // Capacity - ThingsCarried = Available
	int getCapacity() { return Capacity;}
	std::vector<Thing*> Examine(int &counter, bool verbose=false, bool silent=false);
};


#endif // _CONTAINER_H
