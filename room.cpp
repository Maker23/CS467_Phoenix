#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include "room.hpp"
#include "container.hpp"
#include "actions.hpp"

using namespace std;

// Constructor for Room class. 
// It opens the room file and parses the lines.
// Info on files from http://www.cplusplus.com/doc/tutorial/files/ 
Room::Room(string filename)
{
	ifstream roomfile;
	string lineStr, str;
	int roomCount=0;
	numExits = 0;

	for (int i=0;i<MAX_RM_CONNECTIONS; i++)
	{
  	Connections[i] = NULL;
	}

	// Iterate through the room file and set values
	roomfile.open(filename.c_str());  // .c_str() got from https://stackoverflow.com/questions/19531269/c-void-function-with-file-stream-error
	if (roomfile.is_open())
	{
		// https://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once
		// http://www.cplusplus.com/reference/string/string/substr/
	   while (std::getline(roomfile, lineStr))  
   	{
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
   		if(lineStr.find("CONNECTION") != std::string::npos)
			{
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

Room::~Room()
{
	for (int i=0;i<MAX_RM_CONNECTIONS; i++)
	{
  		//Connections[i] = NULL;
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

Actions * Room::Examine (){
	if (DEBUG_FUNCTION) std::cout << "===== begin Room::Examine" << std::endl;

	Actions * returnActions = new Actions();
	Doorway * tmpDoor;
	std::vector<Thing*> thingList;
	std::vector<Doorway*> doorList;

  /* Right now we're ignoring Things since Room-Things haven't been implemented */

	for (int r=0; r < MAX_RM_CONNECTIONS; r++){
		tmpDoor = this->Connections[r];
		if ( tmpDoor != NULL ) {
			if (DEBUG_EXAMINE) std::cout << "found doorway " << tmpDoor->roomName << "; adding to doorList" << std::endl;
			//doorList.push_back(tmpDoor);
			returnActions->Doors.push_back(tmpDoor);
		}
	}

	//returnActions->Doors = doorList;
	returnActions->Things = thingList;
	if (DEBUG_FUNCTION) std::cout << "===== end Room::Examine" << std::endl;

	return returnActions;
	

}

void Room::getExists(std::stack<std::string> &exits)
{
	int i;
	for(i=0; i<numExits; i++)
	{
		exits.push(Connections[i]->roomName);
	}

}

Room * Room::userAction(GameState * PlayerState)
{
	if (DEBUG_FUNCTION) std::cout << "===== begin Room::userAction" << std::endl;
	// TODO: all the magic
	
	Choice * userChoice;
	Room * nextRoom = this;
	Actions * possibleActions; // lists of Things and Doorways that the user can act on

	possibleActions = this->Examine(); // get Doorways and Things from this Room

	userChoice = possibleActions->userChooses();
	
	// And now we do stuff.
	
	return nextRoom;
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
