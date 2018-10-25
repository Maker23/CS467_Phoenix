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
      if((array[i] == "pick" && array[i+1] == "up") ||
      (array[i] == "Pick" && array[i+1] == "up") ||
      (array[i] == "Pick" && array[i+1] == "Up"))
      {
        array[i] = "pickup";
      }

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
      else if( array[i] == "Rug" || array[i] == "rug")
      {
        userChoice->Noun = "Rug";
      }

      else if( array[i] == "Record" || array[i] == "record")
      {
        if(array[i+1] == "Player" || array[i+1] == "player")
        userChoice->Noun = "RecordPlayer";
      }
      else if( array[i] == "Tapestry" || array[i] == "tapestry")
      {
        userChoice->Noun = "Tapestry";
      }
      else if( array[i] == "Dishes" || array[i] == "dishes")
      {
        userChoice->Noun = "Dishes";
      }
      else if( array[i] == "Record" || array[i] == "record")
      {
        userChoice->Noun = "Record";
      }
      else if( array[i] == "Padlock" || array[i] == "padlock")
      {
        userChoice->Noun = "Padlock";
      }
      else if( array[i] == "Piano" || array[i] == "piano")
      {
        userChoice->Noun = "Piano";
      }
      else if( array[i] == "Key" || array[i] == "key")
      {
        userChoice->Noun = "Key";
      }

   }
}

cout << "THIS IS THE VERB: " << userChoice->Verb << endl;
cout << "THIS IS THE NOUN: " << userChoice->Noun << endl;

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
      (verbString.compare("run") == 0)  ||
      (verbString.compare("Go") == 0 )||
    	(verbString.compare("Move") == 0 ) ||
    	(verbString.compare("Travel") == 0) ||
      (verbString.compare("Walk") == 0) ||
      (verbString.compare("Run") == 0))
	     {
		       userChoice->Verb = (validVerbs)go;  //sets the validVerb to 1
           return userChoice->Verb;
	     }
	else if ((verbString.compare("look") == 0 )||  //sets the validVerb to 0
			(verbString.compare("investigate") == 0 ) ||
			(verbString.compare("examine") == 0) ||
      (verbString.compare("Look") == 0 )||
    	(verbString.compare("Investigate") == 0 ) ||
    	(verbString.compare("Examine") == 0))
	     {
		        return (validVerbs)look;
	     }
	else if ((verbString.compare("throw") == 0 )||
			(verbString.compare("toss") == 0 ) ||
			(verbString.compare("hurl") == 0) ||
      (verbString.compare("Throw") == 0)||
    	(verbString.compare("Toss") == 0 ) ||
    	(verbString.compare("Hurl") == 0))         //sets validVerb to 7
	     {
		       return (validVerbs)hurl;
	     }
	else if ((verbString.compare("save") == 0 ) ||
  (verbString.compare("Save") == 0 ))
	{
		return (validVerbs)save;
	}
  else if ((verbString.compare("use") == 0 ) ||
      (verbString.compare("Use") == 0 ))             //sets validVerb to 2
	     {
		       return (validVerbs)use;
	     }
	else if ((verbString.compare("load") == 0 ) ||
      (verbString.compare("Load") == 0 ))           //sets validVerb to 13
	     {
		       return (validVerbs)load;
	     }
  else if ((verbString.compare("pickup") == 0 )||
			(verbString.compare("take") == 0 )       ||
      (verbString.compare("Take") == 0 ))        //sets validVerb to 3
	{
		return (validVerbs)take;
	}
  else if ((verbString.compare("drop") == 0 )  ||
          (verbString.compare("Drop") == 0 ))//sets validVerb to 4
	{
		return (validVerbs)drop;
	}
  else if ((verbString.compare("open") == 0 )   ||
          (verbString.compare("Open") == 0 ))  //sets validVerb to 5
	{
		return (validVerbs)open;
	}
  else if ((verbString.compare("close") == 0 ) ||
          (verbString.compare("Close") == 0 ))//sets validVerb to 6
	{
		return (validVerbs)close;
	}
  else if ((verbString.compare("hit") == 0 ) ||
          (verbString.compare("Hit") == 0 )   ||
          (verbString.compare("Play") == 0 )  ||
          (verbString.compare("play") == 0 )  )    //sets validVerb to 8
	{
		return (validVerbs)hit;
	}
  else if ((verbString.compare("eat") == 0 )  ||
            (verbString.compare("Eat") == 0 ))    //sets validVerb to 9
  {
    return (validVerbs)eat;
  }
  else if ((verbString.compare("inventory") == 0 ) ||
            (verbString.compare("Inventory") == 0 ))  //sets validVerb to 10
  {
    return (validVerbs)eat;
  }
  else if (verbString.compare("LastAction") == 0 ) //sets validVerb to 11
  {                                               //not sure what an example of this would be?
    return (validVerbs)LastAction;
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
