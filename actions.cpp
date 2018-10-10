#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "room.hpp"
#include "container.hpp"
#include "actions.hpp"
#include "utilities.hpp"

using namespace std;

// Constructor for Doorway class. 
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

