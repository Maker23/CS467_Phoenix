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
#include "engine.hpp"
#include "parser.hpp"

using namespace std;

/*
 * Constructor for Room class
 * It opens the room file and parses the lines.
 * Info on files from http://www.cplusplus.com/doc/tutorial/files/ 
 */
//Room::Room(string filename, std::string roomName, std::stack<lockDoorStruct> &doorwayToLock)
Room::Room(string filename, string roomKey, std::stack<lockDoorStruct> &doorwayStack)
{

	ifstream roomfile;
	string lineStr, str;
	int roomCount=0;
	numExits = 0;
	lockDoorStruct lockThisDoor;
	//Feature * newFeature;

	for (int i=0;i<MAX_RM_CONNECTIONS; i++)
	{
	  	Connections[i] = NULL;
	}
	unlockText = "";

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
   		if(lineStr.find("UNLOCKED_TEXT: ") != std::string::npos)
   		{
   			unlockText = lineStr.substr(15, lineStr.length()-1);
				continue;
   		}

   		if(lineStr.find("FEATURE: ") != std::string::npos)
   		{
				str = lineStr.substr(9, lineStr.length()-1);
				if ( str.length() > 0 ) 
				{
					if (DEBUG_FEATURES) std::cout << "Adding feature '" << strToLowercase(str) << "' to room "<< roomName << std::endl;
					// Add this to the vector of features in the room
					roomFeatures.push_back(strToLowercase(str));
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

   		if(lineStr.find("LOCKED_DOOR: ") != std::string::npos)
   		{
				str = lineStr.substr(13, lineStr.length()-1);
				if ( str.length() > 0 ) 
				{
					if (DEBUG_FEATURES) std::cout << "Locking door from " << strToLowercase(roomKey) << " to '" << strToLowercase(str) << "'" << std::endl;
					//lockExitDoorByKey(strToLowercase(str));
					lockThisDoor.doorFrom = strToLowercase(roomKey);
					lockThisDoor.doorTo = strToLowercase(str);
					doorwayStack.push(lockThisDoor);
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

	//if (DEBUG_BRENT) { std::cout << "Number of Features in "<< roomName << ": " << roomFeatures.size() << endl; }

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

std::string Room::getKeyName()
{
	return key;
}

void Room::setKeyName(std::string keyName)
{
	key = strToLowercase(keyName);
}

std::string Room::getRoomName()
{
	return this->roomName;
}

std::string Room::getLongDesc()
{
	LongString LString(this->longDesc);
	return LString.getWrappedText();
}

std::string Room::getShortDesc()
{
	LongString LString(this->shortDesc);
	return LString.getWrappedText();
}

std::string Room::getAdditionalDesc()
{
	LongString LString(this->additionalDesc);
	return LString.getWrappedText();
}

void Room::setRoomSeen()
{
	roomSeen = true;
}

std::string Room::getUnlockText()
{
	return unlockText;
}


/*
 * Searches the Connections (exits) in this room by keyword and if found, will return the room object.
 * returns null if not found.
 */
std::string Room::getExitRoomByKey(std::string searchKey, bool returnLocked=true)
{
	//isExitKeywordFound
	// loop through the connections and check isExitKeywordFound(searchKey)
	// if found, return that object.

	//std::cout << "Enter Room::getExitRoomByKey(" << searchKey << ")" << std::endl;
	if(DEBUG_FUNCTION) std::cout << "[DEBUG_FUNCTION] START Room::getExitRoomByKey" << std::endl;
	for (int r = 0; r < numExits; r++)
	{
		if(Connections[r]->isExitKeywordFound(searchKey))
		{ // it is found, return the name of the room.
			if(returnLocked == true && Connections[r]->isDoorLocked())
			{
				if(DEBUG_FUNCTION) std::cout << "[DEBUG_FUNCTION] END Room::getExitRoomByKey  return locked" << std::endl;
				return "locked";
			}
			else
			{
				if(DEBUG_FUNCTION) std::cout << "[DEBUG_FUNCTION] END Room::getExitRoomByKey  return: " << Connections[r]->getExitRoomName() << std::endl;
				return Connections[r]->getExitRoomName();
			}
		}
	}
	//std::cout << "Exit Room::getExitRoomByKey(" << searchKey << ")" << std::endl;
	if(DEBUG_FUNCTION) std::cout << "[DEBUG_FUNCTION] END Room::getExitRoomByKey  return empty string" << std::endl;
	return "";
}

bool Room::isExitDoorLockedByKey(std::string searchKey)
{
	for (int r = 0; r < numExits; r++)
	{
		if(Connections[r]->isExitKeywordFound(searchKey))
		{
			return Connections[r]->isDoorLocked();
		}
	}

	return false;
}

bool Room::lockExitDoorByKey(std::string searchKey)
{
	if (DEBUG_LOCK) {
		std::cout << "[DEBUG_LOCK] Room::lockExitDoorByKey. Room: " << this->roomName << ",searchKey: " << searchKey << std::endl;
	
	}
	for (int r = 0; r < numExits; r++)
	{
		if (DEBUG_LOCK) std::cout << "     searchKey: " << searchKey << " is exit found: " << Connections[r]->isExitKeywordFound(searchKey) << std::endl;
		if(Connections[r]->isExitKeywordFound(searchKey))
		{ // it is found
			if (DEBUG_LOCK) std::cout << "     keyword found at Connections[" << r << "]" << std::endl;
			Connections[r]->lockDoor();
			if (DEBUG_LOCK)
			{
				std::cout << "     isDoorLocked: " << Connections[r]->isDoorLocked() << std::endl;
				std::cout << "     return true from Room::lockExitDoorByKey" << std::endl;
			}
			return true;
		}
	}
	if (DEBUG_LOCK)	std::cout << "     return true from Room::lockExitDoorByKey" << std::endl;
	return false;
}

bool Room::unlockExitDoorByKey(std::string searchKey)
{
	if (DEBUG_LOCK) std::cout << "[DEBUG_LOCK] Room::unlockExitDoorByKey searchKey: " << searchKey << std::endl;
	for (int r = 0; r < numExits; r++)
	{
		if(Connections[r]->isExitKeywordFound(searchKey))
		{ // it is found
			if (DEBUG_LOCK) std::cout << "  Found connection '" << Connections[r]->getDisplayName() << ", unlocking Door" << std::endl;
			Connections[r]->unlockDoor();
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
	//Doorway * door;
	//Feature * feature;
	std::vector<std::string>::iterator iter;


	std::cout << "\nYou are in the " << getRoomName() << std::endl;
	if ( roomSeen ) {
		std::cout << getShortDesc() << std::endl;
	}
	else {
		std::cout << getLongDesc() << std::endl;
	}
	
	GS->housePtr->printRoomFeatures(this);

	// TODO:? Move this to a command word exits and not all the time?
	// TODO:? Show room name once player has seen that room?
	if(roomSeen)
		std::cout << "Room exits: " << this->getExitsForDisplay() << std::endl;
	else
		roomSeen = true;
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

Room * Room::getRoomOtherSideOfDoor(std::string roomName, GameState * PlayerState)
{
	std::string exitStringReturned;
	Room *roomPtr = this;
	//if (DEBUG_BRENT) std::cout << "[DEBUG_BRENT] Room::getRoomOtherSideOfDoor roomName: " << roomName << std::endl;

	if (DEBUG_FUNCTION) std::cout << "===== begin Room::getRoomOtherSideOfDoor" << std::endl;
	exitStringReturned = roomPtr->getExitRoomByKey(roomName, false);
	//if (DEBUG_BRENT) std::cout << "[DEBUG_BRENT]        exitStringReturned: " << exitStringReturned << std::endl;
	
	roomPtr = PlayerState->housePtr->getRoomPtr(exitStringReturned);
	if(roomPtr != NULL)
	{
		if (DEBUG_FUNCTION) std::cout << "   ===== Return room pointer." << std::endl;
		return roomPtr;
	}
	else
		return NULL;


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
	//if (DEBUG_BRENT) std::cout << "# of Features in Room: " << Features.size() << std::endl;
	std::cout << "Exits: " << getExitsForDisplay() << std::endl;
}

std::vector<std::string> Room::getFeaturesVector()
{
	return roomFeatures;
}

void Room::addFeature(std::string FName, GameState *GS) 
{
	// TODO: Check that FName is in the houseMap
	//
	Parser parse(GS);
	std::string realName = parse.getNoun(FName);

	if (DEBUG_FEATURES) std::cout << "===== begin Room::addFeature, realName= " << realName << std::endl;
	roomFeatures.push_back(realName);
	return;
}

void Room::deleteFeature(std::string featureToDelete) 
{
	// refactored this to use keyname.
	//Parser parse;
	//std::string realName = parse.getNoun(FName);
	//if (DEBUG_BRENT) std::cout << "[DEBUG_BRENT] deleteFeature() FName: " << featureToDelete << std::endl;

	for ( std::vector<std::string>::iterator iter = roomFeatures.begin(); iter != roomFeatures.end(); iter++)
	{
		//if (DEBUG_BRENT) std::cout << "[DEBUG_BRENT]    deleteFeature() (*iter): " << (*iter) << std::endl;
		if ((*iter).compare(featureToDelete) == 0) 
		{
			roomFeatures.erase(iter);
			return;
		}
	}

  // TODO: Could also try FName if 'realName' doesn't work
	std::cout << "WARNING: could not find feature "<<featureToDelete << " to delete from room " << std::endl;
	return;
}

/* **********************************************************
 * Returns the feature pointer if the feature is in  the room,
 * otherwise returns NULL
 *
 * ******************************************************** */
Feature * Room::getFeaturePtr(std::string FeatureName, GameState * GS)
{
	Feature * feature;
	std::vector<std::string>::iterator iter;

	for (iter = roomFeatures.begin(); iter != roomFeatures.end(); iter ++)
	{
		if (DEBUG_FUNCTION) std::cout << " ++++ getFeature found: " << *iter << std::endl;
		feature = GS->housePtr->getFeaturePtr(*iter);
		if ( feature ) return feature;
	}

	return NULL;
}

bool Room::isFeatureInThisRoom(std::string searchFor)
{
	if (DEBUG_FUNCTION) std::cout << "[DEBUG_FUNCTION Room:isFeatureInThisRoom] Begins" << std::endl;
	std::vector<std::string>::iterator iter;
	for (iter = roomFeatures.begin(); iter != roomFeatures.end(); iter ++)
	{
		if(*iter == searchFor)
		{
			if (DEBUG_FUNCTION) std::cout << "[DEBUG_FUNCTION Room:isFeatureInThisRoom] Ends - Return True" << std::endl;
			return true;
		}
	}

	if (DEBUG_FUNCTION) std::cout << "[DEBUG_FUNCTION Room:isFeatureInThisRoom] Ends - Return False" << std::endl;
	return false;

}



// returns lowercase string
// http://www.cplusplus.com/reference/locale/tolower/
std::string Room::strToLowercase(std::string mixedStr)
{
	std::locale loc;

  for (std::string::size_type i=0; i<mixedStr.length(); ++i)
    mixedStr[i] = std::tolower(mixedStr[i],loc);
	return mixedStr;
}


Feature * Room::findFeatureByUnlocksString(std::string searchString, GameState *GS)
{
	Feature *feature = NULL;
	if (DEBUG_FUNCTION || DEBUG_LOCK) std::cout << "[DEBUG_FUNCTION Room:findFeatureByUnlocksString] Begins" << std::endl;
	std::vector<std::string>::iterator iter;
	for (iter = roomFeatures.begin(); iter != roomFeatures.end(); iter ++)
	{
		if (DEBUG_LOCK) std::cout << "     looking at " << (*iter) << std::endl;
		feature = GS->housePtr->getFeaturePtr((*iter));
		if (DEBUG_LOCK) std::cout << "     found " << feature->getKeyName() << std::endl;
		if(feature->getStringByKey("unlocks").compare(strToLowercase(searchString)) == 0)
		{
			if (DEBUG_LOCK) std::cout << "     [DEBUG_LOCK][Room] found " << searchString << " in " << feature->getStringByKey("name") << std::endl;
			return feature;
		}
		else {
			if (DEBUG_LOCK) std::cout << "     [DEBUG_LOCK][Room] did not find search string " << searchString << " in feature " << feature->getStringByKey("name") << std::endl;
		}
	}
	for (std::vector<Feature*>::iterator it = GS->Holding.begin(); it != GS->Holding.end(); it ++)
	{
		feature = (*it);
		if(feature->getStringByKey("unlocks").compare(strToLowercase(searchString)) == 0)
		{
			if (DEBUG_LOCK) std::cout << "     [DEBUG_LOCK][Holding] found " << searchString << " in " << feature->getStringByKey("name") << std::endl;
			return feature;
		}
		else {
			if (DEBUG_LOCK) std::cout << "     [DEBUG_LOCK][Holding] did not find search string " << searchString << " in feature " << feature->getStringByKey("name") << std::endl;
		}
	}

	if (DEBUG_FUNCTION || DEBUG_LOCK) std::cout << "[DEBUG_FUNCTION Room:findFeatureByUnlocksString] Ends - Return False" << std::endl;
	return NULL;
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
