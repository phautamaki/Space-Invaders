#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H
 
#include <string>
#include <vector>
 
void debug( std::string message, int tabs = 0 );
void debug( int value, int tabs = 0 );
void error( std::string message );

std::string IntToString(int Value);

int StringToInt(std::string Text);

float DegreesToRadian(int Degree);

std::vector<std::string> Explode(std::string Text, char Separator);

#endif