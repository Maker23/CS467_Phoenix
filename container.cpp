/* vim:ts=2:
 *
 * File name: Container.cpp
 *
 * Overview:
 *   General functions that work on containers.
 *
 * 	 Container::Examine prints the contents of a container, recursively,
 * 	 and returns a Thing* vector.
 *
 * 	 Container::FindByPtr searches for an object by pointer and returns
 * 	 the holding container if it's found, NULL if not
 *
 * 	 Container::FindByName searches for an object by name and returns
 * 	 a pointer to the object if found, NULL if not
 *
 * 	 GameState:: functions apply to the derived class which is the player's
 * 	 game state 
 *
 */

#include <iostream> // for debugging
#include <list>
#include <string>

#include "container.hpp"


/* ***********************************************************
 * Constructors
 * ********************************************************* */
Thing::Thing (std::string Na, std::string St)
{
	Name=Na;
	Story=St;
	Weight=1;
	UseFunc = NULL;
	OpenFunc = NULL;
	isContainer=false;
	Open=true;
}

Container::Container (std::string Na, std::string St) : Thing (Na, St)
{
	Name=Na;
	Story=St;
	Open=true; 		// Much easier to play this way
	Weight=100; 	// By default - too heavy to carry
	Capacity=99; 	// By default - basically no limit on the number of Things
	UseFunc = NULL;
	OpenFunc = NULL;
	isContainer = true;
}

GameState::GameState(std::string Na) : Container (Na,"")
{
	GameTask[0] = false;
	GameTask[1] = false;
	GameTask[2] = false;
	GameTask[3] = false;
	GameTask[4] = false;
}

/* ***********************************************************
 * Destructors
 * ********************************************************* */
Thing::~Thing()
{
	//TODO
}

Container::~Container()
{
	std::list<Thing*>::iterator iterThing;
	for (iterThing=Contents.begin(); iterThing != Contents.end(); iterThing++)
	{
		delete (*iterThing);
	}
}

GameState::~GameState() 
{
	// No dynamic memory beyond the Container base class
}

/* ********************************************************* */
void Thing::Print () 
{
	std::cout << Name << ". ";
}

/* ********************************************************* */
std::vector<Thing*> Thing::Examine(int &counter, bool verbose, bool silent)
{
	std::vector<Thing*> Nothing;

	std::cout << Name << ". " << Story << std::endl;

  if (DEBUG_EXAMINE) 
	{ 
		std::cout  << "DEBUG: from Thing::Examine " << std::endl;
		std::cout << "		Name = " << Name << std::endl
			<< "		Story = " << Story << std::endl
			<< "		Weight = " << Weight << std::endl
			<< "		UseFunc = " << UseFunc << std::endl
			<< "		OpenFunc = " << OpenFunc << std::endl
			<< "		isContainer = " << isContainer << std::endl
			<< "		Open = " << Open << std::endl;
	}
	return Nothing; // return an empty vector
}


/* ********************************************************* */
void Container::Print()
{
	std::cout << "You are looking at a " << this->Name << std::endl;
	std::cout << this->Story << std::endl;
}

/* ***********************************************************
 * 
 * 
 *
 * ********************************************************* */
std::vector<Thing*> Container::Examine(int &counter, bool verbose, bool silent)
{
	std::list<Thing*>::iterator iter;
	std::vector<Thing*> AllContents;
	std::vector<Thing*> subContents;

  if (DEBUG_FUNCTION) std::cout << "===== begin Container::Examine" << std::endl;
  if (DEBUG_EXAMINE)
	{	
		std::cout  << "DEBUG: Container::Examine counter=" <<counter
			<<  ", Open = " << Open
			<< ", isContainer = " << isContainer << std::endl;
	}
	if ( (this->Story).compare("") != 0 && !silent)
		std::cout << "	    " << this-> Name << ".  " << this->Story << std::endl;

  if ( ! Open )
	{
  	if (OpenFunc != NULL) {
			if (DEBUG_EXAMINE) std::cout << "DEBUG: Running OpenFunc " 
				<< OpenFunc << " on " << this->Name << " (Container::Examine)" 
				<< std::endl;
		
			// We're not really trying to open the container here; we 
			// just want to get any useful printout from the OpenFunc, 
			// for example, "look for a silver key" or somesuch.
			// So we pass the function a NULL container*
			bool Success = OpenFunc((Container*) NULL); 
			if (!Success && !silent) 
			{
				std::cout << "	    The " << this->Name 
					<< " won't open. Maybe it's locked." << std::endl;
				return AllContents;
			}
		}
		else {
			if (!silent) std::cout << "	    The " << this->Name << " doesn't open." << std::endl;
			return AllContents;
		}
	}

	// If the container is holding anything, examine that too
  if ( Contents.size() == 0 ) 
	{
		if (!silent) std::cout << "	    The " << this->Name << " is empty" << std::endl;
	}
	else
	{
  	if (DEBUG_EXAMINE) std::cout  << "	    " << "The " << this->Name 
			<< " holds " << Contents.size() << " items:" << std::endl;

		if (!silent) std::cout << "	    " << "The " << this->Name << " holds:" << std::endl;
		for (iter=Contents.begin(); iter != Contents.end(); iter++)
		{
			if ( (*iter)->isContainer )
			{
				// If we find a container inside a container, deal with it recursively
				if (!silent) std::cout << "	    ";
				if (verbose && !silent ) std::cout << counter++ << ".  " ;
				if (!silent) std::cout << (*iter)->Name << std::endl;
				AllContents.push_back(*iter);
				subContents = (*iter)->Examine(counter, verbose,silent); // Recursion FTW
				AllContents.insert(AllContents.end(), subContents.begin(),subContents.end());
			}
			else
			{
				// If we find a thing, no recursion
				//std::cout << "	    "<< counter++ << ".  " << (*iter)->Name << std::endl;
				if (!silent) std::cout << "	    ";
				if (verbose && !silent ) std::cout << counter++ << ".  " ;
				if (!silent) std::cout << (*iter)->Name << std::endl;
				AllContents.push_back(*iter);
			}
		}
	}
  if (DEBUG_EXAMINE) std::cout  << "===== end   Container::Examine counter=" 
		<< counter << std::endl;
	return AllContents;
}

