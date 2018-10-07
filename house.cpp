#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include<map>
#include <typeindex>
#include "house.hpp"
#include "room.hpp"

using namespace std;

House::House()
{

}

House::~House()
{
	// need to delete the memory allocated	
}

/*
 * Function that builds the entire house and returns a pointer to the
 * starting room.  IRL this function should probably take a string argument
 * that is the directory of room files, and then look for files in that 
 * directory to pass on to buildRoom(). TODO_FEATURE 
 */

Room * House::buildHouse(){  

	buildRoom("rooms/ballroom");
	buildRoom("rooms/foyer");
	buildRoom("rooms/conservatory");

	return  getRoomPtr("Foyer");

}

bool House::buildRoom(string roomToLoad)
{
	Room *roomPtr=NULL;
	roomPtr = new Room(roomToLoad);

	if(!hasRoom(roomPtr->getRoomName()))
	{
		houseMap[roomPtr->getRoomName()] = roomPtr;
		if(hasRoom(roomPtr->getRoomName()))
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool House::hasRoom(string key)
{
	//https://stackoverflow.com/questions/1939953/how-to-find-if-a-given-key-exists-in-a-c-stdmap
	if ( houseMap.find(key) == houseMap.end() )
	{
      return false;
   }
	else
	{
		return true;
   }
}

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

bool House::printRooms()
{
	// Print all the keys in the houseMap
	std::cout << "--- House rooms: ---" << std::endl;
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) {
		std::cout << it->first << std::endl;
	}
	return true;
}
