/*
 * 
 *
 */
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include "utilities.hpp"
#include "engine.hpp"
#include "room.hpp"
#include "house.hpp"
#include "feature.hpp"
#include "puzzle.hpp"

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
	GameState GS("your backpack"); 

	house = new House();
	currentRoom = house->buildHouse("Foyer");
	GS.housePtr = house;
	GS.puzzle = new Puzzle(CRYPTED_STRING, CLEAR_STRING);

	house->printRooms(&GS);
	house->printFeatures(&GS);
	house->debugHouse();


	std::cout << "=============================================" << std::endl;
	std::cout << "\nTo move around use verbs like 'go', 'move', 'walk', etc" << std::endl;
	std::cout << "\nTo interact with objects in the room try words like 'use','take','move', etc" << std::endl;
	std::cout << "=============================================" << std::endl;

	currentRoom->Examine(&GS);
	while ( currentRoom != NULL )
	{
		GS.UpdateGameState(GameClock,currentRoom);
		currentRoom = GS.playerTurn(currentRoom);
	}

	//house->printRooms(); // DEBUG
	printGameEnding();
	delete house;
	return 0;
}



void printGameEnding() {
	std::cout << "Game Over" << std::endl;
	
}
