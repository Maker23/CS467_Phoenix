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
		bool solved;
		bool seen;
		int weight;
		std::string triggers;
		std::string dependsOn;
		std::string description1;
		std::string description2;
		std::string description3;
		std::string description4;

	public:
		std::vector<std::string> actions;
		Feature(std::string);
		~Feature();

		std::string getName();
		std::string getDescription();
		void printWalkingInRoomDescription(); 				// Prints the feature as you walk in the room.
		void printExamineFeature();
		std::string getDependsOn();
		std::string getTriggers();
		std::vector<Feature*> Examine(GameState *GS);
		void hurlFeature(GameState *GS, Feature *Subject);
		bool isSolved();
		bool isSeen();
		void setSolved(bool);


};



#endif // _FEATURE_H
