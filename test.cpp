#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include "room.hpp"
#include "house.hpp"
#include "parser.hpp"
#include "actions.hpp"

int main()
{
	House *house;
	Room *roomPtr;
	Doorway *doorway;

	Choice *choice;
	Thing  *feature;
	Thing  *feature2;
	Parser *parse;
	parse = new Parser();
	int notUsed;
	std::vector<Thing*>::iterator iter;

	house = new House();
	roomPtr = house->buildHouse("Foyer");
	house->printRooms();
	std::cout << "Starting Room: " << roomPtr->getRoomName() << "\n\n";

	choice = parse->ParseLine();
	if (DEBUG_FUNCTION) std::cout << "Choice verb is " << choice->verb << ", test verb is " << (validVerbs) tst << std::endl;
	if ( choice->verb == (validVerbs)tst ) 
	{
			std::cout << roomPtr->getRoomName() << std::endl;
			std::cout << roomPtr->getLongDesc() << "\n\n";
			std::cout << roomPtr->getShortDesc() << "\n\n";
			std::cout << roomPtr->getAdditionalDesc() << "\n\n";
			std::cout << roomPtr->getLongExitDesc() << "\n\n";
			std::cout << roomPtr->getShortExitDesc() << "\n";

			for (int r = 0; r < MAX_RM_CONNECTIONS; r++){
				doorway =  roomPtr->Connections[r];
				if (doorway != NULL) {
					std::cout << "\tDoorway " << doorway->direction << ": " << doorway->roomName << std::endl;
				}
			}
			for ( iter = roomPtr->Features.begin(); iter != roomPtr->Features.end(); iter ++ ) 
			{
				(*iter)->Examine(true,1,0);
			}
	}

	delete(house);

	return 0;
}
