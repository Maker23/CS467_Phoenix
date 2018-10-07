#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "room.hpp"

using namespace std;

// Constructor for Room class. 
// It opens the room file and parses the lines.
// Info on files from http://www.cplusplus.com/doc/tutorial/files/ 
Room::Room(string filename)
{
	ifstream roomfile;
	string lineStr, str;
	int roomCount=0;


	// Iterate through the room file and set values
	roomfile.open(filename.c_str());  // .c_str() got from https://stackoverflow.com/questions/19531269/c-void-function-with-file-stream-error
	if (roomfile.is_open())
	{
		// https://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once
		// http://www.cplusplus.com/reference/string/string/substr/
	   while (std::getline(roomfile, lineStr))  
   	{
   		if(lineStr.find("ROOM NAME: ") != std::string::npos)
   		{
   			roomName = lineStr.substr(11, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("LONG DESC: ") != std::string::npos)
   		{
   			longDesc = lineStr.substr(11, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("SHORT DESC: ") != std::string::npos)
   		{
   			shortDesc = lineStr.substr(12, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("ADITIONAL DESC: ") != std::string::npos)
   		{
   			additionalDesc = lineStr.substr(16, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("LONG EXIT DESC: ") != std::string::npos)
   		{
   			longExitDesc = lineStr.substr(16, lineStr.length()-1);
				continue;
   		}
   		if(lineStr.find("SHORT EXIT DESC: ") != std::string::npos)
   		{
   			shortExitDesc = lineStr.substr(17, lineStr.length()-1);
				continue;
   		}
			//if(lineStr.compare(0, std::string::npos, "CONNECTION ") == 0 )
   		if(lineStr.find("CONNECTION") != std::string::npos)
			{
				if (roomCount >= MAX_RM_CONNECTIONS)
				{
					continue;  // error, actually :)
				}
				size_t startDirection = lineStr.find_last_of(" ") + 1;
				size_t startName = lineStr.find_last_of("|") + 1;
				Connections[roomCount] = new Doorway();
				Connections[roomCount]->direction = lineStr.substr(startDirection, startName - startDirection - 1);
				Connections[roomCount]->roomName = lineStr.substr(startName, lineStr.length() -1);
				roomCount++;
			}
   	}
	}
	else
	{
		cout << "Error opening room file. Exiting...\n";
		exit(1);
	}

	roomSeen = false;
}

Room::~Room()
{

}

std::string Room::getRoomName()
{
	return this->roomName;
}

std::string Room::getLongDesc()
{
	return this->longDesc;
}

std::string Room::getShortDesc()
{
	return this->shortDesc;
}

std::string Room::getAdditionalDesc()
{
	return this->additionalDesc;
}

std::string Room::getLongExitDesc()
{
	return this->longExitDesc;
}

std::string Room::getShortExitDesc()
{
	return this->shortExitDesc;
}

// Constructor for Doorway class. 
Doorway::Doorway()
{
	std::string roomName = "";
	std::string direction = "";
	
}
