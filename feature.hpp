#ifndef _FEATURE_H
#define _FEATURE_H

#include <iostream> // for debugging
#include <list>
#include <string>
#include <vector>
#include "engine.hpp"


enum FeatureType { container, item, door_lock, block_room, used_with_another_item, decoration, puzzle, dropped_item };


class Feature
{
	private:
		std::string name;

		FeatureType type;
		bool solved;
		std::string description1;
		std::string description2;
		std::string description3;
		std::string description4;
		int weight;
		std::string triggers;
		std::string dependsOn;


	public:
		std::vector<std::string> actions;
		Feature(std::string);
		~Feature();

		std::string getName();
		std::string getDescription();
		std::vector<Feature*> Examine(GameState *GS);
};



#endif // _FEATURE_H
