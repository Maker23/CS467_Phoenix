#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include "room.hpp"
#include "house.hpp"

int main()
{
	House *house;
	Room *roomPtr;
	Doorway *doorway;

	house = new House();
	roomPtr = house->buildHouse("rooms/foyer");

/*
  std::string readRooms[] = {"Foyer", "Ballroom", "Conservatory"};

  for (int i=0; i<3; i++){
		roomPtr = house->getRoomPtr(readRooms[i]);

		if ( roomPtr != NULL )
		{
			std::cout << roomPtr->getRoomName() << std::endl;
	
			for (int r = 0; r < MAX_RM_CONNECTIONS; r++){
				doorway =  roomPtr->Connections[r];
				if (doorway != NULL) {
					std::cout << "\tDoorway " << doorway->direction << ": " << doorway->roomName << std::endl;
				}
			}
		}
	}
*/
	house->printRooms();

	std::cout << "Starting Room: " << roomPtr->getRoomName() << "\n\n";



	//std::cout << roomPtr->getLongDesc() << "\n\n";
	//std::cout << roomPtr->getShortDesc() << "\n\n";
	//std::cout << roomPtr->getAdditionalDesc() << "\n\n";
	//std::cout << roomPtr->getLongExitDesc() << "\n\n";
	//std::cout << roomPtr->getShortExitDesc() << "\n";
	delete house;

	return 0;
}
