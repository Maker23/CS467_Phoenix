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

	house = new House();
	roomPtr = house->buildHouse("Foyer");



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


	Choice *choice;
	Thing  *feature;
	Parser *parse;
	parse = new Parser();

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
			std::vector<Thing*>::iterator iter;
			for ( iter = roomPtr->Features.begin(); iter != roomPtr->Features.end(); iter ++ ) 
			{
				feature = (*iter);
				std::cout << "Found feature " << std::endl;
				std::cout << "\t Name:"   << feature->Name << std::endl;
				std::cout << "\t Story: " << feature->Story << std::endl;
				std::cout << "\t isContainer " << (int) feature->isContainer << std::endl;
				std::cout << "\t Open " << (int) feature->Open << std::endl;
			}
	}


	delete(house);

	return 0;
}
