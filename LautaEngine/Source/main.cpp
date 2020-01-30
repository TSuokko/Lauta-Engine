#include "SDL.h"
#include  "Lauta/Application.h"

//for game creation, an application class needs to be created
Application *app = nullptr;

int main(int argc, char *argv[]) 
{

	//frames per second capping
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	app = new Application();
	app->initEngine();
	app->InitApplication();

	//Initialize the webcamera and two screens

	while (app->Running()) 
	{

		frameStart = SDL_GetTicks();
		//main loop of events that our whole program is based on. 
		app->HandleEvents();
		app->Update();
		app->Render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	
	
	app->Cleanup();

	
	
	return 0;
}