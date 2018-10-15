 /*
 * File name: room.cpp
 *
 * Overview:
 *   General functions that work on rooms.
 *
 * 	 Room::getRoomName Returns the room's name
 *
 * 	 Room::getLongDesc Returns the long description of the room
 *
 * 	 Room::getShortDesc Returns the short description of the room
 *
 * 	 Room::getAdditionalDesc Returns the additional description of the room
 *
 * 	 Room::getLongExitDesc Returns the detailed exits of the room
 *
 * 	 Room::getShortExitDesc Returns the short description of the exits
 *
 * 	 Room::Examine  Print a description of everything in the room
 *
 * 	 Room::addExitsToStack Adds this room's exits (room names) to the given stack
 *
 *		 Room::playerTurn  -- TODO
 *
 *		 Room::Examine  -- TODO
 */

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include "room.hpp"
#include "parser.hpp"
#include "actions.hpp"

using namespace std;

/*
 * Constructor for Room class
 * It opens the room file and parses the lines.
 * Info on files from http://www.cplusplus.com/doc/tutorial/files/ 
 */
Room::Room(string filename)
{
	ifstream roomfile;
	string lineStr, str;
	int roomCount=0;
	numExits = 0;
	Thing * newFeature;

	for (int i=0;i<MAX_RM_CONNECTIONS; i++)
	{
		//TODO: fix this by doing it in the constructor #facepalm
  	Connections[i] = NULL;
	}

	// Iterate through the room file line by line and set Room values
	roomfile.open(filename.c_str());  // .c_str() got from https://stackoverflow.com/questions/19531269/c-void-function-with-file-stream-error
	if (roomfile.is_open())
	{
		// https://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once
		// http://www.cplusplus.com/reference/string/string/substr/
	   while (std::getline(roomfile, lineStr))  
   	{
			if (lineStr.substr(0,1).find("#") != std::string::npos )
			{
				// Skip lines that begin with # for comments
				continue;
			}
   		if(lineStr.find("ROOM NAME: ") != std::string::npos)
   		{
   			roomName = lineStr.substr(11, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("LONG DESC: ") != std::string::npos)
   		{
   			longDesc = lineStr.substr(11, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("SHORT DESC: ") != std::string::npos)
   		{
   			shortDesc = lineStr.substr(12, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("ADITIONAL DESC: ") != std::string::npos)
   		{
   			additionalDesc = lineStr.substr(16, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("LONG EXIT DESC: ") != std::string::npos)
   		{
   			longExitDesc = lineStr.substr(16, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("SHORT EXIT DESC: ") != std::string::npos)
   		{
   			shortExitDesc = lineStr.substr(17, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("FEATURE: ") != std::string::npos)
			{
				//TODO: get features here
				newFeature = getFeature( lineStr.substr(9, lineStr.length()-1));
				if ( newFeature != NULL ) {
					// Add this to the vector of features in the room
					Features.push_back(newFeature);
				}
			}
   		if(lineStr.find("CONNECTION") != std::string::npos)
			{
				// TODO: We dont' need numbers on the CONNECTION lines in the room files, should eliminate those
				if (roomCount < MAX_RM_CONNECTIONS)
				{
				size_t startDirection = lineStr.find_last_of(" ") + 1;
				size_t startName = lineStr.find_last_of("|") + 1;
				Connections[roomCount] = new Doorway();
				Connections[roomCount]->direction = lineStr.substr(startDirection, startName - startDirection - 1);
				Connections[roomCount]->roomName = lineStr.substr(startName, lineStr.length() -1);
				roomCount++;
				}
				else
				{
					// should error ;)
				}
			}
			numExits = roomCount;
   	}
	}
	else
	{
		cout << "Error opening room file '" <<  filename << "'. Exiting...\n";
		exit(1);
	}

	roomSeen = false;
}


/*
 * Destructor for Room class
 * Frees up the memory allocated in the Constructor
 */
Room::~Room()
{
	for (int i=0;i<MAX_RM_CONNECTIONS; i++)
	{
  		if(Connections[i] != NULL)
  		{
	  		delete(Connections[i]);
  		}
  }
}

std::string Room::getRoomName()
{
	return this->roomName;
}

std::string Room::getLongDesc()
{
	return this->longDesc;
}

std::string Room::getShortDesc()
{
	return this->shortDesc;
}

std::string Room::getAdditionalDesc()
{
	return this->additionalDesc;
}

std::string Room::getLongExitDesc()
{
	return this->longExitDesc;
}

std::string Room::getShortExitDesc()
{
	return this->shortExitDesc;
}


Thing * Room::getFeature (std::string featureFileName){
	ifstream thingfile;
	string lineStr, str;
	string featureDir = FEATURE_DIRECTORY;
	Thing * newThing = NULL;
	Thing * tmpThing = NULL;

	thingfile.open( (featureDir.append(featureFileName)).c_str()); 
	if (thingfile.is_open()) {
		while (std::getline(thingfile, lineStr))  {
			if(lineStr.find("NAME: ") != std::string::npos) 
			{
				// Make a new Thing.
				// TODO: test for failure...
				newThing = new Thing(lineStr.substr(6, lineStr.length()-1));
			}
			else if ( newThing == NULL ) {
				continue;
			}

			// Only go past this point if we've found the Name and created the Feature
			// TODO: If weight < something, validVerbs = throw
			//       if food, validVerbs = eat
			//       or just accept a list of VERBS ?
			if(lineStr.find("DESCRIPTION: ") != std::string::npos) 
			{
				newThing->Story = (lineStr.substr(13, lineStr.length()-1));
			}
			else if(lineStr.find("OPEN: ") != std::string::npos) 
			{
				if ( stoi(lineStr.substr(6, lineStr.length()-1)) > 0  ) 
				{ newThing->Open = true;}
				else
				{ newThing->Open = false;}
			}
   		else if(lineStr.find("IS_CONTAINER: ") != std::string::npos)
			{
				if ( stoi(lineStr.substr(14, lineStr.length()-1)) > 0  ) 
				{ 
					newThing->isContainer = true;
					newThing->Verbs.push_back((validVerbs) open);
				}
				else
				{ 
					newThing->isContainer = false;
				}
			}
   		else if(lineStr.find("FEATURE: ") != std::string::npos)
			{
				// recursion :)
				newThing->isContainer = true;
				tmpThing = getFeature( lineStr.substr(9, lineStr.length()-1));
				if ( tmpThing != NULL ) {
					newThing->Contents.push_back(tmpThing);
				}
			}
		}
	}
	else {
		//Print useful error, but don't exit
		cout << "Error opening feature file '" <<  featureDir << "\n";
	}

	return newThing;
	
}

/*
 * TODO: Function info goes here
 */
void Room::Examine()
{
	Doorway * door;
	std::vector<Thing*>::iterator iter;

	std::cout << "\nYou are in the " << getRoomName() << std::endl;
	std::cout << getShortDesc() << std::endl;
	
	for (int r = 0; r < MAX_RM_CONNECTIONS; r++) {
		door = Connections[r];
		if (door != NULL) {
			std::cout << "\t Doorway " << door->Examine() << std::endl;
		}
	}
	for (iter = Features.begin(); iter != Features.end(); iter ++)
	{
		(*iter)->Examine(true,1,0);
	}
}

/*
 * Adds this room's exits to the given stack. 
 */
void Room::addExitsToStack(std::stack<std::string> &exits)
{
	int i;
	for(i=0; i<numExits; i++)
	{
		exits.push(Connections[i]->roomName);
	}

}

/*
 * TODO: Function info goes here
 */
Room * Room::playerTurn(GameState * PlayerState)
{
	if (DEBUG_FUNCTION) std::cout << "===== begin Room::playerTurn" << std::endl;
	// TODO: all the magic
	// PlayerState: What the player is holding and any state of the game
	// this:  The room we're in
	// Doorways : the array of doorways
	// Features : the features, a vector of Things
	//          : Things, with a map of verb->Action
	// And now we do stuff.
	Choice * userChoice;
	Room * nextRoom = this;
	Parser parse;

	userChoice = parse.ParseLine();

	// If verb = go, choice should be a door
	// otherwise look for noun in PlayerState and then in Room
	// If no noun - limited choices
	if (userChoice->Verb == (validVerbs)quit)
	{
		return (Room *) NULL;
	}
	if (userChoice->Verb == (validVerbs)help)
	{
		std::cout << "THERE IS NO HELP FOR YOU" << std::endl;
		return this;
	}
	if( (userChoice->Verb == (validVerbs)save) ||
			(userChoice->Verb == (validVerbs)load) )
	{
		std::cout << "Game does not yet support Save or Load" << std::endl;
		return this;
	}

	if ( userChoice->Noun == "" ) {
		if (userChoice->Verb == (validVerbs)look) 
		{
			Examine(); // Examine this room
		}
		else if (userChoice->Verb == (validVerbs)go) 
		{
			std::cout << "Where do you want to " << userChoice->printVerb()<< "?" << std::endl;
		}
		else if (userChoice->Verb < (validVerbs)LastAction)
		{
			std::cout << "What do you want to " << userChoice->printVerb()<< "?" << std::endl;
		}
	}
	else if (userChoice->Verb == (validVerbs)go) {
		nextRoom = goRoom(userChoice->Noun, PlayerState);
		if ( nextRoom != this ) 
		{
			nextRoom->Examine();
		}
	}

	if (DEBUG_FUNCTION) std::cout << "===== end   Room::playerTurn" << std::endl;

	return nextRoom;

	//Actions * possibleActions; // lists of Things and Doorways that the user can act on
	//possibleActions = this->Examine(); // get Doorways and Things from this Room
	//userChoice = possibleActions->userChooses();
}

Room * Room::goRoom(std::string roomName, GameState * PlayerState){
	Room * nextRoom = this;
	Doorway * door;

	if (DEBUG_FUNCTION) std::cout << "===== begin Room::goRoom" << std::endl;
	for (int r = 0; r < MAX_RM_CONNECTIONS; r++) {
		door = Connections[r];
		if (door != NULL) {
			if (DEBUG_FUNCTION) std::cout << "\t looking at Doorway " << door->Examine() << std::endl;
			if ( roomName.compare(door->roomName) == 0 ) {
				nextRoom = PlayerState->housePtr->getRoomPtr(roomName);
				if ( nextRoom != NULL ) {
					if (DEBUG_FUNCTION) cout << "\tSUCCESS moving to room " << door->roomName << std::endl;
					return nextRoom;
				}
				else 
				{
					cout << "\tERROR mvoing to room " << door->roomName << std::endl;
					nextRoom=this;
				}
			}
		}
	}
	std::cout << "Hm, I don't see a doorway that leads to the " << roomName << "...." << std::endl;
	return this;
}

// Constructor for Doorway class. 
Doorway::Doorway()
{
	std::string roomName = "";
	std::string direction = "";
	
}

// Destructor for Doorway class. 
Doorway::~Doorway()
{

}

std::string Doorway::Examine() {
	std::string ReturnThis =  "" + direction + ":" + roomName;
	return ReturnThis;
}

