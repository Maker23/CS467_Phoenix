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
		std::string neverSeenText;
		std::string observeText;
		std::string seenText;
		std::string solvedText;
		std::string examineText;
		std::string solvingText;
		std::string takingText;
		std::string droppingText;
		std::string usingText;
		std::string textToSolve;
		std::map<std::string, std::string> featureStrings;
		bool solved;
		bool seen;
		int weight;
		std::string triggers;
		std::string dependsOn;
		std::string uses; // in the real world this would be a list. meh.
		std::string strToLowercase(std::string);

	public:
		//std::vector<std::string> actions;
		std::map<std::string, validVerbs> actions;
		Feature(std::string);
		~Feature();

		std::string getName();
		std::string getWalkingInRoomText();	// Prints the feature as you walk in the room.
		std::string getExamineText();
		std::string getSolvingText();
		std::string getTakingText();
		std::string getDroppingText();
		std::string getUsingText();
		std::string getDependsOn();
		std::string getTriggers();
		std::string getUses();
		std::string getStringByKey(std::string);
		void setSolved(bool);
		void setKeyName(std::string);
		std::string getKeyName();
		void useFeature(GameState *GS, Feature *Subject);
		void takeFeature(GameState *GS, Room * Rm, Feature *Subject);
		void dropFeature(GameState *GS, Room * Rm, Feature *Subject, bool Silent);
		void dropFeature(GameState *GS, Room * Rm, Feature *Subject);
		void hurlFeature(GameState *GS, Room * Rm, Feature *Subject);
		void hitFeature(Feature *Subject);
		void examineFeature();
		void Examine();
		bool isSolved();
		bool isSeen();
		void printFeaturesActions();

};



#endif // _FEATURE_H
