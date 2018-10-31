/*
 Notes:
 	Everything that can be used in a room is a feature. Features have a differfent quality, check the type enum for the options.
 	It is loaded into a MAP of Features objects located in house (just liek the room objects)
 	To get a feature's object ptr, call it with house->getFeaturePtr(string key);  key is the name of the file, and is used in the rooms vector of what features it has.

TODO: quite a bit, including:

	Fix anything broke with prior way of doing it:  grep FIX_FEATURE_REFACTOR *.cpp for the list
		(should be Room::Examine and GameState::getAvailableCapacity -- if we are going to bother, and i don't think we should bother with carrying capacity)

	Determine for each FeatureType, how the DESCRIPTION[1-4] lines will be used and then change the feature files so they are consistent.

	For a player's inventory: have a vector of strings... and use the feature name of the items they picked up, OR copy the pointer to that feature.

   implement each of the actions such as door locks, take item, drop item, etc.
 	actions for items should be how to use it. Items by default should be able to be picked up and dropped.	



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
#include "engine.hpp"
#include "house.hpp"
#include "room.hpp"
#include "feature.hpp"
#include "parser.hpp"

using namespace std;

Feature::Feature(string fileToOpen)
{
	ifstream featurefile;
	string lineStr, tempStr;


	// set defaults
	name = "";
	solved = false;
	weight = 0;
	triggers = "";
	dependsOn = "";
	uses = "";
	seen = 0;
	neverSeenText = "";
	observeText = "";
	seenText = "";
	solvedText = "";
	examineText = "";
	solvingText = "";
	takingText = "";
	droppingText = "";
	usingText = "";

	featurefile.open(fileToOpen);

	if (featurefile.is_open()) {
		while (std::getline(featurefile, lineStr))  {

			if (lineStr.substr(0,1).find("#") != std::string::npos )
			{
				// Skip lines that begin with # for comments
				continue;
			}
			if(lineStr.find("NAME: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(6, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found NAME " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					name = tempStr;
			}

			if(lineStr.find("TYPE: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(6, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TYPE " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if((tempStr.length() > 0) && (tempStr.compare("null") != 0))
				{
					if(tempStr.compare("container") == 0)
						type = container;
					else if(tempStr.compare("item") == 0)
						type = item;
					else if(tempStr.compare("door_lock") == 0)
						type = door_lock;
					else if(tempStr.compare("block_room") == 0)
						type = block_room;
					else if(tempStr.compare("used_with_another_item") == 0)
						type = used_with_another_item;
					else if(tempStr.compare("decoration") == 0)
						type = decoration;
					else if(tempStr.compare("puzzle") == 0)
						type = puzzle;
					else if(tempStr.compare("dropped_item") == 0)
						type = dropped_item;
					if (DEBUG_FEATURES) { std::cout << "Feature() - Type: " << type << std::endl;}
				}
			}

			if(lineStr.find("TEXT_NEVERSEEN: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(16, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_NEVERSEEN " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					neverSeenText = tempStr;
			}

			if(lineStr.find("TEXT_SEEN: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(11, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_SEEN " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					seenText = tempStr;
			}

			if(lineStr.find("TEXT_EXAMINE: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_EXAMINE " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					examineText = tempStr;
			}

			if(lineStr.find("TEXT_EXAMINE_SOLVED: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(21, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_EXAMINE_SOLVED " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					solvedText = tempStr;
			}

			if(lineStr.find("TEXT_SOLVING: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_SOLVING " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					solvingText = tempStr;
			}


			if(lineStr.find("TEXT_TAKING: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(13, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_TAKING " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					takingText = tempStr;
			}

			if(lineStr.find("TEXT_DROPPING: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(15, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_DROPPING " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					droppingText = tempStr;
			}

			if(lineStr.find("TEXT_USING: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(12, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_USING " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					usingText = tempStr;
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
					triggers = tempStr;
			}

			if(lineStr.find("DEPENDS_ON: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(12, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DEPENDS_ON " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					dependsOn = tempStr;
			}

			if(lineStr.find("USES: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(6, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found USES " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					uses = tempStr;
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

Feature::~Feature()
{

}

std::string Feature::getName()
{
	return name;
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
	std::cout << examineText << std::endl;
	return;
}


void Feature::useFeature(GameState *GS, Feature * Subject)
{
	std::string nounUses;
	Feature * checkFeature=NULL;

	if (DEBUG_FEATURES) { std::cout << "      Setting isSolved to true " << std::endl;}
	setSolved(true);
	
	if ( solvingText.compare("") != 0 )
	{
		std::cout << solvingText << std::endl;
	}
	if ( usingText.compare("") != 0 )
	{
		std::cout << usingText << std::endl;
	}
	else
	{
		nounUses = getUses();
		checkFeature = GS->housePtr->getFeaturePtr(nounUses);
		if ( nounUses.compare("") != 0  &&  checkFeature != NULL )
		{
			if ( checkFeature->usingText.compare("") != 0 ) {
				std::cout << checkFeature->usingText << std::endl;
			}
		}
		// get the used text from the dependency
	}

	return;

	//if ( getTriggers().compare("") == 0 ) {
	//}
	//else {
		// What does this do :)
		// Trigger the triggered object ;)
	//}
}

void Feature::takeFeature(GameState *GS, Room * Rm,Feature * Subject)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::takeFeature()" << std::endl;}
	if ( weight >= 10 ) 
	{
		std::cout << "The " << getName() << " is too heavy to pick up." << std::endl;
	}
	else 
	{
		std::cout << "You pick up the " << getName() << std::endl;
		GS->Holding.push_back(this);
		Rm->deleteFeature(getName());
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

	if (! Silent) std::cout << "You drop the " << FName << std::endl;
	for (std::vector<Feature*>::iterator iter = GS->Holding.begin(); iter != GS->Holding.end(); iter ++ ) {
		CName = (*iter)->getName();
		if ( CName.compare(FName) == 0 ) {
			GS->Holding.erase(iter);
			// This messes with iter++ and causes a SegFault if we don't break. 
			// TODO: investigate how this should really be done
			break;
		}
	}
	Rm->addFeature(getName());

	return;
}

void Feature::hurlFeature( GameState * GS, Room * Rm, Feature * Subject)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::hurlFeature()" << std::endl;}
	std::string hurlWhere;
	bool Silent = true;

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

void Feature::hitFeature(Feature * Subject)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::hitFeature()" << std::endl;}

	std::string FName = getName(); // TODO: should use the noun that the user typed. Just pass the Choice in 

	// TODO: support TEXT_HITTING in the future.. if we care
	std::cout << "You hit the " << FName << ". ";
	if ( weight >= 10 ) {
		std::cout << "Your hand hurts. ";
	}
	
	std::cout << "Nothing happens. What were you expecting?" << std::endl;
	return;
}

/* *********************************************************
 *
 *
 * ********************************************************* */
