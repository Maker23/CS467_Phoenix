#ifndef _FEATURE_H
#define _FEATURE_H

#include <iostream> // for debugging
#include <list>
#include <string>

class Room; // Forward declaration
class House; // Forward declaration

#include "utilities.hpp"
#include "house.hpp"

class Feature {
public:
	std::string Name;
	std::string Story;
	bool isContainer;
	bool Open;
	int Weight;	// We may not use weight and capacity - TBD. 
	int Capacity;

	Feature (std::string Na, std::string St); // constructor
	Feature (std::string Na);	// constructor
	virtual ~Feature();				// destructor

  /* ********************************************************** */
	//std::list<validVerbs> Verbs; 		
	std::list<int> Verbs; 		
	std::vector<Feature*> Contents;   // Objects held by container are also Features
	void Print();

	// Don't know if we're going to use these
	bool (*UseFunc)(Room *, Feature *);	//Does this feature do something? Most do not
	bool (*OpenFunc)(Feature *); 				// Does this feature open? Most do not
	virtual std::vector<Feature*> Examine(bool reCursive=true, bool verbose=true, bool silent=false); 
	virtual int getWeight() {return Weight;};
	virtual Feature * FindByName(std::string);
};

#endif // _FEATURE_H
