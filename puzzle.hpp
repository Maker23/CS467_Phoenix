#ifndef PUZZLE_H
#define PUZZLE_H
#include "utilities.hpp"

#define CRYPTED_STRING "Muaf o whxxwd psoub fog krupx xid mwofd lihzi whyixp xid wofs xa udqdow xid yofd."
#define CLEAR_STRING   "From a little spark may burst the flame which lights the lamp to reveal the game."

class Puzzle
{
	private:
		std::string cryptedString;
		std::string   clearString;
		std::string getUserInput(char* swapOut, char* swapIn);
		std::string replaceChar(std::string modString, char swapOut, char swapIn);

  public:
		bool solvePuzzle();

  Puzzle(std::string xString, std::string cString) {
		cryptedString = xString;
		clearString = cString;
	}

	~Puzzle() {}

};

#endif
