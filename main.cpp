/*
 * 
 *
 */
#include <iostream>
#include <fstream>
#include <typeinfo>
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
void cmdLineFlags(int argc, char **argv, GameState*GS);
/* TODO 
 * 	Make sure the GameStateconstructor deals with game saved files
 *
 *
 *
 *
 *
 */

int main(int argc, char *argv[])
{
	int GameClock=0;
	House *house = NULL;
	Room 	*currentRoom = NULL;
	GameState GS("your backpack"); 

	cmdLineFlags(argc, argv, &GS);

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

/* **********************************************************
 * Not proud of this, but apparently the version of ccmake on
 * flip is too out of date to compile the gflags library :(
 * ********************************************************** */
void cmdLineFlags(int argc, char **argv, GameState*GS){
	std::string testArg = "-test";
	std::string fileName = "";
	std::ifstream testFileIn;


	if (DEBUG_FUNCTION) std::cout << "++++   In CmdLineFlags,argc=" << argc << std::endl;
	for ( int i = 0; i < argc; i++ ) {
		std::cout << "argv[" << i << "] = " << argv[i] << std::endl; 
		if ( testArg.compare(argv[i]) == 0 ) 
		{
			if ( (i + 2) <= argc ) {
				// Is there another argument?  if not error
			  if (DEBUG_FUNCTION) std::cout << "Yes Virginia there is another argument" << fileName <<std::endl;
				fileName = argv[i+1];
				i++;
			  std::cout << "filename =" << fileName <<std::endl;
				GS->GameTestFilename = fileName;
				if (DEBUG_FUNCTION) std::cout << "GameTestFilename =" << GS->GameTestFilename  << std::endl;
				
				GS->GameTestFile.open(GS->GameTestFilename.c_str());
				if (GS->GameTestFile.is_open() ) {
					if (DEBUG_FUNCTION) std::cout << "       Setting GameTest to true" << std::endl;
					GS->GameTest = true;
					if ( ! ( GS->GameTestFile.good() ) ){
						std::cout << "ERROR: ifstream pointers is BAD" << std::endl;
						exit(1);
					}
				}
				else {
					std::cout << "ERROR: Couldn't open test file " << GS->GameTestFilename << std::endl;
					exit(1);
				}
			}
			else {
				std::cout << "ERROR: expected a filename after the -test flag" << std::endl;
				exit(1);
			}
			std::cout << " .";
		}
	}
}


void printGameEnding() {
	std::cout << "Game Over" << std::endl;
	
}
