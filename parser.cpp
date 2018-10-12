#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "parser.hpp"

using namespace std;

Parser::Parser()
{

}

Parser::~Parser()
{
	// need to delete the memory allocated
}

void Parser::ParseLine(){
    string mystr;

    cout << "What would you like to do? ";
    getline (cin, mystr);

    int j = 1;

    for (int i=0; i < mystr.size(); i++) {
        if(mystr[i] == ' ' && mystr[i+1] != ' ')
        {
            j++;
        }
    }

    for (int i=0; i < mystr.size(); i++) {
       // cout << "mystr[" << i << " ] = " << mystr[i] << endl;
    }

    string array[j+1];
    string testing2;
    int test = 1;
    cout << endl;
    cout << "string size = " << mystr.size() << endl;
    for (int i=0; i < mystr.size(); i++)
        {
            if(mystr[i] == ' ' && mystr[i+1] != ' ')
            {
                array[test] = testing2;
                testing2 = "";
                test++;
            }
             testing2 += mystr[i];
        }
    array[test] = testing2;

    for (int i = 1; i < j+1; i++)
    {
        cout << "array[" << i << "] " << array[i] << endl;
    }

    cout << "num args: " << j << endl;
}
