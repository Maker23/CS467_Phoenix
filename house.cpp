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
House::House(std::string DirName)
{
	fileDirectory = DirName;
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

	for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++) {
		delete(it->second);
	}

	houseMap.clear();
	houseFeatures.clear();
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
	stack<lockDoorStruct> doorsToLock;
	lockDoorStruct lockThisDoor;

	firstRoomToLoad = startingRoom;

	if(DEBUG_HOUSE) std::cout << "[DEBUG_HOUSE] begin House::buildHouse, starting room='" << startingRoom << "'" << std::endl;
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
			str.append(fileDirectory);
			str.append(ROOM_DIRECTORY);
			str.append(roomName);
			//roomPtr = new Room(str, roomName, doorsToLock);
			roomPtr = new Room(str, roomName, doorsToLock);
			if (startingRoomPtr == NULL)
			{
				startingRoomPtr = roomPtr;  // sets the starting room pointer for the return
			}
			roomPtr->addExitsToStack(roomsToLoad);
			roomPtr->setKeyName(strToLowercase(roomPtr->getRoomName()));
			houseMap[strToLowercase(roomPtr->getRoomName())] = roomPtr;
		}
	}

	if(DEBUG_HOUSE) printRooms();
	// lock all the doors.
	if(DEBUG_HOUSE) std::cout << "[DEBUG_HOUSE] Lockup the " << doorsToLock.size()  << " doors in House" << std::endl;
	while (!doorsToLock.empty())
	{
		lockThisDoor = doorsToLock.top();
		doorsToLock.pop();
		roomPtr = getRoomPtr(lockThisDoor.doorFrom);
		//if(DEBUG_BRENT) std::cout << "          Lockup from  " << lockThisDoor.doorFrom  << " to " << lockThisDoor.doorTo << std::endl;
		roomPtr->lockExitDoorByKey(lockThisDoor.doorTo);
	}

	 string featuresFolder = fileDirectory;
	 featuresFolder.append(FEATURE_DIRECTORY);
	 string FeatureFileToOpen;
    DIR* dirp = opendir(featuresFolder.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
    	if (dp->d_name[0] != '.')
    	{
    		std::string fileName(dp->d_name);
    		FeatureFileToOpen = featuresFolder + fileName;
    		houseFeatures[strToLowercase(fileName)] = new Feature(FeatureFileToOpen);
    		houseFeatures[strToLowercase(fileName)]->setKeyName(fileName);
    	}

    }
    closedir(dirp);
		// Now add aliases for all features
    for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++)
	  {
      for (auto iter=it->second->aliases.begin(); iter != it->second->aliases.end(); iter++) {
				if ( DEBUG_FEATURES ) {
					std::cout << "setting alias " << iter->first << " for cname " << it->first << std::endl;
				}
				houseFeatureAliases[iter->first] = it->first;
			}
		}

	if ( DEBUG_FEATURES ) 
     for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++) {
	   {
	   	std::cout << it->first << " -- " << it->second->getName();
      for (auto iter=it->second->aliases.begin(); iter != it->second->aliases.end(); iter++) {
			 	std::cout << ", alias: " << iter->first << ", ";
			}
			std::cout << std::endl;
	   }
	}
    
    if ( DEBUG_FEATURES ) 
    {
    	cout << "Number of Features in House: " << houseFeatures.size() << endl;

    }
	return  startingRoomPtr;
}

void House::reloadHouse()
{
	// delete the rooms from memory and erase from the map
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) 
	{
		delete(it->second);
	}
	houseMap.clear();

	// delete houseFeatures
	for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++) 
	{
		delete(it->second);
	}
	houseFeatures.clear();
	houseFeatureAliases.clear(); 

	buildHouse(firstRoomToLoad);
	// unlock all doors, the save file will lock the ones that need locking
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) 
	{
		it->second->unlockAllDoorsInThisRoom();
		it->second->deleteAllFeaturesInThisRoom();
		//delete(it->second);
	}
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

