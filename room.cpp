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
 */

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <sstream>
#include <algorithm>
#include "room.hpp"
#include "parser.hpp"

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
	//Feature * newFeature;

	for (int i=0;i<MAX_RM_CONNECTIONS; i++)
	{
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

   		if(lineStr.find("FEATURE: ") != std::string::npos)
   		{
				//TODO: get features here
				str = lineStr.substr(9, lineStr.length()-1);
				if ( str.length() > 0 ) 
				{
					if (DEBUG_FEATURES) std::cout << "Adding feature '" << str << "' to room" << std::endl;
					// Add this to the vector of features in the room
					roomFeatures.push_back(str);
				}
			}

			if(lineStr.find("CONNECTION") != std::string::npos)
			{
				Connections[numExits] = new Doorway();

				size_t startOfConnectionInfo = lineStr.find_last_of(":") + 1;
				string connectionInfo = lineStr.substr(startOfConnectionInfo, lineStr.length() - 1);
				Connections[numExits]->setDoorway(connectionInfo);
				roomCount++;
			}

			numExits = roomCount;
   	}
	}
	else
	{
		cout << "Error opening room file '" <<  filename << "'. Exiting...\n";
		exit(1);
	}

	if (DEBUG_BRENT) { std::cout << "Number of Features in "<< roomName << ": " << roomFeatures.size() << endl; }

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

void Room::setRoomSeen()
{
	roomSeen = true;
}


/*
 * Searches the Connections (exits) in this room by keyword and if found, will return the room object.
 * returns null if not found.
 */
std::string Room::getExitRoomByKey(std::string searchKey)
{
	//isExitKeywordFound
	// loop through the connections and check isExitKeywordFound(searchKey)
	// if found, return that object.

	//std::cout << "Enter Room::getExitRoomByKey(" << searchKey << ")" << std::endl;
	for (int r = 0; r < numExits; r++)
	{
		if(Connections[r]->isExitKeywordFound(searchKey))
		{ // it is found, return the name of the room.
			if(Connections[r]->isDoorLocked())
				return "locked";
			else
				return Connections[r]->getExitRoomName();
		}
	}
	//std::cout << "Exit Room::getExitRoomByKey(" << searchKey << ")" << std::endl;
	return "";
}


bool Room::lockExitDoorByKey(std::string searchKey)
{
	for (int r = 0; r < numExits; r++)
	{
		if(Connections[r]->isExitKeywordFound(searchKey))
		{ // it is found, return the name of the room.
			Connections[r]->lockDoor();
			return true;
		}
	}
	return false;
}


/*
 * TODO: Function info goes here
 */
void Room::Examine(GameState * GS)
{
	Doorway * door;
	Feature * feature;
	std::vector<std::string>::iterator iter;


	std::cout << "\nYou are in the " << getRoomName() << std::endl;
	if ( roomSeen ) {
		std::cout << getShortDesc() << std::endl;
	}
	else {
		std::cout << getLongDesc() << std::endl;
		roomSeen=true;
	}
	
	for (int r = 0; r < MAX_RM_CONNECTIONS; r++) {
		door = Connections[r];
		if (door != NULL) {
			std::cout << "\t Doorway " << door->Examine() << std::endl;
		}
	}
	for (iter = roomFeatures.begin(); iter != roomFeatures.end(); iter ++)
	{
		std::cout << "You see a " << *iter << std::endl;
		feature = GS->housePtr->getFeaturePtr(*iter);
		if ( feature ) feature->Examine(GS);
	}
}

/*
 * Adds this room's exits to the given stack. 
 */
void Room::addExitsToStack(std::stack<std::string> &exits)
{
	for(int i=0; i<numExits; i++)
	{
		exits.push(Connections[i]->getExitRoomName());
	}

}

std::string Room::getExitsForDisplay()
{
	std::string exitString;
	for(int i=0; i<numExits; i++)
	{
		exitString.append(Connections[i]->getDisplayName());
		if(i < numExits-1)
			exitString.append(", ");
	}

	return exitString;
}

