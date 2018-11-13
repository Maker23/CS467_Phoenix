#ifndef _FEATURE_H
#define _FEATURE_H

#include <iostream> // for debugging
#include <list>
#include <string>
#include <vector>
#include "room.hpp"
#include "engine.hpp"

enum FeatureType { container, item, door_lock, block_room, used_with_another_item, decoration, puzzle, dropped_item };

class Feature
{
	private:
		std::string key;
		std::string name;

		FeatureType type;
		std::map<std::string, std::string> featureStrings;
		// expected keys from the feature files:
		//		neverSeenText;
		//		observeText;
		//		seenText;
		//		solvedText;
		//		examineText;
		//		solvingText;
		//		takingText;
		//		droppingText;
		//		usingText;
		//		textToSolve;
		//		triggers;
		//		dependsOn;
		//    unlocks;
		//		open;
		//		uses; // in the real world this would be a list. meh.
		bool solved;
		bool seen;
		int weight;
		bool dropped;
		std::string strToLowercase(std::string);

	public:
		std::map<std::string, validVerbs> actions;
		std::map<std::string, std::string> aliases;
		Feature(std::string);
		~Feature();

		std::string getStringByKey(std::string);  // use this to get the string read in from the feature files.
		std::string getName();
		std::string getWalkingInRoomText();	// Prints the feature as you walk in the room.
		std::string getExamineText();
		std::string getSolvingText();
		std::string getTakingText();					// calls getStringByKey
		std::string getDroppingText();				// calls getStringByKey
		std::string getUsingText();					// calls getStringByKey
		std::string getDependsOn();					// calls getStringByKey
		std::string getTriggers();						// calls getStringByKey
		std::string getUses();							// calls getStringByKey
		void setSolved(bool);
		void setKeyName(std::string);
		std::string getKeyName();						// calls getStringByKey
		void useFeature(GameState *GS, Feature *Subject);
		void takeFeature(GameState *GS, Room * Rm, Feature *Subject);
		void dropFeature(GameState *GS, Room * Rm, Feature *Subject, bool Silent);
		void dropFeature(GameState *GS, Room * Rm, Feature *Subject);
		void hurlFeature(GameState *GS, Room * Rm, Feature *Subject);
		void hitFeature(Feature *Subject);
		void examineFeature();
		void Examine();
		bool isSolved();
		bool isOpen();
		bool isSeen();
		void printFeaturesActions();
		bool solveFeature(GameState *GS, std::string solveString);

};



#endif // _FEATURE_H