void House::printRooms()
{
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) {
		std::cout << "\t" << it->first << std::endl;
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

std::string House::findFeatureByName (string featureAlias) {
	if ( DEBUG_FEATURES ) std::cout << "----- begin House::findFeatureByName(), looking for '"<< featureAlias<<"'" << std::endl;
	if ( houseFeatures.find(featureAlias) == houseFeatures.end() )
	{
		if ( DEBUG_FEATURES ) std::cout << "     Not found in houseFeatures, searching through houseFeatureAliases" << std::endl;
		if ( houseFeatureAliases.find(featureAlias) == houseFeatureAliases.end() )
		{
			return NOTFOUND;
		}
		if ( DEBUG_FEATURES ) std::cout << "     Found alias" << std::endl;
		return houseFeatureAliases[featureAlias];
  }
	else
	{
		if ( DEBUG_FEATURES ) std::cout << "     found canonical name" << std::endl;
		return featureAlias;
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

std::string House::getRoomFeaturesSaveString()
{
	std::string saveString = "";
	std::vector<std::string> stringVector;
	Room *r1;

	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) {
		//std::cout << "\t" << it->first << "\t" << it->second->getRoomName() << std::endl;
		saveString.append("ROOM_FEATURE:");
		saveString.append(it->first);
		saveString.append("|");
		int count = 0;
		r1 = getRoomPtr(it->first);
		stringVector = r1->getFeaturesVector();
		for (std::vector<std::string>::iterator it = stringVector.begin() ; it != stringVector.end(); ++it)
		{
			if(count == 0)
				saveString.append((*it));
			else
			{
				saveString.append(",");
				saveString.append((*it));
			}
			count++;
		}
		saveString.append("\n");
	}
	return saveString;
}

std::string House::getRoomLockedDoorsSaveString()
{
	std::string saveString = "";
	std::vector<std::string> stringVector;
	Room *r1;

	// for each room in the map
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) 
	{
		r1 = getRoomPtr(it->first);
		saveString.append(r1->getLockedDoorsSaveString());
	}


	return saveString;
}

std::vector<std::string> House::getSolvedFeatures()
{
	std::vector<std::string> completedVector;
	Feature *f1;
	//if (DEBUG_BRENT) std::cout << "Solved Features Starts ====" << std::endl;

	for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++) 
	{
		f1 = getFeaturePtr(it->first);
		if(f1->isSolved())
		{
			//if (DEBUG_BRENT) std::cout << it->first << std::endl;
			completedVector.push_back(it->first);
		}
	}
	//if (DEBUG_BRENT) std::cout << "Solved Features Ends   ====" << std::endl;
	return completedVector;
}

std::vector<std::string> House::getDroppedFeatures()
{
	std::vector<std::string> completedVector;
	Feature *f1;
	//if (DEBUG_BRENT) std::cout << "Solved Features Starts ====" << std::endl;

	for (auto it=houseFeatures.cbegin(); it != houseFeatures.cend(); it++) 
	{
		f1 = getFeaturePtr(it->first);
		if(f1->isDropped())
		{
			//if (DEBUG_BRENT) std::cout << it->first << std::endl;
			completedVector.push_back(it->first);
		}
	}
	//if (DEBUG_BRENT) std::cout << "Solved Features Ends   ====" << std::endl;
	return completedVector;
}

void House::printRoomFeatures(GameState * GS, Room *room)
{
	std::vector<std::string> roomFeatures = room->getFeaturesVector();
	Feature *f1, *f2;
	bool printThis;

	if (DEBUG_BRENT) std::cout << "Start House::printRoomFeatures()" << std::endl;
	for (std::vector<std::string>::iterator it = roomFeatures.begin() ; it != roomFeatures.end(); ++it)
	{
		printThis = true;
   	if (DEBUG_BRENT) std::cout << "printing feature " << (*it) << std::endl;
   	f1 = getFeaturePtr((*it));

		if (! f1) {
			std::cout << "Error getting pointer for " << (*it) << std::endl;
			continue; // This should never happen
		}

   	if(f1->getDependsOn().size() != 0)
   	{
   		f2 = getFeaturePtr(f1->getDependsOn());
   		if (DEBUG_BRENT) std::cout << "DEBUG: " << f1->getName() << " depends on: " << f2->getName() << " solved: " << f2->isSolved() << std::endl;
   		if(! f2->isSolved())
   		{
   			printThis = false;
   		}
   	}
		if (f1->getDependsOnInventory().size() != 0 )
		{
   		f2 = getFeaturePtr(f1->getDependsOnInventory());
   		if (DEBUG_BRENT) std::cout << "DEBUG: " << f1->getName() << " depends on inventory: " << f2->getName() << std::endl;
   		if( f2 && ! GS->featureInHand(f2))
   		{
   			printThis = false;
   		}
		}
   	if ( printThis ) std::cout << f1->getWalkingInRoomText() << std::endl;
	}
	//if (DEBUG_BRENT) std::cout << "Exit House::printFeatures()" << std::endl;
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
