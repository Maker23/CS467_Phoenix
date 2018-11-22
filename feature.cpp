/*
 Notes:
 	Everything that can be used in a room is a feature. Features have a differfent quality, check the type enum for the options.
 	It is loaded into a MAP of Features objects located in house (just liek the room objects)
 	To get a feature's object ptr, call it with house->getFeaturePtr(string key);  key is the name of the file, and is used in the rooms vector of what features it has.

*/


#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <stack>
#include <vector>
#include <typeindex>
#include <sstream>
#include <algorithm>
#include <stdlib.h>  // for rand. Not seeding it for now.
#include "engine.hpp"
#include "house.hpp"
#include "room.hpp"
#include "feature.hpp"
#include "parser.hpp"

using namespace std;

/*
Constructor for feature class. Takes a string of the feature file that's being
   read from and then processes the file to a feature object. 
*/
Feature::Feature(string fileToOpen)
{
	ifstream featurefile;
	string lineStr, tempStr;


	// set defaults
	key = "";
	solved = false;
	weight = 1; // Zero is for intangible objects like ghosts
	seen = 0;
	dropped = 0;

	//name = "";
	//triggers = "";
	//dependsOn = "";
	//uses = "";
	//neverSeenText = "";
	//observeText = "";
	//seenText = "";
	//solvedText = "";
	//examineText = "";
	//solvingText = "";
	//takingText = "";
	//droppingText = "";
	//usingText = "";
	//textToSolve = "";

	featurefile.open(fileToOpen);

	const char * nameCstr;
	char nwords[256];
	char * nptr;

	if (featurefile.is_open()) {
		while (std::getline(featurefile, lineStr))  { // read line by line

			if (lineStr.substr(0,1).find("#") != std::string::npos )
			{
				// Skip lines that begin with # for comments
				continue;
			}
			// big section of if statments to get the beginning of each line,
			// and process it accordingly
			if(lineStr.find("NAME: ") != std::string::npos)
			{
				tempStr = lineStr.substr(6, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found NAME " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
				{
					if ( tempStr.find("|") == std::string::npos) {
						featureStrings["name"] = tempStr;
					}
					else
					{
						nameCstr = tempStr.c_str();
						strcpy(nwords, nameCstr);
						nptr = strtok (nwords, "|");
						if (nptr != NULL) {
							tempStr.erase(tempStr.find("|"));
							nptr = strtok(NULL, ", ");
							while (nptr != NULL ) {
								if (DEBUG_FEATURES) std::cout << "     Finding name alias for " << tempStr << ", token = '" << nptr << "'" << std::endl;
								aliases[nptr] = tempStr;
								nptr = strtok(NULL, ", ");
							}
						}
						featureStrings["name"] = tempStr;
					}
				}
				if (DEBUG_FEATURES) { std::cout << "Feature() - setting NAME to " << tempStr << std::endl;}
			}
	
			if(lineStr.find("SOLVED: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(8, lineStr.length()-1);
				if(tempStr.compare("true") == 0 || tempStr.compare("1") == 0){
					if (DEBUG_FEATURES) { std::cout << "Feature() - setting solved to true" << std::endl;}
					solved = true;
				}
				else if(tempStr.compare("false") == 0 || tempStr.compare("0") == 0){
					solved = false;
				}
				else if (DEBUG_FEATURES) { std::cout << "Feature() - found unknown 'solved' valaue " << tempStr << std::endl;}
			}
			if(lineStr.find("OPEN: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(6, lineStr.length()-1);
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					featureStrings["open"] = tempStr;
				if (DEBUG_FEATURES) { std::cout << "Feature() - Open: " << tempStr << std::endl;}
			}

			if(lineStr.find("SOLVES_HERE: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(13, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found SOLVES_HERE " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					// Chunk up the string
					nameCstr = tempStr.c_str();
					strcpy(nwords, nameCstr);
					nptr = strtok (nwords, ", ");
					while (nptr != NULL ) {
						if (DEBUG_FEATURES) std::cout << "     Finding SOLVES_HERE token = '" << nptr << "'" << std::endl;
						solvesHere.push_back(nptr);
						nptr = strtok(NULL, ", ");
					}
					//featureStrings["solvesHere"] = tempStr;
			}

			if(lineStr.find("TEXT_NEVERSEEN: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(16, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_NEVERSEEN " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//neverSeenText = tempStr;
					featureStrings["neverSeenText"] = tempStr;
			}

			if(lineStr.find("TEXT_SEEN: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(11, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_SEEN " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//seenText = tempStr;
					featureStrings["seenText"] = tempStr;
			}

			if(lineStr.find("TEXT_EXAMINE: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_EXAMINE " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//examineText = tempStr;
					featureStrings["examineText"] = tempStr;
			}

			if(lineStr.find("TEXT_EXAMINE_SOLVED: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(21, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_EXAMINE_SOLVED " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//solvedText = tempStr;
					featureStrings["solvedText"] = tempStr;
			}

			if(lineStr.find("TEXT_SOLVING: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_SOLVING " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//solvingText = tempStr;
					featureStrings["solvingText"] = tempStr;
			}


			if(lineStr.find("TEXT_TAKING: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(13, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_TAKING " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//takingText = tempStr;
					featureStrings["takingText"] = tempStr;
			}

			if(lineStr.find("TEXT_DROPPING: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(15, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_DROPPING " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//droppingText = tempStr;
					featureStrings["droppingText"] = tempStr;
			}

			if(lineStr.find("TEXT_ONFLOOR: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_ONFLOOR " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//solvingText = tempStr;
					featureStrings["onfloorText"] = tempStr;
			}

			if(lineStr.find("TEXT_HINT: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(11, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_HINT " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					featureStrings["hintText"] = tempStr;
			}

			if(lineStr.find("TEXT_USING: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(12, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_USING " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//usingText = tempStr;
					featureStrings["usingText"] = tempStr;
			}

			if(lineStr.find("WEIGHT: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(8, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found WEIGHT " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					weight = stoi(tempStr);
			}

			if(lineStr.find("TRIGGERS: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(10, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TRIGGERS " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//triggers = strToLowercase(tempStr);
					featureStrings["triggers"] = strToLowercase(tempStr);
			}

			if(lineStr.find("DEPENDS_ON: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(12, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DEPENDS_ON " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//dependsOn = strToLowercase(tempStr);
					featureStrings["dependsOn"] = strToLowercase(tempStr);
			}

			if(lineStr.find("DEPENDS_ON_INVENTORY: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(22, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DEPENDS_ON " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//dependsOn = strToLowercase(tempStr);
					featureStrings["dependsOnInventory"] = strToLowercase(tempStr);
			}

			if(lineStr.find("UNLOCKS: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(9, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found UNLOCKS " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//dependsOn = strToLowercase(tempStr);
					featureStrings["unlocks"] = strToLowercase(tempStr);
			}

			if(lineStr.find("USES: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(6, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found USES " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//uses = strToLowercase(tempStr);
					featureStrings["uses"] = strToLowercase(tempStr);
			}

			if(lineStr.find("TEXT_TO_SOLVE: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(15, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_TO_SOLVE " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					//textToSolve = strToLowercase(tempStr);
					featureStrings["textToSolve"] = strToLowercase(tempStr);
			}

			if(lineStr.find("ACTIONS: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(9, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found ACTIONS " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
				{
					// 
					//
					//
					std::string keywordStr = tempStr;
				   std::stringstream mystream (keywordStr);
				   const char * keywordCstr = keywordStr.c_str();
					std::string st;
					char * cptr;
					Parser parse;
					validVerbs kVerb;
					if (DEBUG_FEATURES) { std::cout << "char * Actions = " << keywordCstr << std::endl;}

					char words[keywordStr.length() +1];
					strcpy(words,keywordCstr);

					//1. look for the first part of the string using getVerb, if no result return unknonw
					//2. map the remaining strings to the enum
					cptr = strtok(words, "| ");
					if (cptr) {
						if (DEBUG_FEATURES) { std::cout << "      token = '" << cptr << "'" << std::endl;}
						kVerb = parse.getVerb(cptr);
						if ( kVerb != unknown ) {
							cptr = strtok(NULL, ", ");
							while (cptr != NULL) {
								// convert to string...
								st = cptr;
								actions[strToLowercase(st)] = kVerb;
								if (DEBUG_FEATURES) { std::cout << "      token = '" << st << "'" << std::endl;}
								cptr = strtok(NULL, ", ");
							}
						}
					}
				}
				if (DEBUG_FEATURES) { std::cout << "Total Actions " << actions.size() << std::endl;}
			}
			
		}
	}
	else
	{
		cout << "Error opening feature file '" <<  fileToOpen << "\n";
	}

}

// Destructor
Feature::~Feature()
{

}

std::string Feature::getName()
{
	return getStringByKey("name");
}

// returns the text string by key, or "" if key isn't found.
std::string Feature::getStringByKey(std::string keyString)
{
	if ( featureStrings.find(keyString) == featureStrings.end() )
	{
		return "";
	} else
	{
		return featureStrings[keyString];
	}
}

/* *********************************************************
 *
 *
 * ********************************************************* */
void Feature::Examine()
{
  this->examineFeature();
	return;
}

void Feature::examineFeature()
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::examineFeature()" << std::endl;}
	std::cout << getStringByKey("examineText") << std::endl;
	return;
}

vector<std::string> * Feature::getSolvesHere()
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::getSolvesHere()" << std::endl;}
	return & solvesHere;
}


void Feature::useFeature(GameState *GS, Feature * Subject)
{
	std::string nounUses;
	Feature * checkFeature=NULL;
	//std::string solvesHere;
	//Feature * solvesFeature=NULL;
	bool printedSomething=false;

	//if ( getName().compare(getNoun("puzzle") == 0 ) {
	// Can't think of a way not to hardcode these
	if ( getName().compare("Puzzle") == 0 ) {
		if (DEBUG_PUZZLE) { std::cout << "      Going off to solve a puzzle " << std::endl;}
		bool solved = GS->puzzle->solvePuzzle();
		if ( ! solved ) {
			return;
		}
	}
	else {
		if (DEBUG_PUZZLE) { std::cout << "      getName '" << getName() << "' does not match 'Puzzle'" << std::endl;}
	}
	
	if ( getStringByKey("solvingText").compare("") != 0 )
	{
		std::cout << getSolvingText() << std::endl;
		printedSomething=true;
	}
	if (DEBUG_FEATURES) { std::cout << "      Setting isSolved to true" << std::endl;}
	setSolved(true);


  //solvesHere = getStringByKey("solvesHere");
  //if ( solvesHere.compare("") != 0 && GS->featureWithinReach(currentRoom,solvesHere)) {
  //  solvesFeature = GS->housePtr->getFeaturePtr(solvesHere);
  //  if (solvesFeature) {
  //    solvesFeature->setSolved(true);
  //  }
 // }

	if ( getStringByKey("usingText").compare("") != 0 )
	{
		if (DEBUG_FEATURES) { std::cout << "      printing primary usingText" << std::endl;}
		std::cout << getUsingText() << std::endl;
		printedSomething=true;
	}
	nounUses = getUses();
	checkFeature = GS->housePtr->getFeaturePtr(nounUses);
	if ( nounUses.compare("") != 0  &&  checkFeature != NULL )
	{
		// If there's a dependency get the usingText from that
		if ( checkFeature->getStringByKey("usingText").compare("") != 0 ) {
			if (DEBUG_FEATURES) { std::cout << "      printing secondary usingText" << std::endl;}
			std::cout << checkFeature->getUsingText() << std::endl;
			checkFeature->setSolved(true);
			printedSomething=true;
		}
	}

	if (! printedSomething) std::cout << "Nothing much happens." << std::endl;
	return;
}

bool Feature::solveFeature(GameState *GS, std::string solveString="")
{
	if(DEBUG_FEATURES) std::cout << "[DEBUG_FEATURES Features::solveFeature] Start Function" << std::endl;
	bool completedSolveString = false;
	bool dependencyCompleted = false;

	if(solved)
		return true;  // it's already done.
	
	if(solveString.length() == 0) 
	{
		if(DEBUG_FEATURES) std::cout << "     solveString.length() == 0, set completedSolveString true" << std::endl;
	}
	else
	{
		if(strToLowercase(solveString).compare(strToLowercase(getStringByKey("textToSolve"))) == 0)
		{
			if(DEBUG_FEATURES) std::cout << "     strings match, set completedSolveString true" << std::endl;
			completedSolveString = true;
		}
	}

	if(getStringByKey("dependsOn").length() > 0) 
	{
		// need to check if the dependsOn is completed
		Feature *feature = GS->housePtr->getFeaturePtr(getStringByKey("dependsOn"));
		dependencyCompleted = feature->isSolved();
		if(DEBUG_FEATURES) std::cout << "     feature->isSolved() returned " << dependencyCompleted << std::endl;
	}
	else  // no dependency, so it's completed
	{
		if(DEBUG_FEATURES) std::cout << "     no dependency, set dependencyCompleted true" << std::endl;
		dependencyCompleted = true;
	}

	if((solveString.length() == 0 && dependencyCompleted) || completedSolveString)
	{
		if(DEBUG_FEATURES) std::cout << "[DEBUG_FEATURES Features::solveFeature] End Function - set as solved, Return true" << std::endl;
		solved = true;
		std::cout << getStringByKey("solvingText") << std::endl;
		return true;
	}
	else
	{
		if(DEBUG_FEATURES) std::cout << "[DEBUG_FEATURES Features::solveFeature] End Function - Return false" << std::endl;
		return false;
	}
}

void Feature::takeFeature(GameState *GS, Room * Rm,Feature * Subject)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::takeFeature()" << std::endl;}
	if ( weight >= 10 ) 
	{
		std::cout << "The " << getName() << " is too heavy to pick up." << std::endl;
		return;
	}
	else {
		dropped = false;
		if ( getStringByKey("takingText").length() > 0 )
		{
			std::cout << getStringByKey("takingText") << std::endl;
		}
		else
		{
			std::cout << "You pick up the " << getName() << std::endl;
		}
		GS->Holding.push_back(this);
		//if (DEBUG_BRENT) std::cout << "[DEBUG_BRENT] Feature::takeFeature() getKeyName: " << this->getKeyName() << std::endl;
		Rm->deleteFeature(getKeyName());
	}
}

void Feature::dropFeature(GameState *GS, Room * Rm,Feature * Subject)
{
	dropFeature(GS, Rm, Subject, false);
	return;
}

void Feature::dropFeature(GameState *GS, Room * Rm,Feature * Subject, bool Silent)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::dropFeature()" << std::endl;}
	std::string FName = getName();
	std::string CName = "";
	dropped = true;

	if (! Silent) {
		if ( getDroppingText().length() != 0 ) {
			std::cout << getDroppingText() << std::endl;
		}
		else {
			std::cout << "You drop the " << FName << std::endl;
		}
	}
	for (std::vector<Feature*>::iterator iter = GS->Holding.begin(); iter != GS->Holding.end(); iter ++ ) {
		CName = (*iter)->getName();
		if ( CName.compare(FName) == 0 ) {
			if (DEBUG_FEATURES) { std::cout << "      DEBUG dropping " << FName << std::endl;}
			GS->Holding.erase(iter);
			// This messes with iter++ and causes a SegFault if we don't break. 
			// TODO: investigate how this should really be done
			break;
		}
	}
	if (DEBUG_FEATURES) { std::cout << "      DEBUG Adding " << getKeyName() << std::endl;}
	Rm->addFeature(getKeyName(), GS);

	return;
}

void Feature::hurlFeature( GameState * GS, Room * Rm, Feature * Subject)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::hurlFeature()" << std::endl;}
	std::string hurlWhere;
	bool Silent = true;
	dropped = true;

	std::cout << "You throw the " << getName();
	if ( Subject ) {
		// Is there a subject?
		std::cout << " at the " << Subject->getName();
	}
	else if ( weight > 5 ){
		std::cout << ". It lands a couple of feet away with a thud." << std::endl;
	}
	else {
		std::cout << " across the room. It lands on the floor." << std::endl;
	}
	dropFeature (GS, Rm, Subject, Silent);
}

void Feature::openFeature(GameState * GS, Feature * Subject)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::openFeature()" << std::endl;}
	// We don't implement anything special for this actually
	// This is entirely implemented with override verbs
	//return useFeature(GS, Subject);

	std::cout << "The " << getName() << " can't be opened." << std::endl;
	return;
}

void Feature::hitFeature(Feature * Subject)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::hitFeature()" << std::endl;}

	std::string Response[4];
	Response[0] = "What were you expecting?";
	Response[1] = "That was dumb.";
	Response[2] = "Maybe somebody needs some anger management.";
	Response[3] = "Got any other ideas?";


	std::string FName = getName(); // TODO: should use the noun that the user typed. Just pass the Choice in 

	if ( weight == 0 ) {
		std::cout << "Your hand passes through the incorporeal "<< FName <<" without affecting it." << std::endl;
		std::cout << "You feel a chill." << std::endl;
		return;
	}

	// TODO: support TEXT_HITTING in the future.. if we care
	std::cout << "You hit the " << FName << ". ";
	if ( weight >= 10 ) {
		std::cout << "Your hand hurts. ";
	}
	int i = rand() % 4;
	std::cout << Response[i] << std::endl;
	return;
}

/* *********************************************************
 *
 *
 * ********************************************************* */
std::string Feature::getWalkingInRoomText()
{
	std::string tmpS;

	if(!seen && !dropped)
	{
		seen = true;
		if (DEBUG_FEATURES) { std::cout << getName() << " dropped: " << dropped << " neverSeenText: " << getStringByKey("solvedText") << std::endl;}

		tmpS = getStringByKey("neverSeenText");
		//return neverSeenText;
	}
	else
	{
		//return getStringByKey("seenText");

		// This is a bad idea, shouldn't have text here that changes when solved
		if (DEBUG_FEATURES) {std::cout << getName() << " dropped: " << dropped << " text: " << getStringByKey("solvedText") << std::endl;}

		if(solved && getStringByKey("solvedText").length() > 0) 
		{
			//return solvedText;	
			tmpS = getStringByKey("solvedText");
		}
		else if (dropped && getStringByKey("onfloorText").length() > 0)
		{
			tmpS = getStringByKey("onfloorText");
		}
		else
		{
			tmpS = getStringByKey("seenText");
		}
	}
	LongString LString(tmpS);
	return LString.getWrappedText();
}

std::string Feature::getExamineText()
{
	std::string tmpS;
	tmpS = getStringByKey("solvedText");

	if(solved && tmpS.length() > 0 ) 
	{
		tmpS = getStringByKey("solvedText");
	}
	else {
		tmpS = getStringByKey("examineText");
	}
	LongString LString(tmpS);
	return LString.getWrappedText();
}

std::string Feature::getDependsOn()
{
	if (DEBUG_EXAMINE || DEBUG_BRENT ) std::cout << "In getDependsOn for " << getStringByKey("name") << " returning " << getStringByKey("dependsOn") << std::endl;
	return getStringByKey("dependsOn");
	//return dependsOn;
}

std::string Feature::getDependsOnInventory()
{
	if (DEBUG_EXAMINE) std::cout << "In getDependsOnInventory for " << getStringByKey("name") << " returning " << getStringByKey("dependsOnInventory") << std::endl;
	return getStringByKey("dependsOnInventory");
}

std::string Feature::getUses()
{
	if (DEBUG_EXAMINE) std::cout << "In getUses for " << getStringByKey("name") << " returning " << getStringByKey("uses") << std::endl;
	return getStringByKey("uses");
	//return uses;
}

std::string Feature::getTriggers()
{
	return getStringByKey("triggers");
	//return triggers;
}

std::string Feature::getSolvingText()
{
	std::string tmpS;
	// This logic (if solved) should probably be outside of this function... TODO
	if ( ! solved ) {
		tmpS = getStringByKey("solvingText");
	}
	else {
		tmpS = getStringByKey("solvedText");
	}
	LongString LString(tmpS);
	return LString.getWrappedText();
}

std::string Feature::getTakingText()
{
	return getStringByKey("takingText");
	//return takingText;
}

std::string Feature::getDroppingText()
{
	return getStringByKey("droppingText");
	//return droppingText;
}

std::string Feature::getUsingText()
{
	std::string tmpS;
	
	tmpS = getStringByKey("usingText");

	LongString LString(tmpS);
	return LString.getWrappedText();

}

bool Feature::isOpen()
{
	//return getStringByKey("open");
	//TODO
	return true;
}

bool Feature::isSolved()
{
	return solved;
}

bool Feature::isDropped()
{
	return dropped;
}

bool Feature::isSeen()
{
	return seen;
}

void Feature::setSolved(bool value)
{
	solved = value;
}

void Feature::setDropped(bool value)
{
	dropped = value;
}

void Feature::setKeyName(std::string keyName)
{
	key=strToLowercase(keyName);
}


std::string Feature::getKeyName()
{
	return key;
}

std::string Feature::getHintText(Feature * Subject)
{
	std::string tmpS;
	
	tmpS = getStringByKey("hintText");
	if (tmpS.length() == 0) {
		return "Sorry, no hints for " + getName();
	}

	LongString LString(tmpS);
	return LString.getWrappedText();
}

// returns lowercase string
// http://www.cplusplus.com/reference/locale/tolower/
std::string Feature::strToLowercase(std::string mixedStr)
{
	std::locale loc;

  for (std::string::size_type i=0; i<mixedStr.length(); ++i)
    mixedStr[i] = std::tolower(mixedStr[i],loc);
	return mixedStr;
}


void Feature::printFeaturesActions()
{
	if (!DEBUG_HOUSE ) return;
	std::cout << "--- feature's Actions: ---" << std::endl;
	for (auto it=actions.cbegin(); it != actions.cend(); it++) {
		std::cout << "\t\t" << it->first << "\t" << it->second << std::endl;
	}
}
