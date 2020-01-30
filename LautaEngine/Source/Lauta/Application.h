#pragma once
#include "ECS/ECS.h"
#include "SDL.h"

#define GLOBAL_SCALE 1
#define SCREEN_WIDTH 640 * GLOBAL_SCALE
#define SCREEN_HEIGHT 480 * GLOBAL_SCALE

class Application {

public:

	Application() {};
	~Application() {};

	bool initEngine();	//initialize everything regarding SDL

	void InitApplication(); //initialize everything needed for the application

	bool Running() { return m_IsRunning; } //check if the application is running

	void HandleEvents(); //handles all input events from mouse, keyboard etc.

	void Update(); //Handles data updates

	void Render(); //handles all rendering that happens on screen

	void Cleanup(); //cleans up any resource loaded	

private:


	bool mMinimized = false;
	bool windowFull = false;
	bool m_IsRunning = false;	

};

