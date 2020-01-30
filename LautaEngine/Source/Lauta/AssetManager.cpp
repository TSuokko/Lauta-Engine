#include "AssetManager.h"
#include "TinyXML/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

AssetManager::AssetManager(Manager * manager) : m_manager(manager)
{}

AssetManager::~AssetManager()
{
}

//add texture to the manager
//tileset ID, path to the tileset
void AssetManager::AddTexture(std::string tID, const char * path)
{
	textures.emplace(tID, TextureManager::LoadTexture(path));
}

void AssetManager::AddTextureXML(std::string tID, const char* XMLpath)
{
	XMLDocument doc;
	std::stringstream ss;

	ss << XMLpath;

	doc.LoadFile(ss.str().c_str());

	//this checks the row of the .tmx file which is marked <TextureAtlas>
	XMLElement* TexNode = doc.FirstChildElement("TextureAtlas");

	const char* sheetPath;

	TexNode->QueryStringAttribute("imagePath", &sheetPath);

	AddTexture(tID, sheetPath);

	XMLElement* p_SubTexture = TexNode->FirstChildElement("SubTexture");

	if (p_SubTexture != NULL)
	{
		while (p_SubTexture)
		{
			const char* textureName;
			p_SubTexture->QueryStringAttribute("name", &textureName);

			SDL_Rect sourceRect;
			p_SubTexture->QueryIntAttribute("x", &sourceRect.x);
			p_SubTexture->QueryIntAttribute("y", &sourceRect.y);
			p_SubTexture->QueryIntAttribute("width", &sourceRect.w);
			p_SubTexture->QueryIntAttribute("height", &sourceRect.h);
			
			textureSrcRects.emplace(textureName, sourceRect);

			p_SubTexture = p_SubTexture->NextSiblingElement("SubTexture");
		}
	}
}

//add font to the manager
void AssetManager::AddFont(std::string fID, const char * path, int fontSize)
{
	fonts.emplace(fID, TTF_OpenFont(path, fontSize));
}

//retrieve texture using the given ID for the texture
SDL_Texture * AssetManager::GetTexture(std::string tID)
{
	return textures[tID];
}

//retrieve font using the given ID for the texture
TTF_Font * AssetManager::GetFont(std::string fID)
{
	return fonts[fID];
}

SDL_Rect AssetManager::getSourceRect(std::string tID)
{
	return textureSrcRects[tID];
}


