#ifndef _FEATURE_H
#define _FEATURE_H

#include <iostream> // for debugging
#include <list>
#include <string>
#include <vector>


enum FeatureType { container, item, door_lock, block_room, required_for_another_item, decoration };


class Feature
{
	private:
		std::string name;

		FeatureType type;
		bool solved;
		std::string unSolvedDescription;
		std::string additionalDescription;
		std::string solvedDescription;
		int weight;
		std::string triggers;
		std::string dependsOn;


	public:
				std::vector<std::string> actions;
		Feature(std::string);
		~Feature();

		std::string getName();
};



#endif // _FEATURE_H