#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include "puzzle.hpp"


#define DEBUG true


std::string Puzzle::getUserInput (char *swapOut, char * swapIn)
{
	std::string inputString;
	uint inputLen;
	char * token;

	std::getline (std::cin, inputString);
	inputLen=inputString.length();
	char inputCStr[inputLen+1];

	size_t foundEq = inputString.find("=");
	if ( foundEq != std::string::npos )
	{
		if (DEBUG) std::cout << "found = " << std::endl;
		strcpy(inputCStr, inputString.c_str());
		token = strtok(inputCStr, " =");
		if (DEBUG) std::cout << "1 token = " << token << std::endl;
		swapOut[0] = token[0];
		token = strtok(NULL, " =");
		if ( token == NULL ) {
			std::cout << "ERROR: need two chars for an = statement";
		}
		if (DEBUG) std::cout << "2 token = " << token << std::endl;
		swapIn[0] = token[0];
		return "";
	}
	else if ( inputLen == cryptedString.length() )
	{
		if (DEBUG) std::cout << "found a guess" << std::endl;
		// Assume the user is trying to guess the full string
	}
	else {
		if (DEBUG) std::cout << "found string of length " << inputLen << std::endl;
	}

	return inputString;
}

std::string Puzzle::replaceChar (std::string modString, char swapOut, char swapIn)
{
	for (std::string::iterator iter = modString.begin(); iter < modString.end(); iter++ ) 
	{
		if ( *iter  == swapOut ) { 
			*iter = swapIn;
		}
		else if ( *iter == swapIn ) {
			*iter = swapOut;
		}
	}
	return modString;
}

bool Puzzle::solvePuzzle()
{

	std::string inputString;
	char swapIn = '\0';
	char swapOut = '\0';

	if (DEBUG) std::cout << CLEAR_STRING << "\n" << std::endl;
	std::cout << "This is a substitution cypher.  You can solve it by replacing" << std::endl;
	std::cout << "one character at a time, by entering  " << std::endl;
	std::cout << "     x = c " <<std::endl;
	std::cout << "to replace the encrypted character 'x' with the real character 'c'." << std::endl;
	std::cout << "If you think you know the full phrase you can guess it by typing it in." << std::endl;
	std::cout << "Type 'q' or 'quit' to stop solving the puzzle." << std::endl;
	std::cout << "Good luck!" << std::endl;


	while (cryptedString.compare(clearString) != 0) 
	{
		std::cout << "The phrase is: " << cryptedString << std::endl;
		std::cout << "Your guess:    ";
		inputString = getUserInput(&swapOut, &swapIn);
		if (inputString.length() == 0 ) 
		{
			std::cout << "Swapping character " << swapOut <<" for "<< swapIn << std::endl;
			cryptedString = replaceChar(cryptedString, swapOut, swapIn);
			// Swap two characters in cryptedString
		}
		else if (inputString.compare("q") == 0 || inputString.compare("quit") == 0 )
		{
			if (DEBUG) std::cout << "found QUIT" << std::endl;
			return false;
		}
		else if (inputString.length() == cryptedString.length() ) 
		{
			if (inputString.compare(clearString) == 0 ) {
				break;
			}
			// Test exact match, if yes cryptedString=inputString, if no cryptedString is unchanged
		}
	}
	std::cout << "You got it!" << std::endl;

	return true;
}

/*
main () {
	Puzzle* myPuzzle = new Puzzle (CRYPTED_STRING, CLEAR_STRING);
	bool solved;

	solved = myPuzzle->solvePuzzle();
	std::cout << "solved = " << solved << std::endl;
}
*/

