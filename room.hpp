#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>

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
		std::string getRoomName();
		std::string getLongDesc();
		std::string getShortDesc();
		std::string getAdditionalDesc();
		std::string getLongExitDesc();
		std::string getShortExitDesc();
};

#endif