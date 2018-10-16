#ifndef ROOM_HPP
#define ROOM_HPP


#include <string>
#include <stack>
#include <vector>
#include "utilities.hpp"


#define MAX_RM_CONNECTIONS 7

class Doorway 
{
  public:
		Doorway(); // constructor
		~Doorway(); // destructor
	 	std::string roomName;
		std::string direction;
		std::string Examine();
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
	 	// IRL these would be private with iterator and get/set functions. TODO
		Doorway * Connections[MAX_RM_CONNECTIONS]; 	// Exits from the room
		std::vector<Feature *> Features;
		
		Room(std::string); 									// constructor
		~Room();													// destructor


		std::string getRoomName();
		std::string getLongDesc();
		std::string getShortDesc();
		std::string getAdditionalDesc();
		std::string getLongExitDesc();
		std::string getShortExitDesc();
		void Examine();
		Feature * getFeature(std::string);
		void addExitsToStack(std::stack<std::string> &);	// Adds Room's exits to the given stack - used in House::buildHouse -- should not be needed anywhere else
		Room * goRoom(std::string, GameState *);
};



#endif
