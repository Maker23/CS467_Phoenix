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

		if (DEBUG_FUNCTION) std::cout << "num args: " << j << std::endl;
		//
	  // TODO: Convert the user-entered text into valid commands
for(int i = 0; i <= j; i++)
{
  // I deletd the DEBUG_FUNCTION outputs that were here because they no longer were helpful with my new code
			action = getVerb ( array[i] );
      if(action != unknown && userChoice->Verb == unknown){   //if its a verb we included and verb hasn't been set yet

        userChoice->Verb = action;
        if(DEBUG_FUNCTION) std::cout << "verb is now : " << userChoice->Verb << std::endl;
      }

  if(userChoice->Verb != unknown)  //this is a precondition that the VERB must come first before the subject and noun
                                  //I'm willing to change this, but I can't think of a valid command
                                  //where the user would say the noun and subject before the verb, can you?
  {
      if(array[i] == "Ballroom" || array[i] == "ballroom")
      {
        userChoice->Noun = "Ballroom";
      }
      else if(array[i] == "Foyer" || array[i] == "foyer")
      {
        userChoice->Noun = "Foyer";
      }
      else if( array[i] == "Kitchen" || array[i] == "kitchen")
      {
        userChoice->Noun = "Kitchen";
      }
      else if( array[i] == "Conservatory" || array[i] == "conservatory")
      {
        userChoice->Noun = "Conservatory";
      }
      else if( array[i] == "Conservatory" || array[i] == "conservatory")
      {
        userChoice->Noun = "Conservatory";
      }
      else if( array[i] == "Pantry" || array[i] == "pantry")
      {
        userChoice->Noun = "Pantry";
      }

      if(array[i] == "Ballroom")
      {
        userChoice->Subject = array[i];
        if(DEBUG_FUNCTION) std::cout << "subject is now: " << userChoice->Subject << std::endl;
      }

   }
}
		if (DEBUG_FUNCTION) std::cout << std::endl << "===== end   Parser::ParseLine" << std::endl;
		return userChoice;
}


validVerbs
Parser::getVerb(std::string verbString) {

	// THIS IS DEFINITELY NOT COMPLETE!
	// I hardcoded a bunch of stuff here so we could test other parts of the game

  string mystr;
  Choice * userChoice = new Choice();

	if (DEBUG_FUNCTION) std::cout << "===== begin Parser::getVerb, verb is '" << verbString << "'" << std::endl;
	if ((verbString.compare("h") == 0 )||
			(verbString.compare("help") == 0 ) ||
			(verbString.compare("?") == 0))
	{
    userChoice->Verb = (validVerbs)help;    //sets the validVerb to 12
		return userChoice->Verb;
	}
	else if ((verbString.compare("go") == 0 )||
			(verbString.compare("move") == 0 ) ||
			(verbString.compare("travel") == 0) ||
      (verbString.compare("walk") == 0) ||
      (verbString.compare("run") == 0))
	{
		userChoice->Verb = (validVerbs)go;
    return userChoice->Verb;
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
		return (validVerbs)unknown;
	}
}
//
