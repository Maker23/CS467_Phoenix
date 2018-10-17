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
	Feature * newFeature;

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
				newFeature = getFeature( lineStr.substr(9, lineStr.length()-1));
				if ( newFeature != NULL ) {
					// Add this to the vector of features in the room
					Features.push_back(newFeature);
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

			/*
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
			*/
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



Feature * Room::getFeature (std::string featureFileName){
	ifstream featurefile;
	string lineStr, str;
	string featureDir = FEATURE_DIRECTORY;
	Feature * newFeature = NULL;
	Feature * tmpFeature = NULL;

	featurefile.open( (featureDir.append(featureFileName)).c_str()); 
	if (featurefile.is_open()) {
		while (std::getline(featurefile, lineStr))  {
			if(lineStr.find("NAME: ") != std::string::npos) 
			{
				// Make a new Feature.
				// TODO: test for failure...
				newFeature = new Feature(lineStr.substr(6, lineStr.length()-1));
			}
			else if ( newFeature == NULL ) {
				continue;
			}

			// Only go past this point if we've found the Name and created the Feature
			// TODO: If weight < something, validVerbs = throw
			//       if food, validVerbs = eat
			//       or just accept a list of VERBS ?
			if(lineStr.find("DESCRIPTION: ") != std::string::npos) 
			{
				newFeature->Story = (lineStr.substr(13, lineStr.length()-1));
			}
			else if(lineStr.find("OPEN: ") != std::string::npos) 
			{
				if ( stoi(lineStr.substr(6, lineStr.length()-1)) > 0  ) 
				{ newFeature->Open = true;}
				else
				{ newFeature->Open = false;}
			}
   		else if(lineStr.find("IS_CONTAINER: ") != std::string::npos)
			{
				if ( stoi(lineStr.substr(14, lineStr.length()-1)) > 0  ) 
				{ 
					newFeature->isContainer = true;
					newFeature->Verbs.push_back((validVerbs) open);
				}
				else
				{ 
					newFeature->isContainer = false;
				}
			}
   		else if(lineStr.find("FEATURE: ") != std::string::npos)
			{
				// recursion :)
				newFeature->isContainer = true;
				tmpFeature = getFeature( lineStr.substr(9, lineStr.length()-1));
				if ( tmpFeature != NULL ) {
					newFeature->Contents.push_back(tmpFeature);
				}
			}
		}
	}
	else {
		//Print useful error, but don't exit
		cout << "Error opening feature file '" <<  featureDir << "\n";
	}

	return newFeature;
	
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
			return Connections[r]->getExitRoomName();
		}
	}
	//std::cout << "Exit Room::getExitRoomByKey(" << searchKey << ")" << std::endl;
	return "";
}

/*
 * TODO: Function info goes here
 */
void Room::Examine()
{
	Doorway * door;
	std::vector<Feature*>::iterator iter;

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
		exits.push(Connections[i]->getExitRoomName());
	}

}

Room * Room::goRoom(std::string roomName, GameState * PlayerState){
	Room * nextRoom = this;
	Doorway * door;

	if (DEBUG_FUNCTION) std::cout << "===== begin Room::goRoom" << std::endl;
	for (int r = 0; r < MAX_RM_CONNECTIONS; r++) {
		door = Connections[r];
		if (door != NULL) {
			if (DEBUG_FUNCTION) std::cout << "\t looking at Doorway " << door->Examine() << std::endl;
			if ( roomName.compare(door->getDisplayName()) == 0 ) {
				nextRoom = PlayerState->housePtr->getRoomPtr(roomName);
				if ( nextRoom != NULL ) {
					if (DEBUG_FUNCTION) cout << "\tSUCCESS moving to room " << door->getDisplayName() << std::endl;
					return nextRoom;
				}
				else 
				{
					cout << "\tERROR mvoing to room " << door->getDisplayName() << std::endl;
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

   while(getline(mystream,tempStr,',')){  // https://stackoverflow.com/questions/40611689/c-error-in-tokenizer-variable-stdstringstream-mystream-has-initializer-b/43017562
       keyWords.push_back(tempStr);
   }
}

std::string Doorway::Examine() {
	std::string ReturnThis =  "" + displayName;
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