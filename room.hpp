#ifndef ROOM_HPP
#define ROOM_HPP


#include <string>
#include <stack>
#include <vector>
#include "utilities.hpp"


#define MAX_RM_CONNECTIONS 7

class Doorway 
{
	private:
		std::string displayName;
		std::string goesTo;
		std::vector<std::string> keyWords;

  public:
		Doorway(); // constructor
		~Doorway(); // destructor
		void setDoorway(std::string);  // Parses string and sets the correct values.
		std::string getDisplayName();
		std::string getExitRoomName();

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
		int numExits;
		Doorway * Connections[MAX_RM_CONNECTIONS]; 	// Exits from the room

	public:
	 	// IRL these would be private with iterator and get/set functions. TODO

		std::vector<Feature *> Features;
		
		Room(std::string); 									// constructor
		~Room();													// destructor


		std::string getRoomName();
		std::string getLongDesc();
		std::string getShortDesc();
		std::string getAdditionalDesc();
		void Examine();
		Feature * getFeature(std::string);
		void addExitsToStack(std::stack<std::string> &);	// Adds Room's exits to the given stack - used in House::buildHouse -- should not be needed anywhere else
		Room * goRoom(std::string, GameState *);
};



#endif
