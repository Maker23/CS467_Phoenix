#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "parser.hpp"
#include "engine.hpp"
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
		if (DEBUG_FUNCTION) cout << "string size = " << mystr.size() << endl;
    for (int i=0; i < (int) mystr.size(); i++)
        {
            if(mystr[i] == ' ' && mystr[i+1] != ' ')
            {
                array[test] = testing2;
                testing2 = "";
								i++;
                test++;
            }
            testing2 += mystr[i];
        }
    array[test] = testing2;

    for (int i = 1; i < j+1; i++)
    {
			if (DEBUG_FUNCTION) cout << "array[" << i << "] " << array[i] << endl;
    }

		if (DEBUG_FUNCTION) cout << "num args: " << j << endl;
		//
	  // TODO: Convert the user-entered text into valid commands
	
		// For now we'll just use the literal text
	  if ( j >= 1 ){
			action = getVerb ( array[1] );
			userChoice->Verb = action; 
			if (DEBUG_FUNCTION) std::cout << "Setting verb to " << (validVerbs) action << std::endl;
		}
		if ( j >= 2 ) {
			if (DEBUG_FUNCTION) std::cout << "and noun to" << array[2] << std::endl;
			userChoice->Noun = array[2]; 
		}

		if (DEBUG_FUNCTION) std::cout << "===== end   Parser::ParseLine" << std::endl;
		return userChoice;
}


validVerbs
Parser::getVerb(std::string verbString) {
	
	// THIS IS DEFINITELY NOT COMPLETE!
	// I hardcoded a bunch of stuff here so we could test other parts of the game
	
	if (DEBUG_FUNCTION) std::cout << "===== begin Parser::getVerb, verb is '" << verbString << "'" << std::endl;
	if ((verbString.compare("tst") == 0 )||
			(verbString.compare("test") == 0 ) ||
			(verbString.compare("?") == 0))
	{
		return (validVerbs)tst;
	}
	else if ((verbString.compare("go") == 0 )||
			(verbString.compare("move") == 0 ) ||
			(verbString.compare("travel") == 0))
	{
		return (validVerbs)go;
	}
	else if ((verbString.compare("look") == 0 )||
			(verbString.compare("investigate") == 0 ) ||
			(verbString.compare("examine") == 0))
	{
		return (validVerbs)look;
	}
	else if ((verbString.compare("throw") == 0 )||
			(verbString.compare("toss") == 0 ) ||
			(verbString.compare("hurl") == 0))
	{
		return (validVerbs)hurl;
	}
	else if (verbString.compare("save") == 0 )
	{
		return (validVerbs)save;
	}
	else if (verbString.compare("load") == 0 )
	{
		return (validVerbs)load;
	}
	else if ((verbString.compare("quit") == 0 )||
			(verbString.compare("q") == 0 ) ||
			(verbString.compare("Quit") == 0))
	{
		return (validVerbs)quit;
	}
	else 
	{
		return (validVerbs)help;
	}
}
