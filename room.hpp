#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>

class Room
{
	private:
		bool roomSeen;

	public:
		Room(std::string); // constructor
		~Room();	// destructor
};

#endif