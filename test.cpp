#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include "room.hpp"
#include "house.hpp"

int main()
{
	House *house;
	house = new House();
	house->buildHouse("rooms/foyer");

	Room *roomPtr;
	Doorway *doorway;
	roomPtr = house->getRoomPtr("Foyer");

	std::cout << roomPtr->getRoomName() << std::endl;

	for (int r = 0; r < MAX_RM_CONNECTIONS; r++){
		doorway =  roomPtr->Connections[r];
		if (doorway != NULL) {
			std::cout << "\tDoorway " << doorway->direction << ": " << doorway->roomName << std::endl;
		}
	}
	
	//std::cout << roomPtr->getLongDesc() << "\n\n";
	//std::cout << roomPtr->getShortDesc() << "\n\n";
	//std::cout << roomPtr->getAdditionalDesc() << "\n\n";
	//std::cout << roomPtr->getLongExitDesc() << "\n\n";
	//std::cout << roomPtr->getShortExitDesc() << "\n";
	delete house;

	return 0;
}
