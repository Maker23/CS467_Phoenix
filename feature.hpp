#ifndef _FEATURE_H
#define _FEATURE_H

#include <iostream> // for debugging
#include <list>
#include <string>
#include <vector>
#include "room.hpp"


enum FeatureType { container, item, door_lock, block_room, used_with_another_item, decoration, puzzle, dropped_item };


class Feature
{
	private:
		std::string name;

		FeatureType type;
		std::string neverSeenText;
		std::string seenText;
		std::string solvedText;
		std::string examineText;
		std::string takenText;
		std::string droppedText;
		std::string usedText;
		bool solved;
		bool seen;
		int weight;
		std::string triggers;
		std::string dependsOn;

	public:
		std::vector<std::string> actions;
		Feature(std::string);
		~Feature();

		std::string getName();
		std::string getWalkingInRoomText(); 				// Prints the feature as you walk in the room.
		std::string getExamineText();
		std::string getTakenText();
		std::string getDroppedText();
		std::string getDependsOn();
		std::string getTriggers();
		void setSolved(bool);
		void hurlFeature(GameState *GS, Feature *Subject);
		bool isSolved();
		bool isSeen();
		std::vector<Feature*> Examine(GameState *GS);


};



#endif // _FEATURE_H
