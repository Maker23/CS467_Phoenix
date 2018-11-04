#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "engine.hpp"

class Parser
{
	 private:
	 std::string strToLowercase(std::string);

    public:
    Parser(); // constructor
    ~Parser();	// destructor
    Choice * TestLine(std::ifstream *);
    Choice * ParseLine(std::string);
    Choice * ParseLine();
    validVerbs getVerb(std::string);
    std::string getNoun(std::string);
};

#endif
