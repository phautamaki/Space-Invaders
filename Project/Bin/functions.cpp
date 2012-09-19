//==============================================================================
#include "functions.h"

#include <cstdlib>
#include <iostream>
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
