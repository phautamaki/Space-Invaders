#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H
 
#include <string> 
 
void debug( std::string message, int tabs = 0 );

std::string IntToString(int Value);

int StringToInt(std::string Text);

float DegreesToRadian(int Degree);

#endif