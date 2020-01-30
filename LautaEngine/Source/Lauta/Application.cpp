#include "Application.h"
#include "Globals.h"
#include "ECS/EventManager.h"
#include "AssetManager.h"

//Global SDL structs 
SDL_Event Globals::Global_Event;
SDL_Window* Globals::Global_Window = nullptr;
SDL_Renderer* Globals::Global_Renderer = nullptr;
SDL_Rect Globals::Camera;

//All managers needed for the engine
Manager manager;	
AssetManager* AssetManager::Assets = new AssetManager(&manager);
EventManager *myEventManager = EventManager::Instance();


//Initialize everything regarding the app, that needs to be done once. 
void Application::InitApplication()
{
	

}



void Application::HandleEvents()
{
	//Wait for events and give them to the Global_Event
	SDL_PollEvent(&Globals::Global_Event);
	//check what event is happening
	switch (Globals::Global_Event.type) {
	case SDL_QUIT:
		m_IsRunning = false;
		break;
	case SDL_WINDOWEVENT:
		switch (Globals::Global_Event.window.event)
		{
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;
		}
	case SDL_KEYDOWN:
		switch (Globals::Global_Event.key.keysym.sym)
		{
		case SDLK_RETURN:
			if (!windowFull)
			{
				SDL_SetWindowFullscreen(Globals::Global_Window, SDL_TRUE);
				windowFull = true;
			}
			else {
				SDL_SetWindowFullscreen(Globals::Global_Window, SDL_FALSE);
				windowFull = false;
			}
			break;
		case SDLK_ESCAPE:
			Globals::Global_Event.type = SDL_QUIT;
			break;
		default:
			break;
		}
	}
}


void Application::Update()
{
	manager.refresh();
	manager.update();
	///////////////////////////////////////////////



	///////////////////////////////////////////////

}

//render everything under manager
//NOTE: render order matters
void Application::Render() 
{
	if (!mMinimized)
	SDL_RenderClear(Globals::Global_Renderer);
	////////////////////////////////////////////////



	////////////////////////////////////////////////
	SDL_RenderPresent(Globals::Global_Renderer);

	
}

void Application::Cleanup() 
{
	SDL_DestroyWindow(Globals::Global_Window);
	SDL_DestroyRenderer(Globals::Global_Renderer);
	SDL_Quit();
	printf("Cleanup finished\n");
}

bool Application::initEngine()
{
	//initialize everything regarding SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		//printf("Systems have initialized!\n");
		Globals::Global_Window = SDL_CreateWindow("Lauta Engine",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (!Globals::Global_Window)
			printf("Window could not be created\n");
		Globals::Global_Renderer = SDL_CreateRenderer(Globals::Global_Window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!Globals::Global_Renderer)
			printf("Renderer could not be created\n");
		m_IsRunning = true;
	}
	else
	{
		printf("SDL_init_everything failed...\n");
		return m_IsRunning;
	}
	if (TTF_Init() == -1)
		printf("ERROR! - Failed to init SDL_TTF");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	return m_IsRunning;
}
