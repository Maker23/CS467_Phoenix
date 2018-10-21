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
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <stack>
#include <vector>
#include <typeindex>
#include <sstream>
#include <algorithm>
#include "house.hpp"
#include "room.hpp"
#include "feature.hpp"

using namespace std;

Feature::Feature(string fileToOpen)
{
	ifstream featurefile;
	string lineStr, tempStr;


	// set defaults
	name = "";
	solved = false;
	description1 = "";
	description2 = "";
	description3 = "";
	description4 = "";
	weight = 0;
	triggers = "";
	dependsOn = "";



	featurefile.open(fileToOpen);

	if (featurefile.is_open()) {
		while (std::getline(featurefile, lineStr))  {

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

			if(lineStr.find("DESCRIPTION1: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(15, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DESCRIPTION1 " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					description1 = tempStr;
			}

			if(lineStr.find("DESCRIPTION2: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(17, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DESCRIPTION2 " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					description2 = tempStr;
			}

			if(lineStr.find("DESCRIPTION3: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(16, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DESCRIPTION3 " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					description3 = tempStr;
			}

			if(lineStr.find("DESCRIPTION4: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(16, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found DESCRIPTION4 " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
					description4 = tempStr;
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

			if(lineStr.find("ACTIONS: ") != std::string::npos) 
			{
				tempStr = lineStr.substr(9, lineStr.length()-1);
				if (DEBUG_FEATURES) { std::cout << "Feature() - Found ACTIONS " << tempStr << std::endl;}
				// check if not empty and isn't set to "null"
				if(tempStr.length() > 0 && tempStr.compare("null") != 0)
				{
					std::string keywordStr = tempStr;
				   std::stringstream mystream (keywordStr);

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
				       actions.push_back(tempStr);
				   }
					//dependsOn = lineStr.substr(6, lineStr.length()-1);
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

std::vector<Feature*> Feature::Examine(GameState * GS)
{

	if (DEBUG_FEATURES) { std::cout << "----- begin Feature::Examine()" << std::endl;}
	std::vector<Feature *> Contents;
	// TODO: return an empty vector for a non-container hting
	// For a feature with is_container=true, return a vector to its Contents
	// Should we have a recursive flag? Yes... yes we should
	std::vector<Feature*>::iterator iter;
	std::cout << getName() << ". " << getDescription() << std::endl;
	if (DEBUG_EXAMINE) 
	{ 
	/*
		std::cout  << "DEBUG Feature::Examine " << std::endl;
		std::cout << "		Name = " << Name << std::endl
			<< "		Story = " << Story << std::endl
			<< "		isContainer = " << isContainer << std::endl
			<< "		Open = " << Open << std::endl;
		std::cout << "		Verbs = ";
			for (std::list<int>::iterator iter = Verbs.begin(); iter != Verbs.end(); iter++)
			{
				std::cout << *iter << ",";
			}
			std::cout << std::endl;
	*/
			/*
 			 * 	<< "		Weight = " << Weight << std::endl
 			 * 	<< "		UseFunc = " << UseFunc << std::endl
 			 * 	<< "		OpenFunc = " << OpenFunc << std::endl
 			 */
	}
	/*
	if ( isContainer && Open && reCursive ) {
  	if (DEBUG_EXAMINE) std::cout  << "DEBUG Feature::Examine -- reCursing now " << std::endl;
		for ( iter = Contents.begin(); iter != Contents.end(); iter ++ )
		{
			(*iter)->Examine(reCursive, verbose, silent);
		}
	}
	*/
	return Contents; // may be empty if the Feature is not a container. Do we need this though? No we do not. TODO: this returns a void I think
}

std::string Feature::getDescription()
{
	return ""; // TODO implement dis
}
