/* vim:ts=2:
 *
 * File name: Container.cpp
 *
 * Overview:
 *   General functions that work on Features.
 *
 * 	 Feature::Examine  prints the contents of a Feature, recursively,
 * 	 and returns a Feature* vector.
 *
 * 	 Feature::FindByName searches for an object by name and returns
 * 	 a pointer to the object if found, NULL if not
 *
 * 	 GameState:: functions apply to the derived class which is the player's
 * 	 game state 
 *
 */

#include <iostream> // for debugging
#include <vector>
#include <string>

#include "feature.hpp"
#include "house.hpp"


/* ***********************************************************
 * Constructors
 * ********************************************************* */
Feature::Feature (std::string Na)
{
	Name=Na;
	Story="";
	Weight=1;
	UseFunc = NULL;
	OpenFunc = NULL;
	isContainer=false;
	Open=true;
}

Feature::Feature (std::string Na, std::string St)
{
	Name=Na;
	Story=St;
	Weight=1;
	UseFunc = NULL;
	OpenFunc = NULL;
	isContainer=false;
	Open=true;
}

/* ***********************************************************
 * Destructors
 * ********************************************************* */
Feature::~Feature()
{
	//TODO
	std::vector<Feature*>::iterator iterFeature;
	for (iterFeature=Contents.begin(); iterFeature != Contents.end(); iterFeature++)
	{
		delete (*iterFeature);
	}
}

/* ********************************************************* */
void Feature::Print () 
{
	std::cout << Name << ". ";
}

/* ********************************************************* */
std::vector<Feature*> Feature::Examine(bool reCursive, bool verbose, bool silent)
{
  // TODO: return an empty vector for a non-container hting
	// For a feature with is_container=true, return a vector to its Contents
	// Should we have a recursive flag? Yes... yes we should.

	std::vector<Feature*>::iterator iter;

	std::cout << Name << ". " << Story << std::endl;

  if (DEBUG_EXAMINE) 
	{ 
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
			
			/*
			<< "		Weight = " << Weight << std::endl
			<< "		UseFunc = " << UseFunc << std::endl
			<< "		OpenFunc = " << OpenFunc << std::endl
			*/
	}
	if ( isContainer && Open && reCursive ) {
  	if (DEBUG_EXAMINE) std::cout  << "DEBUG Feature::Examine -- reCursing now " << std::endl;
		for ( iter = Contents.begin(); iter != Contents.end(); iter ++ )
		{
			(*iter)->Examine(reCursive, verbose, silent);
		}
	}
	return Contents; // may be empty if the Feature is not a container. Do we need this though? No we do not. TODO: this returns a void I think
}


/* ***********************************************************
 * Search inside a container for a Feature with a particulare name
 * Return the pointer for that Feature
 * ********************************************************* */
Feature * Feature::FindByName(std::string SeekingFeatureName)
{
	std::vector<Feature*>::iterator iter;
	Feature * tmpVal;

	if (DEBUG_FIND) std::cout << "===== begin Feature::FindByName, looking in " << this->Name << std::endl;
	for (iter=Contents.begin(); iter != Contents.end(); iter++)
	{
			if ( ((*iter)->Name).compare(SeekingFeatureName) == 0 ) 
			{
				//return this; // The Feature is in this container
				return (*iter); // Return the pointer to feature found
			}
			if ( (*iter)->isContainer )
			{
				if (DEBUG_FIND) std::cout << "DEBUG: Recursing FindByName into " << (*iter)->Name << std::endl;
				tmpVal = (((Feature*)(*iter))->Feature::FindByName(SeekingFeatureName));
				if (tmpVal) return tmpVal;
			}
	}
	if (DEBUG_FIND) std::cout << "===== begin Feature::FindByName, looking in " << this->Name << std::endl;
	return ((Feature *)0);
}

