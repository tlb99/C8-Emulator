#include "gfx.hpp"

bool Graphics::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window, renderer, and texture
		window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		texture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
	}

	return success;
}

void Graphics::draw(unsigned char gfx[])
{
	SDL_UpdateTexture(texture, NULL, gfx, 64 * sizeof(Uint32));


	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Graphics::close()
{
	//Destroy texture
	SDL_DestroyTexture(texture);
	window = NULL;

	//Destroy renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}