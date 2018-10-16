/* vim:ts=2:
 *
 * File name: utilities.cpp
 *
 * Overview: 
 *   General functions used by both main.cpp and test.cpp to run 
 *   the game.
 *
 */

#include <iostream>
#include <string>
#include <map>

#include "engine.hpp"
#include "utilities.hpp"
#include "feature.hpp"
#include "room.hpp"
#include "house.hpp"

enum GameTaskMarker 
{
	Task1,
	Task2,
	Task3,
	Task4,
	Task5
};

/* ***************************************************************
 * UpdateGameState - runs after every player turn
 *************************************************************** */
void UpdateGameState(int &GameClock, GameState* PlayerState, Room* currentRoom)
{
  int points = PlayerState->getGameTaskStatus();

	if (DEBUG_FUNCTION) std::cout << "===== begin Utilities::UpdateGameState" << std::endl;
	if(points) {} // TODO... silence compile-time warnings...
	GameClock++;
	// Can check on or update various GameTasks here
}

/* ***************************************************************
 * Get Yes or No answer from user  
 * TODO: do we use this in Phoenix? it can be useful for automated testing
 *************************************************************** */
bool getUserYN(const char Default)
{
	enum YNMenuChoice { y, n, YNunknown};
	char YNMenuOptions[] = {'y', 'n'};
	char menuOption;
	char inputBuffer[1024];

	if (TEST) 
	{
		std::cout << "TESTING getUserYN is always Yes" << std::endl;
		return true; // Always Yes for testing
	}

	bool Result=false;

	YNMenuChoice menuChoice = YNunknown;


  do {
  	menuOption = std::cin.get(); // get the first character
		std::cin.getline(inputBuffer,1024); // throw away anything else they typed

	  if ( ! menuOption )
			menuOption = Default; // TODO: Doesn't work, fix.
	
		for (int i=0; i<2; i++)
		{
			if (tolower(menuOption) == YNMenuOptions[i])
			{
				menuChoice = (YNMenuChoice) i;
			}
		}
		switch (menuChoice){
			case y:
				Result = true;
				break;
			case n:
				break;
			case YNunknown:
				std::cout << "Please enter Y or N: ";
				break;
		}
	}
	while (menuChoice == YNunknown);
	return (Result);
}
/*****************************************************************/
