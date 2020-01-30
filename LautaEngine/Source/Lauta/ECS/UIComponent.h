#pragma once

#include <string>
#include "ECS.h"
#include "Components.h"
#include "SDL.h"
#include "../Board.h"

extern Manager manager;

//std::string UIName, Group groupLabel, int worldScale
class UIComponent : public Component
{
public:

	UIComponent() = default;

	//std::string UI_Name, groupLabel, float world scale
	UIComponent(std::string UIName, Group groupLabel, int scale)
	{
		UI = new Board();

		this->UItiles = UI->LoadTiles(UIName, scale);

		for (auto& t : UItiles)
		{
			auto& UItile(manager.addEntity());

			UItile.addComponent <TransformComponent>(t.posOnMap.x, t.posOnMap.y);
			UItile.addComponent<SpriteComponent>(t.TsID, t.s, t.posOnTs,
				scale, t.flip, t.angle, false);
			UItile.addGroup(groupLabel);
		}
	}

	//Adding a sprite to the UI, this will add a new Tile Component
	void addTileToUI(Vector2Di positionOnUI,
		Vector2Di size,
		Vector2Di positionOnTileSheet,
		std::string tileSetID, Group groupLabel, int worldScale)
	{
		auto& tile(manager.addEntity());

		tile.addComponent <TransformComponent>(positionOnUI.x, positionOnUI.y);
		tile.addComponent<SpriteComponent>(tileSetID, size, positionOnTileSheet,
			worldScale, SDL_FLIP_NONE,0, false);
		tile.addGroup(groupLabel);
	}
	
private:

	Board* UI;
	std::vector<Tile> UItiles;

};


class MapComponent : public Component
{
public:

	MapComponent() = default;

	MapComponent(std::string mapName, Group groupLabel, int worldScale)
	{
		Map = new Board();
		this->mapTiles = Map->LoadTiles(mapName, worldScale);

		for (auto& mt : mapTiles)
		{
			auto& UItile(manager.addEntity());

			UItile.addComponent <TransformComponent>(mt.posOnMap.x, mt.posOnMap.y);
			UItile.addComponent<SpriteComponent>(mt.TsID, mt.s, mt.posOnTs,
				worldScale, mt.flip, mt.angle, true);
			UItile.addGroup(groupLabel);
		}
	}



	~MapComponent() 
	{
		entity->destroy();
	}

	Vector2Di getMapAreaSize()
	{
		return Map->getMapSize();
	}

private:

	Board* Map;
	std::vector<Tile> mapTiles;

};


