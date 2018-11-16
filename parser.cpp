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
	// Limited capabilities if we don't get a GS pointer -
	// basically will only parse verbs correctly
	GS = NULL ;
}
Parser::Parser(GameState *GameStatePtr)
{
	GS = GameStatePtr;

}

Parser::~Parser()
{
	// need to delete the memory allocated
}

Choice * Parser::ParseLine(){
	return ParseLine("");
}

Choice * Parser::ParseLine(std::string inString){
    string mystr;
		string myNoun = NOTFOUND;
    string myVerb = NOTFOUND;
    string mySubject = NOTFOUND;
		list <string> words;
		string oneword;
		Choice * userChoice = new Choice();
		validVerbs action;

		if ( inString.compare("") == 0 ) {
			// Get input from the user / keyboard
			cout << "What would you like to do? ";
			getline (cin, mystr);
		}
		else {
			mystr = inString;
		}

		// Tokenize the user input with the strtok command,
		// and push all the tokens onto the ordered list 'words'
		char * sptr;
		char myCstr[mystr.length()+1];
		strcpy(myCstr, mystr.c_str());
		sptr = strtok(myCstr, " ");
		while (sptr != NULL ) {
			oneword = sptr;
			words.push_back(oneword);
			sptr = strtok(NULL, " ");
		}

		// I think this is leftover from the previous tokenization method,
		// commenting it out for now.
		/*
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
		if (DEBUG_PARSER) cout << "string size = " << mystr.size() << endl;
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
			if (DEBUG_PARSER) cout << "array[" << i << "] " << array[i] << endl;
    }
		if (DEBUG_PARSER) std::cout << "num args: " << j << std::endl;
		*/

		// Need to convert this to use the "words" list if we're going to keep it.
    string array[1];
		int i=0;
		if((array[i] == "pick" && array[i+1] == "up") ||
			(array[i] == "Pick" && array[i+1] == "up") ||
			(array[i] == "Pick" && array[i+1] == "Up"))
		{
			array[i] = "pickup";
			// TODO: should shuffle-delete array[i+1]
		}

    //while ( ! words.empty() && myVerb.compare(NOTFOUND) == 0 ) //nick changed this
    // while (( ! words.empty() && userChoice->Verb == 16 )
  	userChoice->inputVerb = words.front();
		for (list<string>::iterator iter=words.begin(); iter!=words.end(); iter++)
    {
  		action = getVerb ( *iter );
      if (action != (validVerbs)unknown)
      {
        userChoice->Verb = action;
  			userChoice->inputVerb = *iter;
  			//words.pop_front();
  			*iter = "";  // this is lame
				break;
      }
    }
  	while ( ! words.empty())
		{
			if (myNoun.compare(NOTFOUND) == 0 )
			{
				myNoun = getNoun(words.front());
      	if (myNoun.compare(NOTFOUND) != 0) {
					userChoice->inputNoun = words.front();
					words.pop_front();
					continue;
				}
			}
			if (mySubject.compare(NOTFOUND) == 0 )
			{
				mySubject = getSubject(words.front());
			}
			words.pop_front();
		}
		userChoice->Noun = strToLowercase(myNoun);
		userChoice->Subject = strToLowercase(mySubject);

	if (DEBUG_PARSER)  cout << "THIS IS THE INPUT VERB: " << userChoice->inputVerb << endl;
	if (DEBUG_PARSER)  cout << "THIS IS THE INPUT NOUN: " << userChoice->inputNoun << endl;
	if (DEBUG_PARSER)  cout << "THIS IS THE VERB: " << userChoice->Verb << endl;
	if (DEBUG_PARSER)  cout << "THIS IS THE NOUN: " << userChoice->Noun << endl;
	if (DEBUG_PARSER)  cout << "THIS IS THE SUBJECT: " << userChoice->Subject << endl;

	if (DEBUG_PARSER) std::cout << std::endl << "===== end   Parser::ParseLine" << std::endl;
	return userChoice;
}

Choice * Parser::TestLine()
{
	if (DEBUG_PARSER) std::cout << "===== begin Parser::TestLine" << std::endl;
	if ( GS == NULL ) std::cout << "WARNING : Attempting to test with NULL GS " << std::endl;
	// Read one line from ifstream, parse it as usual
	std::string inputString;
	std::ifstream *inputFile = & ( GS->GameTestFile);

	if ( ! inputFile ) {
		std::cout << "ERROR: inputFile is NULL"<< std::endl;
		exit(1);
	}
	else if ( inputFile == NULL ) {
		std::cout << "ERROR: inputFile is NULL here"<< std::endl;
		exit(1);
	}
	if ( ! inputFile->good() ) {
		std::cout << "WARNING: inputFile is empty, switching to user input"<< std::endl;
		GS->GameTest=false;
		return ParseLine("");
	}
	else {
		std::getline(*inputFile, inputString);
		if ( inputString.substr(0,1).compare("#") == 0 )
		{
			//Found a comment, print instead of executing
			std::cout << inputString << std::endl;
			return TestLine(); // Go to the next line in the file
		}
		std::cout << "TEST command = " << inputString<< std::endl;
		return ParseLine(inputString);
	}
}

