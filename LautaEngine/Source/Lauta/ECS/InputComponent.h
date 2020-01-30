#pragma once

#include "Components.h"
#include <sstream>
#include "../LautaMath.h"
#include "CameraComponent.h"


class InputComponent : public Component
{
public:

	TransformComponent *transform;
	CameraComponent *cam;

	bool leftMouseButtonDown = false;
	bool rightMouseButtonDown = false;
	bool rightMouseEnable = false;
	bool windowFull = false;
	bool windowMovement = false;

	int mouseX = 0;
	int mouseY = 0;
	
	
	//add position component to get coordinates
	void init() override
	{
		transform = &entity->GetComponent<TransformComponent>();
	}

	Vector2Di getMouseCoordinates()
	{
		return Vector2Di(mouseX, mouseY);
	}

	Vector2Di getMouseCoordsWithCamera()
	{
		return Vector2Di(mouseX + Globals::Camera.x, mouseY + Globals::Camera.y);
	}


	void update() override
	{
		SDL_GetMouseState(&mouseX, &mouseY);

		/*
		switch (Globals::Global_Event.type)
		{
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
			default:
				break;
			}
			break;
		}
		
		case SDL_MOUSEBUTTONDOWN:
			switch (Globals::Global_Event.button.button)
			{
				//left button
			case SDL_BUTTON_LEFT:
			{
				if (!rightMouseEnable)
				{
					//sets mouse button clicked to true, so we can drag the mouse				
					leftMouseButtonDown = true;				
				}
				break;
			}
			case SDL_BUTTON_RIGHT:
				//put a popup test message if right mouse is clicked
				switch (rightMouseEnable)
				{
				case true:
					rightMouseEnable = false;
					break;
				case false:
					rightMouseEnable = true;
					break;
				}
				break;
			default:
				//puts a popup if for example the wheel is clicked
				//SDL_ShowSimpleMessageBox(0, "Mouse", "Some other button was pressed!", Globals::Global_Window);
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (Globals::Global_Event.button.button)
			{
			case SDL_BUTTON_LEFT:
				leftMouseButtonDown = false;
				break;
			case SDL_BUTTON_RIGHT:
				rightMouseButtonDown = false;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			if (rightMouseEnable && windowMovement)
			{
				if (entity->hasComponent<CameraComponent>())
				{	
					Globals::Camera.x -= Globals::Global_Event.motion.xrel;
					Globals::Camera.y -= Globals::Global_Event.motion.yrel;	
				}
			}
			break;
		}*/
	}

	void changeWindowMovementState(bool state)
	{
		windowMovement = state;
	}



	Vector2D movePlayerInMouseDirection(Vector2D mousePos)
	{
		Vector2D direction =
			Vector2D(mousePos.x - (transform->position.x + (transform->width * transform->scale) / 2 - Globals::Camera.x),
				mousePos.y - (transform->position.y + (transform->height * transform->scale) / 2 - Globals::Camera.y));

		if ((-transform->width < direction.x && direction.x < transform->width)
			&& (-transform->height < direction.y && direction.y < transform->height))
			return direction.Zero();
		else 
			return direction.Normalize();
	}

};

