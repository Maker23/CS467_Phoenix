#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "engine.hpp"
#include "room.hpp"

class Parser
{
	 private:
	 std::string strToLowercase(std::string);
	 GameState * GS;

    public:
    Parser(); // constructor
    Parser(GameState *GameStatePtr); // constructor
    ~Parser();	// destructor
    Choice * TestLine(Room *);
    Choice * ParseLine(Room *, std::string);
    Choice * ParseLine(Room *);
    validVerbs getVerb(std::string);
    std::string getNoun(std::string, Room *);
    std::string getRoom(std::string, Room *);
    std::string getFeature(std::string);
    std::string getSubject(std::string);
};

#endif
