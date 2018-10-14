#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "parser.hpp"
#include "actions.hpp"
#include <typeinfo>

using namespace std;

Parser::Parser()
{

}

Parser::~Parser()
{
	// need to delete the memory allocated
}

Choice * Parser::ParseLine(){
    string mystr;
		Choice * userChoice = new Choice();
		validVerbs action;

    cout << "What would you like to do? ";
    getline (cin, mystr);

    int j = 1;

    for (int i=0; i < (int) mystr.size(); i++) {
        if(mystr[i] == ' ' && mystr[i+1] != ' ')
        {
            j++;
        }
    }

    for (int i=0; i < (int) mystr.size(); i++) {
       // cout << "mystr[" << i << " ] = " << mystr[i] << endl;
    }

    string array[j+1];
    string testing2;
    int test = 1;
    cout << endl;
    cout << "string size = " << mystr.size() << endl;
    for (int i=0; i < (int) mystr.size(); i++)
        {
            if(mystr[i] == ' ' && mystr[i+1] != ' ')
            {
                array[test] = testing2;
                testing2 = "";
                test++;
            }
             testing2 += mystr[i];
        }
    array[test] = testing2;

    for (int i = 1; i < j+1; i++)
    {
        cout << "array[" << i << "] " << array[i] << endl;
    }

    cout << "num args: " << j << endl;
		//
	  // TODO: Convert the user-entered text into valid commands
	
		// For now we'll just use the literal text
		action = getVerb ( array[1] );
		if (DEBUG_FUNCTION) std::cout << "Setting choice verb to " << (validVerbs) action << std::endl;
		userChoice->verb = action; 

		return userChoice;
}

validVerbs Parser::getVerb(std::string verbString) {
	
	if ((verbString.compare("tst") == 0 )||
			(verbString.compare("test") == 0 ) ||
			(verbString.compare("?") == 0))
	{
		return (validVerbs)tst;
	}
	else 
	{
		return (validVerbs)help;
	}
}
