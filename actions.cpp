#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "room.hpp"
#include "container.hpp"
#include "actions.hpp"
#include "utilities.hpp"

using namespace std;

// Constructor for Actions class. 
Actions::Actions()
{
}

Actions::~Actions()
{
  // TODO: Delete userChoice from userChooses()
}

Choice * Actions::userChooses()
{
	if (DEBUG_FUNCTION) std::cout << "===== begin Actions::userChooses" << std::endl;
	Choice * userChoice = new Choice();
	
	// TODO: Implement this

	return userChoice;
}

std::string Choice::printVerb()
{
	std::string verbPrint[] = {"look", "go", "use", "pick up or take", "drop", "open", "close", "throw", "hit", "eat", "examine"};

	return verbPrint[(int)Verb];
}
