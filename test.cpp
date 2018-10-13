#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include "room.hpp"
#include "house.hpp"
#include "parser.hpp"

int main()
{
	House *house;
	Room *roomPtr;
	Doorway *doorway;

	house = new House();
<<<<<<< HEAD
	roomPtr = house->buildHouse("Foyer");
=======
	house->buildHouse("rooms/ballroom");
	house->buildHouse("rooms/foyer");
	house->buildHouse("rooms/conservatory");
>>>>>>> dc8970063b7b2c123f70cf049b513c83b2be613c



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

<<<<<<< HEAD
	std::cout << "Starting Room: " << roomPtr->getRoomName() << "\n\n";

=======

	Parser *parse;
	parse = new Parser();

	parse->ParseLine();
>>>>>>> dc8970063b7b2c123f70cf049b513c83b2be613c


	//std::cout << roomPtr->getLongDesc() << "\n\n";
	//std::cout << roomPtr->getShortDesc() << "\n\n";
	//std::cout << roomPtr->getAdditionalDesc() << "\n\n";
	//std::cout << roomPtr->getLongExitDesc() << "\n\n";
	//std::cout << roomPtr->getShortExitDesc() << "\n";
	delete(house);

	return 0;
}
