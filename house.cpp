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
#include "house.hpp"
#include "room.hpp"

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

<<<<<<< HEAD

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
=======
bool House::buildHouse(string roomToLoad)
{
>>>>>>> dc8970063b7b2c123f70cf049b513c83b2be613c
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
			str.append("rooms/");
			str.append(roomName);
			roomPtr = new Room(str);
			if (startingRoomPtr == NULL)
			{
				startingRoomPtr = roomPtr;  // sets the starting room pointer for the return
			}
			roomPtr->addExitsToStack(roomsToLoad);
			houseMap[roomPtr->getRoomName()] = roomPtr;
		}
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
	if ( houseMap.find(key) == houseMap.end() )
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
	if(hasRoom(roomName))
	{
		return houseMap[roomName];
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
bool House::printRooms()
{
	// Print all the keys in the houseMap
	std::cout << "--- House rooms: ---" << std::endl;
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) {
		std::cout << it->first << std::endl;
	}
	return true;
}
