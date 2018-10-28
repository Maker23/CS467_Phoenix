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
	seenText = "";
	solvedText = "";
	examineText = "";
	takenText = "";
	droppedText = "";
	usedText = "";

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

			if(lineStr.find("TEXT_SOLVED: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(13, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_SOLVED " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					solvedText = tempStr;
			}


			if(lineStr.find("TEXT_TAKEN: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(12, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_TAKEN " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					takenText = tempStr;
			}

			if(lineStr.find("TEXT_DROPPED: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_TAKEN " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					droppedText = tempStr;
			}

			if(lineStr.find("TEXT_USED: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(11, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found TEXT_USED " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					usedText = tempStr;
			}

/*
			if(lineStr.find("DESCRIPTION1: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DESCRIPTION1 " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					description1 = tempStr;
			}

			if(lineStr.find("DESCRIPTION2: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DESCRIPTION2 " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					description2 = tempStr;
			}

			if(lineStr.find("DESCRIPTION3: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DESCRIPTION3 " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					description3 = tempStr;
			}

			if(lineStr.find("DESCRIPTION4: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(14, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DESCRIPTION4 " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					description4 = tempStr;
			}
*/
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
								actions[st] = kVerb;
								if (DEBUG_FEATURES) { std::cout << "      token = '" << st << "'" << std::endl;}
								cptr = strtok(NULL, ", ");
							}
						}
					}
					/*

				   while(getline(mystream,tempStr,','))
				   {  // https://stackoverflow.com/questions/40611689/c-error-in-tokenizer-variable-stdstringstream-mystream-has-initializer-b/43017562
				   		// remove leading and trailing spaces
				    		while(tempStr[0] == ' ')
				    		{
				    			tempStr = tempStr.substr(1, tempStr.length() - 1);
				    		}
				    		while(tempStr[tempStr.length() - 1] == ' ')
				    		{
				    			tempStr = tempStr.substr(0, tempStr.length() - 2);
				    		}
				       //actions.push_back(tempStr);
				   }
					 */
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

void Feature::Examine(GameState * GS)
{
	std::vector<Feature *> Contents;
	std::vector<Feature*>::iterator iter;


	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::Examine()" << std::endl;}
	if (DEBUG_EXAMINE) 
	{ 
		// sheesh dudes
	}
	return;
}

/* *********************************************************
 *
 *
 * ********************************************************* */
bool Feature::canUseFeature(GameState *GS)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::canUseFeature()" << std::endl;}

	Feature * checkFeature=NULL;

	// 2. If depends_on, check the state of that object - must be solved, or we can't "use" the thing
	// 3. If no depends on - use the thing
	// Triggers = not currently used
	//
  if ( dependsOn.compare("") != 0 ) 
	{
		// If depends_on, check the state of that object - 
		// must be solved, or we can't "use" the thing
	
		if (DEBUG_FEATURES) { std::cout << "      Checking dependency "<< dependsOn << std::endl;}
		checkFeature = GS->housePtr->getFeaturePtr(dependsOn);
		if (checkFeature != NULL){
			if (! checkFeature->isSolved()) {
				return false;
			}
		}
		//Missing a test case here - if checkFeature is NULL that's an error...
	}
	// Depends on nothing, or dependency is solved
	return true;
}

void Feature::useFeature(GameState *GS, Feature * Subject)
{
	std::string nounUses;
	Feature * checkFeature=NULL;

	if ( ! canUseFeature(GS) ) {return;}

	if (DEBUG_FEATURES) { std::cout << "      Setting isSolved to true " << std::endl;}
	setSolved(true);
	
	if ( solvedText.compare("") != 0 )
	{
		std::cout << solvedText << std::endl;
	}
	if ( usedText.compare("") != 0 )
	{
		std::cout << usedText << std::endl;
	}
	else
	{
		nounUses = getUses();
		checkFeature = GS->housePtr->getFeaturePtr(nounUses);
		if ( nounUses.compare("") != 0  &&  checkFeature != NULL )
		{
			if ( checkFeature->usedText.compare("") != 0 ) {
				std::cout << checkFeature->usedText << std::endl;
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
	if ( ! canUseFeature(GS) ) {return;}
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
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::dropFeature()" << std::endl;}
	if ( ! canUseFeature(GS) ) {return;}
	std::string FName = getName();
	std::string CName = "";

	if (DEBUG_FEATURES) { GS->housePtr->printFeatures(GS); }
	std::cout << "You drop the " << FName << std::endl;
	if (DEBUG_FEATURES) { std::cout << "      here 1" << CName << std::endl;}
	for (std::vector<Feature*>::iterator iter = GS->Holding.begin(); iter != GS->Holding.end(); iter ++ ) {
		if (DEBUG_FEATURES) { std::cout << "      here 2" << CName << std::endl;}
		CName = (*iter)->getName();
		if (DEBUG_FEATURES) { std::cout << "      looking for item" << CName << std::endl;}
		if ( CName.compare(FName) == 0 ) {
			// can this possibly work :/
			if (DEBUG_FEATURES) { std::cout << "      erasing item" << CName << std::endl;}
			GS->Holding.erase(iter);
			// This messes with iter++ and causes a SegFault.  TODO: investigate how this should really be done
			break;
		}
	}
	if (DEBUG_FEATURES) { GS->housePtr->printFeatures(GS); }
	Rm->addFeature(getName());
	if (DEBUG_FEATURES) { std::cout << "------------------------- end Feature::dropFeature()" << std::endl;}
}

void Feature::hurlFeature(GameState *GS, Feature * Subject)
{
	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::hurlFeature()" << std::endl;}
	if ( ! canUseFeature(GS) ) {return;}
	std::string hurlWhere;
	if ( weight >= 10 ) 
	{
		std::cout << "The " << getName() << " is too heavy to pick up." << std::endl;
	}
	else 
	{
		std::cout << "You throw the " << getName();
		if ( Subject ) {
			// Is there a subject?
			std::cout << " at the " << Subject->getName() << std::endl; 
		}
		else {
			std::cout << " across the room. It lands on the floor." << std::endl;
		}
	}
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
		if(solved) 
		{
			return solvedText;	
		}
		else
		{
			return seenText;
		}
	}
}

std::string Feature::getExamineText()
{
	return examineText;
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

std::string Feature::getTakenText()
{
	return takenText;
}

std::string Feature::getDroppedText()
{
	return droppedText;
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
