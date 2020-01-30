#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include <map>
#include "../AssetManager.h"
#include "../Application.h"




//Components: const char* path, int width, int height, int scale
//If using sprite sheet: const char* spriteSheetPath, int spriteWidth, int spriteHeight, 
//int xPositionInSheet, int yPositionInSheet, int scale
//if only path has been set, will default to texture's original size and scale

//std::string tID, Vector2Di spriteSize, Vector2Di positionInSpriteSheet, float scale
class SpriteComponent : public Component
{
private:

	SDL_Texture *m_texture;
	TransformComponent *m_transform;

	//animation variables
	int m_frames = 0;
	int m_speed = 100;
	bool animated = false;
	
	SDL_Rect m_sourceRect, m_destinationRect;
	Vector2Di m_textureSize;
	const char* m_currentTexturePath;

	Vector2Di m_TextureSourcePosition;

	float m_textureScale;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	int m_angle = 0;

	bool Movable = true;

	std::string id;
	
public:

	//draws the texture on screen which is assinged to the entity
	void draw() override
	{
		TextureManager::Draw(m_texture, m_sourceRect, m_destinationRect, spriteFlip, m_angle);
	}

	SpriteComponent() = default;

	
	SpriteComponent(std::string tID, Vector2Di spriteSize, Vector2Di positionInSpriteSheet, float scale)
	{
		//////////////
		id = tID;
		setTexture(tID);


		m_textureSize = spriteSize;
		m_TextureSourcePosition = positionInSpriteSheet;
		m_textureScale = scale;
	}

	//using xml given texture info
	SpriteComponent(std::string tID, SDL_Rect sourceRect, float scale)
	{
		//////////////
		id = tID;

		//////////////
		setTexture(tID);
		m_TextureSourcePosition = Vector2Di(sourceRect.x, sourceRect.y);
		m_textureSize = Vector2Di(sourceRect.w, sourceRect.h);
		m_textureScale = scale;
		
	}

	SpriteComponent(std::string tID, SDL_Rect sourceRect, float scale,
		SDL_RendererFlip flip, int rotation, bool isMovable)
	{
		//////////////
		id = tID;

		//////////////
		setTexture(tID);
		m_TextureSourcePosition = Vector2Di(sourceRect.x, sourceRect.y);
		m_textureSize = Vector2Di(sourceRect.w, sourceRect.h);
		m_textureScale = scale;

		spriteFlip = flip;
		m_angle = rotation;
		Movable = isMovable;

	}


	//for more freedom regarding texture 
	SpriteComponent(std::string tID, Vector2Di spriteSize, Vector2Di positionInSpriteSheet, 
		float scale, SDL_RendererFlip flip, int rotation, bool isMovable)
	{
		//////////////
		id = tID;

		//////////////
		setTexture(tID);
		m_textureSize = spriteSize;
		m_TextureSourcePosition = positionInSpriteSheet;
		m_textureScale = scale;

		spriteFlip = flip;
		m_angle = rotation;
		Movable = isMovable;
	}

	void changeRGBValuesOfWholeTexture(SDL_Color color)
	{
		SDL_SetTextureColorMod(m_texture, color.r, color.g, color.b);
	}

	void changeSpriteAngle(int angle)
	{
		m_angle = angle;
	}

	bool isSpriteStatic()
	{
		return Movable;
	}

	~SpriteComponent() 
	{
		SDL_DestroyTexture(m_texture);
	}

	void init() override
	{
		//initialize transformation so we can change dimensions if we want to
		m_transform = &entity->GetComponent<TransformComponent>();
		m_transform->initSize(m_textureSize.x, m_textureSize.y, m_textureScale);
		//use sizes that the user has given to us
		m_sourceRect.x = m_TextureSourcePosition.x;
		m_sourceRect.y = m_TextureSourcePosition.y;
		m_sourceRect.w = m_transform->width;
		m_sourceRect.h = m_transform->height;
		m_destinationRect.w = m_transform->width * m_transform->scale;
		m_destinationRect.h = m_transform->height * m_transform->scale;
	}


	SDL_Rect getSpriteSourceRect()
	{
		return m_sourceRect;
	}

	void changeTextureSourcePosition(Vector2Di newPositionInSpriteSheet)
	{

		m_sourceRect.x = newPositionInSpriteSheet.x;
		m_sourceRect.y = newPositionInSpriteSheet.y;
	}

	void changeSourceTexture(std::string textureName)
	{
		m_sourceRect = AssetManager::Assets->getSourceRect(textureName);
	}

	void update() override
	{
		//update every frame what size the texture is and where it is
		if (Movable) 
		{
			m_destinationRect.x = m_transform->position.x - Globals::Camera.x;
			m_destinationRect.y = m_transform->position.y - Globals::Camera.y;		
		}
		else
		{
			m_destinationRect.x = m_transform->position.x;
			m_destinationRect.y = m_transform->position.y;
		}
		
		m_destinationRect.w = m_transform->width * m_transform->scale;
		m_destinationRect.h = m_transform->height * m_transform->scale;
	}


	//if the user wants to change texture scale
	void ChangeTextureScale(float scale)
	{
		m_textureScale = scale;
	}

	void ChangeDestinationPosition(Vector2Di newPosition)
	{
		m_destinationRect.x = newPosition.x;
		m_destinationRect.y = newPosition.y;
	}

	//loads in the texture assigned to the entity
	void setTexture(std::string tID)
	{
		//destroy the current texture
		SDL_DestroyTexture(m_texture);
		//and load in the new one
		m_texture = AssetManager::Assets->GetTexture(tID);
	}

	SDL_Texture* getTexture()
	{
		return m_texture;
	}

};