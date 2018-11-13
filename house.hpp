#ifndef HOUSE_HPP
#define HOUSE_HPP

#include <string>
#include <stack>
#include <vector>
#include <map>
#include "room.hpp"
#include "feature.hpp"
#include "engine.hpp"

class House
{
	private:
		std::map<std::string,Room *> houseMap;   // this is a map of the house... room is key, holds the room pointer
		std::map<std::string,Feature *> houseFeatures; // this is a map of all features... feature name is key, holds the feature pointer
		std::map<std::string,std::string> houseFeatureAliases; // Feature aliases. Alias is the key, canonical name is the value.
		std::string strToLowercase(std::string);
		std::string firstRoomToLoad;

	public:
		House(); // constructor
		~House();	// destructor
		Room *buildHouse(std::string);		// Takes starting Room, Builds House, then returns the ptr of the starting room
		bool hasRoom(std::string);   			// Returns true if room ("Foyer" for instance) is found in the houseMap
		bool hasFeature(std::string);   			// Returns true if feature is in the house 
		std::string findFeatureByName(std::string);  	// Returns the 'official' name of a feature, or NOTFOUND
		Room *getRoomPtr(std::string);      // Returns the ptr of the Room object for the requested room.
		Feature *getFeaturePtr(std::string); // returns the ptr of the Feature object for the requsted feature.
		Room *getRoomPtrByConnectionAlias(std::string); 
		void printRooms(GameState *);						// For debugging really
		void printRooms();						// debugging
		void printFeatures(GameState *);						// For debugging really
		void printRoomFeatures(Room *);		// display the features in the room.
		void debugHouse();
		std::vector<std::string> getSolvedFeatures();
		std::vector<std::string> getDroppedFeatures();
		std::string getRoomFeaturesSaveString();
		std::string getRoomLockedDoorsSaveString();
		void reloadHouse();
};

#endif
