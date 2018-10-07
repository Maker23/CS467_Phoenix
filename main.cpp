/*
 * 
 *
 */
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include "room.hpp"
#include "house.hpp"
#include "utilities.hpp"
#include "container.hpp"

void printGameEnding();

int main()
{
	int GameClock=0;
	House *house = NULL;
	Room 	*currentRoom = NULL;
	GameState PlayerState("player");

	house = new House();
	currentRoom = house->buildHouse();

	if ( currentRoom != NULL )
	{
		std::cout << "\nYou are in the " << currentRoom->getRoomName() << std::endl;
		std::cout << currentRoom->getShortDesc() << "\n\n";
		//std::cout << currentRoom->getLongDesc() << "\n\n";
		//std::cout << currentRoom->getAdditionalDesc() << "\n\n";
		Doorway * doorway;
		for (int r = 0; r < MAX_RM_CONNECTIONS; r++){
			doorway =  currentRoom->Connections[r];
			if (doorway != NULL) {
				std::cout << "\tDoorway " << doorway->direction << ": " << doorway->roomName << std::endl;
			}
		}
		currentRoom = currentRoom->userAction(&PlayerState);
		UpdateGameState(GameClock,&PlayerState,currentRoom);
	}

	//house->printRooms(); // DEBUG
	printGameEnding();
	delete house;
	return 0;
}



void printGameEnding() {
	std::cout << "Game Over" << std::endl;
	
}
