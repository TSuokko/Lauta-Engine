#pragma once
#include "SDL.h"
#include <iostream>


class DebugDraw
{
public:

	DebugDraw(int windowWidth, int windowHeight){
	
		m_windowH = windowHeight;
		m_windowW = windowWidth;
	}

	~DebugDraw() {
		delete[] m_pixels;
		SDL_DestroyTexture(m_pixelTexture);
	}

	//initialize debug drawing tool
	void init(SDL_Renderer* renderer)
	{
		m_pixelTexture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, m_windowW, m_windowH);

		m_pixels = new Uint32[m_windowW * m_windowH];

		memset(m_pixels, 255, m_windowW * m_windowH * sizeof(Uint32));
	}

	void update()
	{
		SDL_UpdateTexture(m_pixelTexture, NULL, m_pixels, m_windowW * sizeof(Uint32));
	}
	void render(SDL_Renderer* renderer)
	{
		SDL_RenderCopy(renderer, m_pixelTexture, NULL, NULL);
	}

	//draw pixels for debugging purposes
	void drawPixel(int xpos, int ypos)
	{
		//check boundaries of window
		if (xpos > 0 && ypos > 0)
		{
			if (xpos <= m_windowW && ypos <= m_windowH)
				//draw pixels
				m_pixels[ypos * m_windowW + xpos] = 0;
		}
	}

private:

	int m_windowH, m_windowW;
	Uint32 * m_pixels;
	SDL_Texture * m_pixelTexture;

};