std::string Feature::getWalkingInRoomText()
{
	if(!seen)
	{
		seen = true;
		return neverSeenText;;
	}
	else
	{
		return seenText;
	}

		// taking this out, shouldn't have text here that changes when solved
		//if(solved) 
		//{
			//return solvedText;	
		//}
		//else
		//{
			//return seenText;
		//}
}

std::string Feature::getExamineText()
{
		if(solved) 
		{
			return solvedText;	
		}
		else {
			return examineText;
		}
}

std::string Feature::getDependsOn()
{
	if (DEBUG_EXAMINE) std::cout << "In getDependsOn for " << getName() << " returning " << dependsOn << std::endl;
	return dependsOn;
}

std::string Feature::getUses()
{
	if (DEBUG_EXAMINE) std::cout << "In getUses for " << getName() << " returning " << uses << std::endl;
	return uses;
}

std::string Feature::getTriggers()
{
	return triggers;
}

std::string Feature::getSolvingText()
{
	return solvingText;
}

std::string Feature::getTakingText()
{
	return takingText;
}

std::string Feature::getDroppingText()
{
	return droppingText;
}

std::string Feature::getUsingText()
{
	return usingText;
}

bool Feature::isSolved()
{
	return solved;
}

bool Feature::isSeen()
{
	return seen;
}

void Feature::setSolved(bool value)
{
	solved = value;
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