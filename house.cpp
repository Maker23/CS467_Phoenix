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

bool House::buildHouse(string roomToLoad)
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