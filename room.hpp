#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <stack>
#include <vector>


#define MAX_RM_CONNECTIONS 7

class GameState; // Forward declaration
class Actions; // Forward declaration

class Doorway 
{
  public:
		Doorway(); // constructor
		~Doorway(); // destructor
	 	std::string roomName;
		std::string direction;
	
};

class Room
{
	private:
		bool roomSeen;
		std::string roomName;
		std::string longDesc;
		std::string shortDesc;
		std::string additionalDesc;
		std::string longExitDesc;
		std::string shortExitDesc;
		int numExits;

	public:
		Doorway * Connections[MAX_RM_CONNECTIONS]; 	// Exits from the room
		
		Room(std::string); 									// constructor
		~Room();													// destructor


		std::string getRoomName();
		std::string getLongDesc();
		std::string getShortDesc();
		std::string getAdditionalDesc();
		std::string getLongExitDesc();
		std::string getShortExitDesc();

		void addExitsToStack(std::stack<std::string> &);	// Adds Room's exits to the given stack

		Room * userAction(GameState *);
		Actions * Examine();
};



#endif
