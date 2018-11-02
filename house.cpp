 /* File name: house.cpp
 *
 * Overview:
 *   General functions that work on the house.
 *
 * 	 House::buildHouse reads all the room data files and stores Room objects in houseMap. 
 *				  Returns starting Room pointer.
 *
 *		 House::hasRoom receives string of room name, and returns true or false if it's in the houseMap
 *
 *		 House::getRoomPtr returns the Room object pointer of the requested Room. NULL if not found.
 *
 *		 House::printRooms is for debugging, it prints all the rooms currently loaded in the house.
 */

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <stack>
#include <vector>
#include <typeindex>
#include <sys/types.h>
#include <dirent.h>
#include "utilities.hpp"
#include "house.hpp"
#include "feature.hpp"
#include "room.hpp"
#include "engine.hpp"

using namespace std;

/*
 * Constructor for House. Does nothing at all. 
 * Need to call buildHouse seperately.  
 */
House::House()
{

}


/*
 * Destructor -- Deletes the memory allocated in buildHouse() 
 * and clears the map.  
 */
House::~House()
{
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) {
		delete(it->second);
	}
	houseMap.clear();
}

/*
 * Function that builds the entire house and returns a pointer to the
 * starting room.  
 * Takes starting room name.
 * Returns pointer of starting room.
 */
Room *House::buildHouse(string startingRoom){  
	stack<string> roomsToLoad;
	string roomName;
	Room *startingRoomPtr=NULL;
	Room *roomPtr=NULL;

	// Push the startingRoom to the stack
	roomsToLoad.push(startingRoom);

	// Pull from the stack. If it's not already in the houseMap, load the data file, create
	// the room object, add the new room's connections to the stack, and add to houseMap.
	while (!roomsToLoad.empty())
	{
		roomName=roomsToLoad.top();
		roomsToLoad.pop();
		if(!hasRoom(roomName))
		{
			string str;
			str.append(ROOM_DIRECTORY);
			str.append(roomName);
			roomPtr = new Room(str);
			if (startingRoomPtr == NULL)
			{
				startingRoomPtr = roomPtr;  // sets the starting room pointer for the return
			}
			roomPtr->addExitsToStack(roomsToLoad);
			houseMap[strToLowercase(roomPtr->getRoomName())] = roomPtr;
		}
	}

	 string featuresFolder = "features";
	 string FeatureFileToOpen;
    DIR* dirp = opendir(featuresFolder.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
    	if (dp->d_name[0] != '.')
    	{
    		std::string fileName(dp->d_name);
    		FeatureFileToOpen = "features/" + fileName;
    		houseFeatures[strToLowercase(fileName)] = new Feature(FeatureFileToOpen);
    		houseFeatures[strToLowercase(fileName)]->setKeyName(fileName);
    	}

    }
    closedir(dirp);

	if ( DEBUG_FEATURES ) 
      for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++) {
	   {
	   	std::cout << it->second->getName() << std::endl;;
	   }
	}
    
    if ( DEBUG_FEATURES ) 
    {
    	cout << "Number of Features in House: " << houseFeatures.size() << endl;
    }



	return  startingRoomPtr;
}


/*
 * Check if the room is already in the map.
 * Takes starting room name.
 * Returns true if the room is in the houseMap, false otherwise.
 */
bool House::hasRoom(string key)
{
	// map syntax learned from: 
	//    https://stackoverflow.com/questions/1939953/how-to-find-if-a-given-key-exists-in-a-c-stdmap
	if ( houseMap.find(strToLowercase(key)) == houseMap.end() )
	{
      return false;
   }
	else
	{
		return true;
   }
}


/*
 * Returns the Room pointer from the given room name 
 */
Room * House::getRoomPtr(string roomName)
{
	if(hasRoom(strToLowercase(roomName)))
	{
		return houseMap[strToLowercase(roomName)];
	}
	else
	{
		return NULL;
	}
}

