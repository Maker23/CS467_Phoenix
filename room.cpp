#include <string>
#include <iostream>
#include <fstream>
#include "room.hpp"

using namespace std;

// Constructor for Room class. 
// It opens the room file and parses the lines.
// Info on files from http://www.cplusplus.com/doc/tutorial/files/ 
Room::Room(string filename)
{
	ifstream roomfile;
	string str;

	roomfile.open(filename.c_str());  // .c_str() got from https://stackoverflow.com/questions/19531269/c-void-function-with-file-stream-error
   while (std::getline(roomfile, str))  // https://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once
   {
      cout << str << "\n";
   }
}

Room::~Room()
{

}