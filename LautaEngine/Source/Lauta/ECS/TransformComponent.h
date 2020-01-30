#pragma once

#include "Components.h"
#include "../LautaMath.h"
#include "../Globals.h"
//Components: float x position, float y position
//Will default to position 0,0
class TransformComponent : public Component
{

public:

	Vector2D position;
	Vector2D velocity;
	Vector2Di size;

	int width;
	int height;
	float scale;

	//for movement purposes
	int speed = 1;

	//default position
	TransformComponent()
	{
		position.Zero();
	}
	
	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(Vector2D pos)
	{
		position.x = pos.x;
		position.y = pos.y;
	}

	TransformComponent(float x, float y, int w, int h)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
	}

	Vector2D getGlobalPosition()
	{
		return position;
	}

	Vector2D getWindowPosition()
	{
		Vector2D cam(Globals::Camera.x, Globals::Camera.y);
		return getGlobalPosition().subtract(cam);

	}

	void changePosition(Vector2D newPos)
	{
		position = newPos;
	}

	Vector2Di getSize()
	{
		return size;
	}

	void initSize(int textureWidth, int textureHeight, float scaleTexture)
	{
		width = textureWidth;
		height = textureHeight;
		scale = scaleTexture;

		size = Vector2Di(width * scale, height * scale);
	}

	void init() override
	{
		velocity.Zero();
	}

	//movement of the entity
	void update() override 
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;

		
	}
};

