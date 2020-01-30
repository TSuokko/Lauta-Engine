#pragma once
#include "Components.h"


struct Animation
{
	int index;
	int frames;
	int speed;

	Animation() {}
	Animation(int i, int f, int s)
	{
		index = i;
		frames = f;
		speed = s;
	}
};



//int frames, int speed
//larger the speed, the slower the animation
//use for animation files with all frames in a row
class AnimationComponent : public Component
{
public:
	
	std::map<const char*, Animation> animations;


	AnimationComponent(int frames, int speed, bool loop)
	{
		playing = loop;
 
		Animation defaultAnimation = Animation(animationIndex, frames, speed);
		animations.emplace("default", defaultAnimation);

		playAnimation("default");
	}

	void init() override
	{
		trn = &entity->GetComponent<TransformComponent>();
		spr = &entity->GetComponent<SpriteComponent>();

		SDL_QueryTexture(spr->getTexture(), NULL, NULL, &lastFramePos, NULL);
	}

	void update() override
	{
		if (playing) 
		{
			int framePos = 
				spr->getSpriteSourceRect().w * static_cast<int>((SDL_GetTicks() / m_speed) % m_frames);
			spr->changeTextureSourcePosition(Vector2Di(framePos, animationIndex * trn->height));

			if (playOnce)
			{
				//check the position of the last frame 
				//by subtracting the sprites width from the whole texture
				if (framePos == lastFramePos - spr->getSpriteSourceRect().w)
				{
					stopAnimation();
				}
			}
		}
	}


	void addAnimation(const char* animationName, int frames, int speed)
	{
		Animation newAnimation = Animation(++animationIndex, frames, speed);
		animations.emplace(animationName, newAnimation);
	}

	void playAnimation(const char* animationName)
	{
		m_speed = animations[animationName].speed;
		m_frames = animations[animationName].frames;
		animationIndex = animations[animationName].index;
		playing = true;
	}

	void playAnimationOnce()
	{
		playOnce = true;
	}

	void stopAnimation()
	{
		playing = false;
	}




private:

	SpriteComponent* spr;
	TransformComponent* trn;
	int lastFramePos;

	int animationIndex = 0;
	int m_frames = 0;
	int activeFrame = 0;
	int m_speed = 0;
	bool playing = false;
	bool playOnce = false;

};