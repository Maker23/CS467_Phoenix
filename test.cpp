#include <iostream>
#include <string>
#include <stdlib.h>
#include "room.hpp"

int main()
{
	Room *room;
	room = new Room("rooms/foyer");

	std::cout << room->getRoomName() << "\n\n";
	std::cout << room->getLongDesc() << "\n\n";
	std::cout << room->getShortDesc() << "\n\n";
	std::cout << room->getAdditionalDesc() << "\n\n";
	std::cout << room->getLongExitDesc() << "\n\n";
	std::cout << room->getShortExitDesc() << "\n";
	delete room;

	return 0;
}
