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

	choice = parse->ParseLine();
	if (DEBUG_FUNCTION) std::cout << "Choice verb is " << choice->Verb << ", test verb is " << (validVerbs) tst << std::endl;
	if (( choice->Verb == (validVerbs)tst ) ||
			( choice->Verb == (validVerbs)look))
	{
			// TODO: write Room::Examine()  :)
			std::cout << roomPtr->getRoomName() << std::endl;
			std::cout << roomPtr->getLongDesc() << "\n\n";
			std::cout << roomPtr->getShortDesc() << "\n\n";
			std::cout << roomPtr->getAdditionalDesc() << "\n\n";


			std::cout << "Short Exit Desc: " << roomPtr->getExitsForDisplay() << std::endl;

			/*
			for ( iter = roomPtr->Features.begin(); iter != roomPtr->Features.end(); iter ++ ) 
			{
				(*iter)->Examine(true,1,0);
			}
			*/
	}

	delete(house);

	return 0;
}