/* ***********************************************************
 * Search inside a container for a Thing with a particulare address
 * If found, return a pointer to the holding container;
 * else return NULL
 * ********************************************************* */
Container * Container::FindByPtr(Thing * SeekingThingPtr)
{
	std::list<Thing*>::iterator iter;
	if (DEBUG_FIND) std::cout << "DEBUG: Find: Looking in " << this->Name << std::endl;
	for (iter=Contents.begin(); iter != Contents.end(); iter++)
	{
			if ( (*iter) == SeekingThingPtr ) 
			{
				return this; // The Thing is in this container
			}
			if ( (*iter)->isContainer )
			{
				if (DEBUG_FIND) std::cout << "DEBUG: Recursing FindByPtr into " << (*iter)->Name << std::endl;
				return(((Container*)(*iter))->Container::FindByPtr(SeekingThingPtr));
			}
	}
	return ((Container *)0);
}

/* ***********************************************************
 * Search inside a container for a Thing with a particulare name
 * Return the pointer for that Thing
 * ********************************************************* */
Thing * Container::FindByName(std::string SeekingThingName)
{
	std::list<Thing*>::iterator iter;
	Thing * tmpVal;

	if (DEBUG_FIND) std::cout << "===== begin Container::FindByName, looking in " << this->Name << std::endl;
	for (iter=Contents.begin(); iter != Contents.end(); iter++)
	{
			if ( ((*iter)->Name).compare(SeekingThingName) == 0 ) 
			{
				//return this; // The Thing is in this container
				return (*iter); // Return eht pointer to thing found hting
			}
			if ( (*iter)->isContainer )
			{
				if (DEBUG_FIND) std::cout << "DEBUG: Recursing FindByName into " << (*iter)->Name << std::endl;
				tmpVal = (((Container*)(*iter))->Container::FindByName(SeekingThingName));
				if (tmpVal) return tmpVal;
			}
	}
	if (DEBUG_FIND) std::cout << "===== begin Container::FindByName, looking in " << this->Name << std::endl;
	return ((Thing *)0);
}

/* ***********************************************************
 * GameState functions
 *
 *
 * GameState::Examine is different in that:
 *
 * 	 It returns the GameState in the vector (container examine does not return itself)
 * 	 It doesn't need to handle open tests or OpenFunc functions
 *
 * ********************************************************* */
std::vector<Thing*> GameState::Examine(int &counter, bool verbose, bool silent)
{
	std::vector<Thing *> AllContents;
	std::vector<Thing*> subContents;

  if (DEBUG_FUNCTION) std::cout << "===== begin GameState::Examine" << std::endl;
	if ( !silent && verbose)
		std::cout << "	" << counter ++ << ".  You are carrying a " << this->Name << std::endl;
 	else
		counter++;

	AllContents.push_back(this);
	subContents = Container::Examine (counter, verbose, silent);
	AllContents.insert(AllContents.end(), subContents.begin(),subContents.end());
  if (DEBUG_FUNCTION) std::cout << "===== end GameState::Examine" << std::endl;

	return AllContents;

}

int GameState::getGameTaskStatus()
{
	int points = 0;
	if (DEBUG_FUNCTION) std::cout << "===== begin GameState::getGameTaskStatus" << std::endl;
	/*
	if (GameTask[0])
	{
		std::cout << "Game goal zero is met." << std::endl;
		points ++;
	}
	*/
	return points;
}

int GameState::getAvailableCapacity()
{
	int GameStateIsCarrying = 0;
	int counter = 0;
	std::vector<Thing*> backpackContents;
	std::vector<Thing*>::iterator iterThing;

	backpackContents = this->Examine(counter, false, true);
	for (iterThing=backpackContents.begin(); iterThing != backpackContents.end(); iterThing++)
	{
		if ( (*iterThing) == this) continue; // Don't count the holdall itself
		GameStateIsCarrying += (*iterThing)->getWeight();
	}
	return (this->Capacity - GameStateIsCarrying);
}

