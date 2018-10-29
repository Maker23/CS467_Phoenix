#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
		string myNoun = NOTFOUND;
		list <string> words;
		string oneword;
		Choice * userChoice = new Choice();
		validVerbs action;

    cout << "What would you like to do? ";
    getline (cin, mystr);

		char * sptr;
		char myCstr[mystr.length()+1];
		strcpy(myCstr, mystr.c_str());
    int j = 1;
		sptr = strtok(myCstr, " ");
		while (sptr != NULL ) {
			oneword = sptr;
			words.push_back(oneword);
			sptr = strtok(NULL, " ");
		}

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

		int i=0;
		if((array[i] == "pick" && array[i+1] == "up") ||
			(array[i] == "Pick" && array[i+1] == "up") ||
			(array[i] == "Pick" && array[i+1] == "Up"))
		{
			array[i] = "pickup";
			// TODO: should shuffle-delete array[i+1]
		}

		userChoice->inputVerb = words.front();
		action = getVerb ( words.front());
		userChoice->Verb = action;
		words.pop_front();
		while ( ! words.empty() && myNoun.compare(NOTFOUND) == 0 )
		{
			userChoice->inputNoun = words.front();
			myNoun = getNoun(words.front());
			words.pop_front();
		}
		userChoice->Noun = myNoun;

		//if( userChoice->Verb == unknown) {
		//	action = getVerb ( array[i] );
		//	if(action != unknown) {
		//		userChoice->Verb = action;
		//		if(DEBUG_FUNCTION) std::cout << "verb is now : " << userChoice->Verb << std::endl;
		//	}
		//}
		//else if(userChoice->Verb != unknown)
		//{
			//this is a precondition that the VERB must come first before the subject and noun
    	//I'm willing to change this, but I can't think of a valid command
    	//where the user would say the noun and subject before the verb, can you?
		//	{
	//			userChoice->Noun = getNoun(array[i]);
	//		}
  	//}
	//}

	if (DEBUG_FUNCTION)  cout << "THIS IS THE INPUT VERB: " << userChoice->inputVerb << endl;
	if (DEBUG_FUNCTION)  cout << "THIS IS THE INPUT NOUN: " << userChoice->inputNoun << endl;
	if (DEBUG_FUNCTION)  cout << "THIS IS THE VERB: " << userChoice->Verb << endl;
	if (DEBUG_FUNCTION)  cout << "THIS IS THE NOUN: " << userChoice->Noun << endl;

	if (DEBUG_FUNCTION) std::cout << std::endl << "===== end   Parser::ParseLine" << std::endl;
	return userChoice;
}


std::string
Parser::getNoun(std::string nounString) {
	std::string returnString = NOTFOUND;

	if (DEBUG_FUNCTION) std::cout << "===== begin Parser::getNoun, noun is '" << nounString << "'" << std::endl;

      if(nounString == "West" || nounString == "west"
				|| nounString == "East" || nounString == "east"
				|| nounString == "North" || nounString == "north"
				|| nounString == "South" || nounString == "south"
				|| nounString == "Up" || nounString == "up"
				|| nounString == "Down" || nounString == "down")
      {
        returnString = nounString;
      }
      else if(nounString == "Ballroom" || nounString == "ballroom")
      {
        returnString = "Ballroom";
      }
      else if(nounString == "Foyer" || nounString == "foyer")
      {
        returnString = "Foyer";
      }
      else if( nounString == "Kitchen" || nounString == "kitchen")
      {
        returnString = "Kitchen";
      }
      else if( nounString == "Conservatory" || nounString == "conservatory")
      {
        returnString = "Conservatory";
      }
      else if( nounString == "Conservatory" || nounString == "conservatory")
      {
        returnString = "Conservatory";
      }
      else if( nounString == "Pantry" || nounString == "pantry")
      {
        returnString = "Pantry";
      }
      else if( nounString == "Basement" || nounString == "basement")
      {
        returnString = "Basement";
      }
      else if( nounString == "Rug" || nounString == "rug" || nounString == "Rug1")
      {
        returnString = "Rug1";
      }
      else if( nounString == "Note" || nounString == "note" || nounString == "Paper" || nounString == "paper" || nounString == "Note1" )
      {
        returnString = "Note1";
      }
      else if( nounString == "RecordPlayer" || nounString == "recordplayer" || nounString == "player")
      {
        returnString = "RecordPlayer";
      }
      else if( nounString == "Tapestry" || nounString == "tapestry" || nounString == "Tapestry1")
      {
        returnString = "Tapestry1";
      }
      else if( nounString == "Dishes" || nounString == "dishes" || nounString == "DirtyDishes" )
      {
        returnString = "DirtyDishes";
      }
      else if( nounString == "Record" || nounString == "record" || nounString == "Record1")
      {
        returnString = "Record1";
      }
      else if( nounString == "Padlock" || nounString == "padlock")
      {
        returnString = "Padlock";
      }
      else if( nounString == "Piano" || nounString == "piano" || nounString == "Piano1")
      {
        returnString = "Piano1";
      }
      else if( nounString == "Key" || nounString == "key" || nounString == "Key1")
      {
        returnString = "Key1";
      }


	return returnString;
}

validVerbs
Parser::getVerb(std::string verbString) {
  string mystr;
  Choice * userChoice = new Choice();

	userChoice->inputVerb = verbString; // Save this for overrides

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
      		(verbString.compare("Use") == 0 )  ||
					(verbString.compare("Play") == 0 ) ||
					(verbString.compare("play") == 0 )  )
	     {
		       return (validVerbs)use;
	     }
	else if ((verbString.compare("load") == 0 ) ||
      (verbString.compare("Load") == 0 ))           //sets validVerb to 13
	     {
		       return (validVerbs)load;
	     }
  else if ((verbString.compare("pickup") == 0 )||
			(verbString.compare("get") == 0 )       ||
			(verbString.compare("Get") == 0 )       ||
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
		return (validVerbs)shut;
	}
  else if ((verbString.compare("hit") == 0 ) ||
          (verbString.compare("Hit") == 0 )  )    //sets validVerb to 8
	{
		return (validVerbs)hit;
	}
  else if ((verbString.compare("unlock") == 0 )  ||
            (verbString.compare("Unlock") == 0 ))    //sets validVerb to 9
  {
    return (validVerbs)unlock;
  }
  else if ((verbString.compare("inventory") == 0 ) ||
            (verbString.compare("Inventory") == 0 ) ||
            (verbString.compare("Carrying") == 0 ) ||
            (verbString.compare("carrying") == 0 ) ||
            (verbString.compare("Holding") == 0 ) ||
            (verbString.compare("holding") == 0 ) )
  {
    return (validVerbs)inventory; //sets validVerb to 10
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
