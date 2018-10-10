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

House::House()
{

}

House::~House()
{
	for (auto it=houseMap.cbegin(); it != houseMap.cend(); it++) {
		delete(it->second);
	}
}

/*
 * Function that builds the entire house and returns a pointer to the
 * starting room.  IRL this function should probably take a string argument
 * that is the directory of room files, and then look for files in that 
 * directory to pass on to buildRoom(). TODO_FEATURE 
 */

Room *House::buildHouse(string startingRoom){  

	stack<string> roomsToLoad;
	string roomName;

	// Load the first room, and get its pointer for the return.
	Room *startingRoomPtr=NULL;
	Room *roomPtr=NULL;
	roomPtr = new Room(startingRoom);
	startingRoomPtr = roomPtr;

	roomPtr->getExists(roomsToLoad);
	if(!hasRoom(roomPtr->getRoomName()))
	{
		houseMap[roomPtr->getRoomName()] = roomPtr;
	}

	while (!roomsToLoad.empty())
	{
		roomName=roomsToLoad.top();
		roomsToLoad.pop();
		string str;
		str.append("rooms/");
		str.append(roomName);
		if(!hasRoom(roomName))
		{
			roomPtr = new Room(str);
			roomPtr->getExists(roomsToLoad);
			houseMap[roomPtr->getRoomName()] = roomPtr;
		}
	}


	return  startingRoomPtr;

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