/*
 * Prints all the rooms found in the map.
 * Used for debugging. 
 */
void House::printRooms(GameState * GS)
{
	// Print all the keys in the houseMap
	if ( ! DEBUG_EXAMINE ) return;
	std::cout << "--- House rooms: ---" << std::endl;
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) {
		std::cout << "\t" << it->first << std::endl;
		it->second->Examine(GS);
	}
	return;
}
void House::printFeatures(GameState * GS)
{
	// Print all the keys in the houseMap
	if ( ! DEBUG_EXAMINE ) return;
	std::cout << "--- House features: ---" << std::endl;
	for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++) {
		std::cout << "\t" << it->first << std::endl;
		//it->second->Examine(GS);
	}
	return;
}

bool House::hasFeature(string key)
{
	// map syntax learned from: 
	//    https://stackoverflow.com/questions/1939953/how-to-find-if-a-given-key-exists-in-a-c-stdmap
	if ( houseFeatures.find(key) == houseFeatures.end() )
	{
      return false;
   }
	else
	{
		return true;
   }
}


Feature * House::getFeaturePtr(string featureName)
{
	if ( DEBUG_FEATURES ) std::cout << "----- begin House::getFeaturePtr(), looking for '"<< featureName<<"'" << std::endl;
	if(hasFeature(strToLowercase(featureName)))
	{
		if ( DEBUG_FEATURES ) std::cout << "      returning " << featureName << std::endl;
		return houseFeatures[strToLowercase(featureName)];
	}
	else
	{
		if ( DEBUG_FEATURES ) std::cout << "      returning NULL"<<  std::endl;
		// We need this error only when we're loading the house / Rooms. 
	  //std::cout << "WARNING: Failed to find FeaturePtr for " << featureName << "; game may not work properly" << std::endl;	
		return NULL;
	}
}


void House::printRoomFeatures(Room *room)
{
	std::vector<std::string> roomFeatures = room->getFeaturesVector();
	Feature *f1, *f2;

	if (DEBUG_BRENT) std::cout << "Start House::printFeatures()" << std::endl;
	for (std::vector<std::string>::iterator it = roomFeatures.begin() ; it != roomFeatures.end(); ++it)
	{
   	if (DEBUG_BRENT) std::cout << (*it) << std::endl;
   	f1 = getFeaturePtr((*it));

		if (f1==NULL) std::cout << "Error getting pointer for " << (*it) << std::endl;

   	if(f1->getDependsOn().size() == 0)
   	{
   		std::cout << f1->getWalkingInRoomText() << std::endl;
   	}
   	else
   	{
   		f2 = getFeaturePtr(f1->getDependsOn());
   		if (DEBUG_BRENT) std::cout << "DEBUG: " << f1->getName() << " depends on: " << f2->getName() << " solved: " << f2->isSolved() << std::endl;
   		if(f2->isSolved())
   		{
   			std::cout << f1->getWalkingInRoomText() << std::endl;
   		}
   	}
	}
	if (DEBUG_BRENT) std::cout << "Exit House::printFeatures()" << std::endl;
}

// returns lowercase string
// http://www.cplusplus.com/reference/locale/tolower/
std::string House::strToLowercase(std::string mixedStr)
{
	std::locale loc;

  for (std::string::size_type i=0; i<mixedStr.length(); ++i)
    mixedStr[i] = std::tolower(mixedStr[i],loc);
	return mixedStr;
}

void House::debugHouse()
{
	if ( ! DEBUG_HOUSE ) return;

	std::cout << "--- House Debug: ---" << std::endl;
	std::cout << "--- rooms: ---" << std::endl;
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) {
		std::cout << "\t" << it->first << "\t" << it->second->getRoomName() << std::endl;
	}

	std::cout << "--- features: ---" << std::endl;
	for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++) {
		std::cout << "\t" << it->first << "\t" << it->second->getName() << std::endl;
		it->second->printFeaturesActions();
	}
}