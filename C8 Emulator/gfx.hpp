#pragma once

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>

class Graphics
{
private:

	//Loads individual image
	SDL_Surface* loadSurface(std::string path);

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The surface contained by the window
	SDL_Surface* gScreenSurface = NULL;

	//The images that correspond to a keypress
	SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

	//Current displayed image
	SDL_Surface* gCurrentSurface = NULL;

	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

public:


	//Key press surfaces constants
	enum KeyPressSurfaces
	{
		KEY_PRESS_SURFACE_DEFAULT,
		KEY_PRESS_SURFACE_UP,
		KEY_PRESS_SURFACE_DOWN,
		KEY_PRESS_SURFACE_LEFT,
		KEY_PRESS_SURFACE_RIGHT,
		KEY_PRESS_SURFACE_TOTAL
	};

	//Starts up SDL and creates window
	bool init();

	//Loads media
	bool loadMedia();

	//Frees media and shuts down SDL
	void close();




};