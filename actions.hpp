#ifndef _Actions_H
#define _Actions_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>

#include "container.hpp"

class Thing; // Forward declaration
class Doorway; // Forward declaration
class Room; // Forward declaration
class Choice; // Forward declaration

// TODO : Is this the right set of verbs
//save and load added to list
enum validVerbs { look, go, use, take, drop, open, close, hurl, hit, eat, inventory, LastAction, help, save, load, quit, tst, LastVerb};

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
	Thing * useOn;
	Doorway * goDoorway;

	validVerbs Verb;
	std::string Noun;
	std::string Subject;

	std::string printVerb();

	// Default constructor
	Choice() {
		nextRoom = NULL;
		useThing = NULL;
		useOn = NULL;
		goDoorway = NULL;
		Verb = tst;
		Noun = "";
		Subject = "";
	};
};



#endif // _Actions_H
