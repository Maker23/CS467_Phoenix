#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "engine.hpp"

class Parser
{
	 private:
	 std::string strToLowercase(std::string);
	 GameState * GS;

    public:
    Parser(); // constructor
    Parser(GameState *GameStatePtr); // constructor
    ~Parser();	// destructor
    Choice * TestLine();
    Choice * ParseLine(std::string);
    Choice * ParseLine();
    validVerbs getVerb(std::string);
    std::string getNoun(std::string);
    std::string getRoom(std::string);
    std::string getFeature(std::string);
    std::string getSubject(std::string);
};

#endif