Room * Room::goRoom(std::string roomName, GameState * PlayerState){
	std::string exitStringReturned;
	Room *roomPtr = this;

	if (DEBUG_FUNCTION) std::cout << "===== begin Room::goRoom" << std::endl;

	// getExitRoomByKey returns locked if the door can't open, empty string if not found, or the string key of the doorway
	exitStringReturned = roomPtr->getExitRoomByKey(roomName);

	if(exitStringReturned.compare("locked") == 0)
	{
		if (DEBUG_FUNCTION) std::cout << "===== exitStringReturned.compare(\"locked\") returned locked." << std::endl;
		std::cout << "Door won't open." << std::endl;
		return this;
	}
	else if(exitStringReturned.length() > 0)
	{
		if (DEBUG_FUNCTION) std::cout << "===== Get the room pointer of the room we want." << std::endl;
  		roomPtr = PlayerState->housePtr->getRoomPtr(exitStringReturned);
  		if(roomPtr != NULL)
  		{
  			if (DEBUG_FUNCTION) std::cout << "===== Return room pointer." << std::endl;
  			return roomPtr;
  		}
	}

	if (DEBUG_FUNCTION) std::cout << "===== Did not return already, so something was not found." << std::endl;
	std::cout << roomName << "? Hm, I don't see a doorway that leads that way." << std::endl;
	return this;


/*
	Room * nextRoom = this;
	Doorway * door;

	if (DEBUG_FUNCTION) std::cout << "===== begin Room::goRoom" << std::endl;
	for (int r = 0; r < MAX_RM_CONNECTIONS; r++) {
		door = Connections[r];
		if (door != NULL) {
			if (DEBUG_FUNCTION) std::cout << "\t looking at Doorway " << door->Examine() << std::endl;
			if (( roomName.compare(door->getDisplayName()) == 0 ) ||
					( roomName.compare(door->getExitRoomName()) == 0 ) )
			{
				nextRoom = PlayerState->housePtr->getRoomPtr(roomName);
				if ( nextRoom != NULL ) {
					if (DEBUG_FUNCTION) cout << "\tSUCCESS moving to room " << door->getDisplayName() << std::endl;
					return nextRoom;
				}
				else 
				{
					cout << "\tERROR moving to room " << door->getDisplayName() << std::endl;
					nextRoom=this;
				}
			}
		}
	}
	std::cout << "Hm, I don't see a doorway that leads to the " << roomName << "...." << std::endl;
	return this;
*/	
}

// prints the room.
// TODO: game engine should correctly word wrap in the terminal.
void Room::displayRoom()
{
	std::cout << std::endl;
	if(roomSeen)
		std::cout << shortDesc << std::endl;
	else
		std::cout << longDesc << std::endl;

	std::cout << additionalDesc << std::endl;

	// this we may want to only show if user asks for the exits.
	if (DEBUG_ROOM) std::cout << "# of Features in Room: " << Features.size() << std::endl;
	std::cout << "Exits: " << getExitsForDisplay() << std::endl;
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

void Doorway::setDoorway(std::string connectionString)
{
	std::string keywordStr, tempStr;

	std::size_t startDisplayName = connectionString.find_last_of(":") + 1;
	std::size_t startRoomTo = connectionString.find_first_of("|") + 1;
	std::size_t startKeywords = connectionString.find_last_of("|") + 1;

	displayName = connectionString.substr(startDisplayName, startRoomTo - startDisplayName - 1);
	goesTo = connectionString.substr(startRoomTo, startKeywords - startRoomTo - 1);
	keywordStr = connectionString.substr(startKeywords, connectionString.length() - 1);

   std::stringstream mystream (keywordStr);

   while(getline(mystream,tempStr,','))
   {  // https://stackoverflow.com/questions/40611689/c-error-in-tokenizer-variable-stdstringstream-mystream-has-initializer-b/43017562
   		// remove leading and trailing spaces
    		while(tempStr[0] == ' ')
    		{
    			tempStr = tempStr.substr(1, tempStr.length() - 1);
    		}
    		while(tempStr[tempStr.length() - 1] == ' ')
    		{
    			tempStr = tempStr.substr(0, tempStr.length() - 2);
    		}
       keyWords.push_back(tempStr);
   }
   doorLocked = false;
}

std::string Doorway::Examine() {
	std::string ReturnThis =  "" + goesTo;
	return ReturnThis;
}


std::string Doorway::getDisplayName()
{
	return displayName;
}

std::string Doorway::getExitRoomName()
{
	return goesTo;
}

bool Doorway::isExitKeywordFound(std::string searchFor)
{

	std::string lcSearchFor = strToLowercase(searchFor);

	for (unsigned int i=0; i<keyWords.size(); i++)
	{
		std::string key = strToLowercase(keyWords[i]);
		if (lcSearchFor.compare(key) == 0)
		{
			return true;
		}
	}
	return false;
}

// returns lowercase string
// http://www.cplusplus.com/reference/locale/tolower/
std::string Doorway::strToLowercase(std::string mixedStr)
{
	std::locale loc;

  for (std::string::size_type i=0; i<mixedStr.length(); ++i)
    mixedStr[i] = std::tolower(mixedStr[i],loc);
	return mixedStr;
}

void Doorway::lockDoor()
{
	doorLocked = true;
}

void Doorway::unlockDoor()
{
	doorLocked = false;
}

bool Doorway::isDoorLocked()
{
	return doorLocked;
}
