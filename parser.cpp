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

Choice * Parser::ParseLine(Room * rPtr){
	return ParseLine(rPtr, "");
}

Choice * Parser::ParseLine(Room * rPtr, std::string inString){
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

		// This Is A Terrible Hack  :) Turn "pick up" into a single word
		list<string>::iterator itTwo=words.begin();
		list<string>::iterator itOne = itTwo++;
		if (   ( (*itOne).compare("pick") ==0 || (*itOne).compare("Pick") == 0)
				&& ( (*itTwo).compare("up") ==0 || (*itTwo).compare("Up") == 0 )) 
		{
			
			if (DEBUG_PARSER) std::cout << "Fixing Pick Up" << std::endl;

			(*itOne) = "pickup";
			(*itTwo) = "";
		}
		else {
			if (DEBUG_PARSER) std::cout << "No PickUp found, '" << (*itOne) << "','" << (*itTwo) << std::endl;
		}

		// Main parser logic: Go through the words list and recognize verbs
		// and nouns
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
				myNoun = getNoun(words.front(), rPtr);
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
	//userChoice->Noun = strToLowercase(myNoun);
	userChoice->Noun = myNoun;
	userChoice->Subject = strToLowercase(mySubject);

	if (DEBUG_PARSER)  cout << "THIS IS THE INPUT VERB: " << userChoice->inputVerb << endl;
	if (DEBUG_PARSER)  cout << "THIS IS THE INPUT NOUN: " << userChoice->inputNoun << endl;
	if (DEBUG_PARSER)  cout << "THIS IS THE VERB: " << userChoice->Verb << endl;
	if (DEBUG_PARSER)  cout << "THIS IS THE NOUN: " << userChoice->Noun << endl;
	if (DEBUG_PARSER)  cout << "THIS IS THE SUBJECT: " << userChoice->Subject << endl;

	if (DEBUG_PARSER) std::cout << std::endl << "===== end   Parser::ParseLine" << std::endl;
	return userChoice;
}

Choice * Parser::TestLine(Room * rPtr)
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
		return ParseLine(rPtr, "");
	}
	else {
		std::getline(*inputFile, inputString);
		if ( inputString.substr(0,1).compare("#") == 0 )
		{
			//Found a comment, print instead of executing
			std::cout << inputString << std::endl;
			return TestLine(rPtr); // Go to the next line in the file
		}
		std::cout << "TEST command = " << inputString<< std::endl;
		return ParseLine(rPtr, inputString);
	}
}

std::string Parser::getNoun(std::string nounString, Room * rPtr) {
	if (DEBUG_PARSER) std::cout << "===== begin Parser::getNoun, noun is '" << strToLowercase(nounString) << "'" << std::endl;

	std::string returnString;
  std::string lcNounString = strToLowercase(nounString);
	std::string tmpString;

	returnString = getRoom(lcNounString, rPtr);
	if ( returnString.compare(NOTFOUND) == 0 ) {
		returnString = strToLowercase(getFeature(lcNounString));
	}

	return returnString;
}

std::string Parser::getRoom(std::string lcNounString, Room * rPtr) {

	if (DEBUG_PARSER) std::cout << "===== begin Parser::getRoom, noun is '" << lcNounString << "'" << std::endl;
	std::string returnString = NOTFOUND;
	std::string tmpString = "";

	if (rPtr !=NULL){
		tmpString = rPtr->getExitRoomByKey(lcNounString, false);
		if ( tmpString.length() > 0 ) returnString = tmpString;
	}
	else {
		std::cout << "ERROR:  Room pointer is NULL in Parser::getRoom!"
							<< "        Game may not work correctly!" << std::endl;
	}
	if (DEBUG_PARSER) std::cout << "===== end Parser::getRoom, returning '" << returnString << "'" << std::endl;
	return returnString;
}

std::string Parser::getFeature(std::string lcNounString) {
	if (DEBUG_PARSER) std::cout << "===== begin Parser::getFeature, noun is '" << lcNounString << "'" << std::endl;

	std::string returnString = NOTFOUND;

	if (GS !=NULL){
		returnString = GS->housePtr->findFeatureByName(lcNounString);
	}
	else {
		std::cout << "ERROR:  GameState pointer is NULL in Parser::getFeature" 
							<< "        Game may not work correctly!" << std::endl;
	}
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
  if(subjectstring == "amulet" || subjectstring == "Amulet")
  {
		returnString = getFeature("amulet");
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
      (verbString.compare("where") == 0) ||
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
          //(verbString.compare("clean") == 0 ) ||
          //(verbString.compare("Clean") == 0 ) ||
          //(verbString.compare("Wash") == 0 ) ||
          //(verbString.compare("wash") == 0 ) ||
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
