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
    string myVerb = NOTFOUND;
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

    //while ( ! words.empty() && myVerb.compare(NOTFOUND) == 0 ) //nick changed this
    while ( ! words.empty() && userChoice->Verb == 16 )
    {
  		userChoice->inputVerb = words.front();
  		action = getVerb ( words.front());
      if (action != 16)
      {
        userChoice->Verb = action;
      }
  		words.pop_front();
    }
  		while ( ! words.empty() && myNoun.compare(NOTFOUND) == 0 )
    //  while ( ! words.empty() || userChoice->Noun == "" )
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
				|| nounString == "Down" || nounString == "down"
        || nounString == "southwest" || nounString == "southcenter"
        || nounString == "southeast" || nounString == "right"
        || nounString == "left" || nounString == "ne" || nounString == "se"
        || nounString == "n" || nounString == "s"
        || nounString == "1" || nounString == "2" || nounString == "3"
        || nounString == "4" || nounString == "5" || nounString == "stairs")
      {
        returnString = nounString;
      }
      else if(nounString == "Ballroom" || nounString == "ballroom")
      {
        returnString = "ballroom";
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
      else if( nounString == "Door1" || nounString == "door1" || nounString == "Bedroom1" || nounString == "bedroom1")
      {
        returnString = "Bedroom1";
      }
      else if(  nounString == "Door2" || nounString == "door2" || nounString == "Nursery" || nounString == "nursery" )
      {
        returnString = "Nursery";
      }
      else if(  nounString == "Door3" || nounString == "door3" || nounString == "Library" || nounString == "library" )
      {
        returnString = "Library";
      }
      else if(  nounString == "Door4" || nounString == "door4" || nounString == "Study" || nounString == "study" )
      {
        returnString = "Study";
      }
      else if(  nounString == "Door5" || nounString == "door5" || nounString == "Balcony" || nounString == "balcony" )
      {
        returnString = "Balcony";
      }
      else if( nounString == "Balcony" || nounString == "balcony" )
      {
        returnString = "Balcony";
      }
      else if( nounString == "2nd" || nounString == "second" || nounString == "hallway" || nounString == "Hallway")
      {
        returnString = "hallway";
      }
      else if( nounString == "Closet" || nounString == "closet")
      {
        returnString = "Closet";
      }
      else if( nounString == "Library" || nounString == "library")
      {
        returnString = "Library";
      }

      else if( nounString == "Rug" || nounString == "rug" || nounString == "Rug1" || nounString == "rug1")
      {
        returnString = "rug1";
      }
      else if( nounString == "Note" || nounString == "note" || nounString == "Paper" || nounString == "paper" || nounString == "Note1" || nounString == "note1")
      {
        returnString = "note1";
      }
      else if( nounString == "RecordPlayer" || nounString == "recordplayer" || nounString == "player")
      {
        returnString = "recordplayer";
      }
      else if( nounString == "Tapestry" || nounString == "tapestry" || nounString == "Tapestry1" || nounString == "tapestry1")
      {
        returnString = "tapestry1";
      }
      else if( nounString == "Dishes" || nounString == "dishes" || nounString == "DirtyDishes" )
      {
        returnString = "dirtydishes";
      }
      else if( nounString == "Record" || nounString == "record" || nounString == "Record1")
      {
        returnString = "record1";
      }
      else if( nounString == "Padlock" || nounString == "padlock")
      {
        returnString = "padlock";
      }
      else if( nounString == "Piano" || nounString == "piano" || nounString == "Piano1")
      {
        returnString = "piano1";
      }
      else if( nounString == "Key" || nounString == "key" || nounString == "Key1")
      {
        returnString = "key1";
      }
      else if( nounString == "2ndFloorDoors")
      {
        returnString = "2ndfloordoors";
      }
      else if( nounString == "ClosetShelf" || nounString == "closetshelf" ||  nounString == "ClosetShelves" || nounString == "closetshelves")
      {
        returnString = "ClosetShelf";
      }
      else if( nounString == "LibraryShelf" || nounString == "libraryshelf" ||  nounString == "LibraryShelves" || nounString == "libraryshelves")
      {
        returnString = "LibraryShelves";
      }
      else if( nounString == "Camera" || nounString == "camera")
      {
        returnString = "camera";
      }
      else if( nounString == "Puzzle1" || nounString == "puzzle1" || nounString == "Puzzle" || nounString == "puzzle")
      {
        returnString = "puzzle1";
      }
      else if( nounString == "VisionHallway2" || nounString == "visionhallway2")
      {
        returnString = "visionhallway2";
      }
      else if( nounString == "Firestarter" || nounString == "firestarter" || nounString == "fire" || nounString == "Fire" || nounString == "starter" || nounString == "Starter")
      {
        returnString = "firestarter";
      }
      else if( nounString == "SolvingPuzzle1" || nounString == "solvingpuzzle1" || nounString == "solving" || nounString == "Solving")
      {
        returnString = "solvingpuzzle1";
      }
      else if( nounString == "Ghosts" || nounString == "ghosts")
      {
        returnString = "ghosts";
      }
      else if( nounString == "WoodBox1" || nounString == "woodbox1" || nounString == "box"|| nounString == "Box")
      {
        returnString = "woodbox1";
      }
      else if( nounString == "Spectacles" || nounString == "spectacles" || nounString == "glasses" || nounString == "Glasses")
      {
        returnString = "spectacles";
      }
      else if( nounString == "Floor2Lamps" || nounString == "floor2lamps")
      {
        returnString = "Floor2Lamps";
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
      (verbString.compare("read") == 0) ||
      (verbString.compare("Look") == 0 )||
    	(verbString.compare("Investigate") == 0 ) ||
      (verbString.compare("Read") == 0) ||
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
          (verbString.compare("clean") == 0 ) ||
          (verbString.compare("Clean") == 0 ) ||
          (verbString.compare("Wash") == 0 ) ||
          (verbString.compare("wash") == 0 ) ||
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
          (verbString.compare("Open") == 0 ) ||
          (verbString.compare("lift") == 0 ) ||
          (verbString.compare("Lift") == 0 ))  //sets validVerb to 5
	{
		return (validVerbs)open;
	}
  else if ((verbString.compare("close") == 0 ) ||
          (verbString.compare("Close") == 0 ) ||
          (verbString.compare("shut") == 0 ) ||
          (verbString.compare("Shut") == 0 ) ||
          (verbString.compare("seal") == 0 ) ||
          (verbString.compare("Seal") == 0 ))//sets validVerb to 6
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
            (verbString.compare("Stuff") == 0 ) ||
            (verbString.compare("stuff") == 0 ) ||
            (verbString.compare("Items") == 0 ) ||
            (verbString.compare("items") == 0 ) ||
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

// returns lowercase string
// http://www.cplusplus.com/reference/locale/tolower/
std::string Parser::strToLowercase(std::string mixedStr)
{
  std::locale loc;

  for (std::string::size_type i=0; i<mixedStr.length(); ++i)
    mixedStr[i] = std::tolower(mixedStr[i],loc);
  return mixedStr;
}
