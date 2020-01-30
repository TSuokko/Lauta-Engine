#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "LautaMath.h"
#include "ECS/ECS.h"
#include "SDL_ttf.h"


class AssetManager
{
public:

	AssetManager(Manager* manager);
	~AssetManager();

	void AddTexture(std::string tID, const char* path);

	void AddTextureXML(std::string tID, const char* path);

	void AddFont(std::string fID, const char* path, int fontSize);

	SDL_Texture* GetTexture(std::string tID);

	TTF_Font* GetFont(std::string fID);
	
	SDL_Rect getSourceRect(std::string tID);

	static AssetManager* Assets;

private:

	Manager* m_manager;

	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;

	std::map<std::string, SDL_Rect> textureSrcRects;

};