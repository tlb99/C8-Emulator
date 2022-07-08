#pragma once

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>

class Graphics
{
private:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The renderer contained by the window
	SDL_Renderer* renderer = NULL;

	SDL_Texture* texture = NULL;

	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

public:
	//Starts up SDL and creates window
	bool init();

	//Iterate through array and draw pixels on window correspondingly
	void draw(unsigned char gfx[]);

	//Frees media and shuts down SDL
	void close();
};