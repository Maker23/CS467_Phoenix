#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include "room.hpp"
#include "house.hpp"
#include "parser.hpp"
#include "engine.hpp"

int main()
{
	House *house;
	Room *roomPtr;
	Doorway *doorway;

	Choice *choice;
	Feature  *feature;
	Feature  *feature2;
	Parser *parse;
	parse = new Parser();
	int notUsed;
	std::vector<Feature*>::iterator iter;

	house = new House();
	roomPtr = house->buildHouse("Foyer");
	house->printRooms();
	std::cout << "Starting Room: " << roomPtr->getRoomName() << "\n\n";
	//std::cout << roomPtr->getExitRoomByKey("notThere") << std::endl;
	//std::cout << roomPtr->getExitRoomByKey("SOUTH") << std::endl;


	std::string userCommand;
	std::string exitStringReturned;

   std::cout << roomPtr->getRoomName() << std::endl;
   std::cout << roomPtr->getExitsForDisplay() << std::endl;

	std::cout << "Command (" << roomPtr->getRoomName() << "): ";
   std::getline (std::cin,userCommand);

   // lock ballroom door to kitchen
   roomPtr = house->getRoomPtr("Ballroom");
   roomPtr->lockExitDoorByKey("kitchen");

   // take simple commands to move through the house.
   while (userCommand.compare("quit") != 0)
   {
   	exitStringReturned = roomPtr->getExitRoomByKey(userCommand);
   	if(exitStringReturned.compare("locked") == 0)
   	{
   		std::cout << "Door is locked." << std::endl;
   		userCommand = "";
   	}

   	if(userCommand.length() > 0)
   	{
   		if(exitStringReturned.length() > 0)
   		{
	     		roomPtr = house->getRoomPtr(exitStringReturned);
	   		std::cout << roomPtr->getExitsForDisplay() << std::endl; 			
   		}
   	}

		std::cout << "Command (" << roomPtr->getRoomName() << "): ";
   	std::getline (std::cin,userCommand);
   }

	delete(house);

	return 0;
}
