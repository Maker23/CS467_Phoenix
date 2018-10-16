#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "engine.hpp"

class Parser
{
    public:
    Parser(); // constructor
    ~Parser();	// destructor
    Choice * ParseLine();
    validVerbs getVerb(std::string);
};

#endif
