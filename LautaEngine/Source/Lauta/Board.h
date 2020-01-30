#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include "LautaMath.h"

struct TileSet;
struct Tile;

class Board
{
public:

	Board();
	~Board();

	std::vector<Tile> LoadTiles(std::string MapName, int worldScale);

	Vector2Di getMapSize()
	{
		return this->m_mapSize;
	}

private:

	Vector2Di m_size;
	Vector2Di m_tileSize;
	Vector2Di m_mapSize;

	std::vector<TileSet> m_TileSets;

};

struct TileSet
{
	SDL_Texture* tex;
	Uint32 FirstGid;
	std::string tilesetName;
	TileSet() { this->FirstGid = -1; };
	TileSet(SDL_Texture* texture, int Gid, std::string name)
	{
		this->tex = texture;
		this->FirstGid = Gid;
		this->tilesetName = name;
	}
};

struct Tile
{
	Vector2Di posOnMap;
	Vector2Di s;
	Vector2Di posOnTs;
	std::string TsID;
	SDL_RendererFlip flip;
	int angle;
	bool vis;

	//Tile Constructor
	Tile(
		Vector2Di positionOnMap,
		Vector2Di size,
		Vector2Di positionOnTileSheet,
		std::string tileSetID,
		SDL_RendererFlip flippedState,
		int rotationAngle,
		bool visibility
	)
	{
		this->posOnMap = positionOnMap;
		this->s = size;
		this->posOnTs = positionOnTileSheet;
		this->TsID = tileSetID;
		this->flip = flippedState;
		this->angle = rotationAngle;
		this->vis = visibility;
	}
};

