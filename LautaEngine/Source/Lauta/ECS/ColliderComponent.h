#pragma once
#include <string>
#include "Components.h"
#include "../LautaMath.h"
#include "SDL.h"

//default just add a tag for what your component is called
//for tiles, give also position coordinates. 
class ColliderComponent : public Component
{
public:

	SDL_Rect colliderArea;
	std::string tag;
	Vector2Di position;
	Vector2Di size;
	
	bool state = true;

	TransformComponent* transform;

	//make entity into a clickable one/////////////
	Uint32 click(SDL_MouseButtonEvent e)
	{
		bool insideArea = true;
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		if (mouseX < colliderArea.x) { insideArea = false; }
		else if (mouseY < colliderArea.y) { insideArea = false; }
		else if (mouseX > colliderArea.x + colliderArea.w) { insideArea = false; }
		else if (mouseY > colliderArea.y + colliderArea.h) { insideArea = false; }
		if(insideArea)
		{
			return e.type;
		}

		return NULL;
	}

	//tag what will the entity collide with
	ColliderComponent(std::string t)
	{
		tag = t;
	}

	std::string getTag()
	{
		return tag;
	}


	ColliderComponent(std::string t, Vector2Di pos, Vector2Di s)
	{
		tag = t;
		position = pos;
		size = s;
	}

	void setColliderState(bool newState)
	{
		state = newState;
	}

	void init() override
	{
		//Position component is needed, so we will add that if the entity does not have it yet
		if (!entity->hasComponent<TransformComponent>())
		{
			printf("add a Transform component to the collider entity!");
			//entity->addComponent<TransformComponent>(position.x, position.y, size.x, size.y);
		}
		
		transform = &entity->GetComponent<TransformComponent>();
	}

	void update() override
	{
		if (entity->GetComponent<SpriteComponent>().isSpriteStatic())
		{
			colliderArea.x = transform->position.x - Globals::Camera.x;
			colliderArea.y = transform->position.y - Globals::Camera.y;
		}
		else {
			colliderArea.x = transform->position.x;
			colliderArea.y = transform->position.y;
		}

		

		colliderArea.w = transform->width * transform->scale * state;
		colliderArea.h = transform->height * transform->scale * state;
	}


};