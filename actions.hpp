#include <iostream>
#include <string>
#include <list>
#include <vector>

#ifndef _Actions_H
#define _Actions_H

#include "container.hpp"

class Thing; // Forward declaration
class Doorway; // Forward declaration
class Room; // Forward declaration
class Choice; // Forward declaration


class Actions {
  public: 
		std::vector<Thing*> Things;
		std::vector<Doorway*> Doors;

		Actions();
		~Actions();

		Choice * userChooses();


};

struct Choice {
	Room  * nextRoom;
	Thing * useThing;
	int verb;

	// Default constructor
	Choice() {
		nextRoom = NULL;
		useThing = NULL;
		verb = -1;
	};
};

#endif // _Actions_H
