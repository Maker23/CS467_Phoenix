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

	delete(house);

	return 0;
}
