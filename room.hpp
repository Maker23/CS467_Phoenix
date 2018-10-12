#ifndef ROOM_HPP
#define ROOM_HPP


#include <string>

#define MAX_RM_CONNECTIONS 5

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

	public:
		Room(std::string); // constructor
		~Room();	// destructor

		Doorway * Connections[MAX_RM_CONNECTIONS];
		std::string getRoomName();
		std::string getLongDesc();
		std::string getShortDesc();
		std::string getAdditionalDesc();
		std::string getLongExitDesc();
		std::string getShortExitDesc();
};



#endif
