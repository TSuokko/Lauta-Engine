#pragma once

#include "Components.h"
#include "../Globals.h"

//Vector2Di CameraMovementDimensions, Vector2Di startingPoint, Vector2Di UIwindowDimensions
//if you have a UI blocking the view, the UI window width must be added, 
//if you want to have the whole map render on screen
//otherwise, it can be set to zero
class CameraComponent : public Component
{
public:

	CameraComponent() = default;

	//currently locks the camera in the center of the screen
	CameraComponent(Vector2Di CameraMovementDimensions, Vector2Di startingPoint, 
		Vector2Di UIwindowDimensions)
	{
		Globals::Camera.x = startingPoint.x;
		Globals::Camera.y = startingPoint.y;
		Globals::Camera.w = CameraMovementDimensions.x + UIwindowDimensions.x;
		Globals::Camera.h = CameraMovementDimensions.y + UIwindowDimensions.y;
	}

	Vector2D getCameraPos()
	{
		return Vector2D(Globals::Camera.x, Globals::Camera.y);
	}

	void update() override
	{

		if (Globals::Camera.x < 0)
			Globals::Camera.x = 0;
		if (Globals::Camera.y < 0)
			Globals::Camera.y = 0;
		if (Globals::Camera.x > Globals::Camera.w)
			Globals::Camera.x = Globals::Camera.w;
		if (Globals::Camera.y > Globals::Camera.h)
			Globals::Camera.y = Globals::Camera.h;
	}

	bool WithMouse = true;

};