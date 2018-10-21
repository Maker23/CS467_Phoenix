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
#include "feature.hpp"
#include "engine.hpp"

void printGameEnding();
/* TODO 
 * 	Make sure the GameStateconstructor deals with game saved files
 *
 *
 *
 *
 *
 */

int main()
{
	int GameClock=0;
	House *house = NULL;
	Room 	*currentRoom = NULL;
	GameState PlayerState("player"); 

	house = new House();
	currentRoom = house->buildHouse("Foyer");

	PlayerState.housePtr = house;
	house->printRooms(&PlayerState);
	std::cout << "=============================================" << std::endl;
	std::cout << "\n Obviously not yet finished !  You can \"go Ballroom\" but not \"go east\"" << std::endl;
	std::cout << " Implemented commands are :" << std::endl;
	std::cout << "   look, go, quit " <<std::endl;
	std::cout << " Parseable but not implemented:" <<std::endl;
	std::cout << "   throw, save, load. " << std::endl;
	std::cout << "\n Any other command will result in a useless help message being printed." << std::endl;
	std::cout << "=============================================" << std::endl;

	currentRoom->Examine(&PlayerState);
	while ( currentRoom != NULL )
	{
		currentRoom = PlayerState.playerTurn(currentRoom);
		PlayerState.UpdateGameState(GameClock,currentRoom);
	}

	//house->printRooms(); // DEBUG
	printGameEnding();
	delete house;
	return 0;
}



void printGameEnding() {
	std::cout << "Game Over" << std::endl;
	
}