std::string Parser::getNoun(std::string nounString) {
	if (DEBUG_PARSER) std::cout << "===== begin Parser::getNoun, noun is '" << strToLowercase(nounString) << "'" << std::endl;

	std::string returnString;
  std::string lcNounString = strToLowercase(nounString);
	std::string tmpString;

	returnString = getRoom(lcNounString);
	if ( returnString.compare(NOTFOUND) == 0 ) {
		returnString = getFeature(lcNounString);
	}

	return returnString;
}

std::string Parser::getRoom(std::string lcNounString) {

	if (DEBUG_PARSER) std::cout << "===== begin Parser::getRoom, noun is '" << lcNounString << "'" << std::endl;
	std::string returnString = NOTFOUND;

      if(lcNounString == "west"
				|| lcNounString == "east"
				|| lcNounString == "north"
				|| lcNounString == "south"
				|| lcNounString == "up"
				|| lcNounString == "upstairs"
				|| lcNounString == "down"
        || lcNounString == "southwest" || lcNounString == "southcenter"
        || lcNounString == "southeast" || lcNounString == "right"
        || lcNounString == "left" || lcNounString == "ne" || lcNounString == "se"
        || lcNounString == "n" || lcNounString == "s"
        || lcNounString == "1" || lcNounString == "2" || lcNounString == "3"
        || lcNounString == "4" || lcNounString == "5" || lcNounString == "stairs")
      {
        return lcNounString;
      }

      if(lcNounString == "Ballroom" || lcNounString == "ballroom")
      {
        returnString = "ballroom";
      }
      else if(lcNounString == "Foyer" || lcNounString == "foyer")
      {
        returnString = "foyer";
      }
      else if( lcNounString == "Kitchen" || lcNounString == "kitchen")
      {
        returnString = "kitchen";
      }
      else if( lcNounString == "Conservatory" || lcNounString == "conservatory")
      {
        returnString = "conservatory";
      }
      else if( lcNounString == "Pantry" || lcNounString == "pantry")
      {
        returnString = "pantry";
      }
      else if( lcNounString == "Basement" || lcNounString == "basement")
      {
        returnString = "basement";
      }
      else if( lcNounString == "Door1" || lcNounString == "door1" || lcNounString == "Bedroom1" || lcNounString == "bedroom1")
      {
        returnString = "bedroom1";
      }
      else if(  lcNounString == "Door2" || lcNounString == "door2" || lcNounString == "Nursery" || lcNounString == "nursery" )
      {
        returnString = "nursery";
      }
      else if(  lcNounString == "Door3" || lcNounString == "door3" || lcNounString == "Library" || lcNounString == "library" )
      {
        returnString = "library";
      }
      else if(  lcNounString == "Door4" || lcNounString == "door4" || lcNounString == "Study" || lcNounString == "study" )
      {
        returnString = "study";
      }
      else if(  lcNounString == "Door5" || lcNounString == "door5" || lcNounString == "Balcony" || lcNounString == "balcony" )
      {
        returnString = "balcony";
      }
      else if( lcNounString == "Balcony" || lcNounString == "balcony" )
      {
        returnString = "balcony";
      }
      else if( lcNounString == "2nd" || lcNounString == "second" || lcNounString == "Second")
      {
        returnString = "2ndFloorHallway";
      }
			else if( lcNounString == "hall" || lcNounString == "3rd" || lcNounString == "third" || lcNounString == "Third")
      {
        returnString = "3rdFloorHallway";
      }
      else if( lcNounString == "Bedroom2" || lcNounString == "bedroom2")
      {
        returnString = "bedroom2";
      }
      else if( lcNounString == "Bedroom3" || lcNounString == "bedroom3")
      {
        returnString = "bedroom3";
      }
      else if( lcNounString == "MasterBedroom" || lcNounString == "masterbedroom" || lcNounString == "master")
      {
        returnString = "masterbedroom";
      }
      else if( lcNounString == "Closet" || lcNounString == "closet")
      {
        returnString = "closet";
      }
      else if( lcNounString == "Library" || lcNounString == "library")
      {
        returnString = "library";
      }
			else if( lcNounString == "bedroom" || lcNounString == "Bedroom")
      {
        returnString = "bedroom";
      }
			else if( lcNounString == "hallway" || lcNounString == "Hallway")
      {
        returnString = "hallway";
      }
			else if( lcNounString == "laundry" || lcNounString == "Laundry")
      {
        returnString = "laundry";
      }

	return returnString;
}

