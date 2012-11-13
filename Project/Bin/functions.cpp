//==============================================================================
#include "functions.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <SDL.h>

//==============================================================================
void debug( std::string message, int tabs ) {
	int counter = 0;
	while( counter < tabs )
	{
		std::cout << '\t';
		counter++;
	}
	std::cout << SDL_GetTicks() << ": " << message << std::endl;
}

//==============================================================================
std::string IntToString(int Value) {
	std::stringstream ss;
	ss << Value;
	return ss.str();
}

//------------------------------------------------------------------------------
int StringToInt(std::string Text) {
	return atoi( Text.c_str() );

	int number;
	std::istringstream ss( Text );
	ss >> number;
	if (! ss.good())
	{
		return -999;
	}
	return number;
}

//------------------------------------------------------------------------------
float DegreesToRadian(int Degree) {
	return static_cast<float>(Degree) * (3.14159265/180.0);
}

//==============================================================================