std::string Parser::getFeature(std::string lcNounString) {
	if (DEBUG_PARSER) std::cout << "===== begin Parser::getFeature, noun is '" << lcNounString << "'" << std::endl;

	std::string returnString = NOTFOUND;

	if (GS !=NULL){
		returnString = GS->housePtr->findFeatureByName(lcNounString);
	}
	/*
	if (DEBUG_PARSER) std::cout << "   findFeatureByNamer returns '" << returnString << "'" << std::endl;

			if( lcNounString == "book" || lcNounString == "Book")
			{
				returnString = "book1";
			}
      else if( lcNounString == "Rug" || lcNounString == "rug" || lcNounString == "Rug1" || lcNounString == "rug1")
      {
        returnString = "rug1";
      }
      else if( lcNounString == "Note" || lcNounString == "note" || lcNounString == "Paper" || lcNounString == "paper" || lcNounString == "Note1" || lcNounString == "note1")
      {
        returnString = "note1";
      }
      else if( lcNounString == "RecordPlayer" || lcNounString == "recordplayer" || lcNounString == "player")
      {
        returnString = "recordplayer";
      }
      else if( lcNounString == "Tapestry" || lcNounString == "tapestry" || lcNounString == "Tapestry1" || lcNounString == "tapestry1")
      {
        returnString = "tapestry1";
      }
      else if( lcNounString == "Dishes" || lcNounString == "dishes" || lcNounString == "DirtyDishes" )
      {
        returnString = "dirtydishes";
      }
      else if( lcNounString == "Record" || lcNounString == "record" || lcNounString == "Record1")
      {
        returnString = "record1";
      }
      else if( lcNounString == "Padlock" || lcNounString == "padlock")
      {
        returnString = "padlock";
      }
      else if( lcNounString == "Piano" || lcNounString == "piano" || lcNounString == "Piano1")
      {
        returnString = "piano1";
      }
      else if( lcNounString == "Key" || lcNounString == "key" || lcNounString == "Key1")
      {
        returnString = "key1";
      }
      else if( lcNounString == "2ndFloorDoors")
      {
        returnString = "2ndfloordoors";
      }
      else if( lcNounString == "ClosetShelf" || lcNounString == "closetshelf" ||  lcNounString == "ClosetShelves" || lcNounString == "closetshelves")
      {
        returnString = "closetshelf";
      }
      else if( lcNounString == "LibraryShelf" || lcNounString == "libraryshelf" ||  lcNounString == "LibraryShelves" || lcNounString == "libraryshelves")
      {
        returnString = "libraryshelves";
      }
      else if( lcNounString == "Camera" || lcNounString == "camera")
      {
        returnString = "camera";
      }
      else if( lcNounString == "Puzzle1" || lcNounString == "puzzle1" || lcNounString == "Puzzle"
						|| lcNounString == "puzzle"  || lcNounString == "sentence" || lcNounString == "Sentence"
						|| lcNounString == "words"  || lcNounString == "Words" )
      {
        returnString = "puzzle1";
      }
      else if( lcNounString == "VisionHallway2" || lcNounString == "visionhallway2")
      {
        returnString = "visionhallway2";
      }
      else if( lcNounString == "Firestarter" || lcNounString == "firestarter" || lcNounString == "fire" || lcNounString == "Fire" || lcNounString == "Lighter" || lcNounString == "lighter")
      {
        returnString = "lighter";
      }
      else if( lcNounString == "Ghosts" || lcNounString == "ghosts")
      {
        returnString = "ghosts";
      }
      else if( lcNounString == "WoodBox1" || lcNounString == "woodbox1" || lcNounString == "box"|| lcNounString == "Box")
      {
        returnString = "woodbox1";
      }
      else if( lcNounString == "Spectacles" || lcNounString == "spectacles" || lcNounString == "glasses" || lcNounString == "Glasses")
      {
        returnString = "spectacles";
      }
      else if( lcNounString == "Floor2Lamps" || lcNounString == "floor2lamps")
      {
        returnString = "floor2lamps";
      }
			else if( lcNounString == "canned" || lcNounString == "food" || lcNounString == "Canned" || lcNounString == "Food")
      {
        returnString = "cannedfood";
      }
	*/
	if (DEBUG_PARSER) std::cout << "=====   end Parser::getFeature, noun is '" << returnString << "'" << std::endl;
	return returnString;
}

std::string Parser::getSubject(std::string subjectstring) {
	std::string returnString = NOTFOUND;
  std::string lcNounString = strToLowercase(subjectstring);
  if(subjectstring == "eac" || subjectstring == "EAC")
  {
    returnString = "eac";
  }
  if(subjectstring == "1894")
  {
    returnString = "1894";
  }
  if(subjectstring == "hello" || subjectstring == "Hello")
  {
    returnString = "hello";
  }

  return returnString;
}

validVerbs
Parser::getVerb(std::string verbString) {
  string mystr;
  Choice * userChoice = new Choice();

	userChoice->inputVerb = verbString; // Save this for overrides

	if (DEBUG_PARSER) std::cout << "===== begin Parser::getVerb, verb is '" << verbString << "'" << std::endl;
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
  (verbString.compare("Save") == 0) || (verbString.compare("savegame") == 0) || (verbString.compare("Savegame") == 0 ))
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
      (verbString.compare("Load") == 0) || (verbString.compare("loadgame") == 0) ||  (verbString.compare("Loadgame") == 0))           //sets validVerb to 13
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
  else if ((verbString.compare("hint") == 0 )  ||
            (verbString.compare("Hint") == 0 ))    //sets validVerb to
  {
    return (validVerbs)hint;
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
