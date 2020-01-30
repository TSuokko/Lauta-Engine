#include <sstream>
#include <algorithm>
#include <cmath>

#include "TinyXML/tinyxml2.h"

#include "Board.h"
#include "ECS/Components.h"


const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG	 = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

using namespace tinyxml2;

Board::Board(){}
Board::~Board(){}

std::vector<Tile> Board::LoadTiles(std::string MapPath, int worldScale)
{
	//.tmx is a version of an XML-file
	XMLDocument doc;
	std::stringstream ss;
	ss << MapPath; //when we pass in the whole map path, we get Assets/Maps/______.tmx

	//open the file itself
	doc.LoadFile(ss.str().c_str());

	//this checks the second row of the .tmx file which is marked <map>
	XMLElement* mapNode = doc.FirstChildElement("map");

	//Get the width and height of the whole map and store it in m_size;
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->m_size = Vector2Di(width, height);

	//Get width and height of the tiles and store it in m_tileSize
	int tileWidth, tileHeight;
	int columns, spacing;

	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);

	this->m_tileSize = Vector2Di(tileWidth * worldScale, tileHeight * worldScale);
	this->m_mapSize = m_size.multiply(m_tileSize);

	//Loading the tilesets
	XMLElement* p_TileSet = mapNode->FirstChildElement("tileset");
	if (p_TileSet != NULL)
	{
		//allows us to get multiple tilesets from one .tmx file
		while (p_TileSet)
		{
			int firstGid;

			const char* tilesetName;

			const char* source = p_TileSet->FirstChildElement("image")->Attribute("source");

			std::stringstream ss;
			ss << "Assets/Maps/" << source;

			p_TileSet->QueryIntAttribute("firstgid", &firstGid);

			p_TileSet->QueryStringAttribute("name", &tilesetName);

			 p_TileSet->QueryIntAttribute("columns", &columns);
			 p_TileSet->QueryIntAttribute("spacing", &spacing);


			AssetManager::Assets->AddTexture(tilesetName, ss.str().c_str());

			SDL_Texture* tex = AssetManager::Assets->GetTexture(ss.str().c_str());

			this->m_TileSets.push_back(TileSet(tex, firstGid, tilesetName));

			p_TileSet = p_TileSet->NextSiblingElement("tileset");
		}
	}

	//Loading all the layers
	XMLElement* p_Layer = mapNode->FirstChildElement("layer");

	std::vector<Tile> TOB;

	if (p_Layer != NULL)
	{
		while (p_Layer)
		{
			//check visibility
			bool visible = true;
			p_Layer->QueryBoolAttribute("visible", &visible);

			//loading the data element
			XMLElement* p_Data = p_Layer->FirstChildElement("data");

			if (p_Data != NULL)
			{

				while (p_Data)
				{

					const char* tileArray;
					tileArray = p_Data->GetText();

					std::string tileString(tileArray);

					std::vector<Uint32> vect;
					std::stringstream ss(tileString);

					Uint32 i;

					//remove the commas while reading the map
					while (ss >> i)
					{
						vect.push_back(i);
						if (ss.peek() == ',')
						{
							ss.ignore();
						}
					}

					//vect.size = how many tiles there are
					for (int j = 0; j < vect.size(); j++)
					{
						if (vect.at(j) != 0)
						{
							//std::cout << "ID: " << vect.at(j) << std::endl;
							SDL_RendererFlip flip = SDL_FLIP_NONE;
							int angle = 0;

							TileSet tls;
							for (unsigned int i = 0; i < this->m_TileSets.size(); i++)
							{
								if (this->m_TileSets[i].FirstGid <= vect.at(j))
								{

									//This is the tileset we want
									tls = this->m_TileSets.at(i);

									break;
								}
							}
							if (tls.FirstGid == -1)
							{
								printf("Error, tile not found");

							}

							//check tile positioning, if it's flipped or rotated
							unsigned global_tile_id = vect.at(j);

							bool flipped_horizontally = (global_tile_id & FLIPPED_HORIZONTALLY_FLAG);
							bool flipped_vertically = (global_tile_id & FLIPPED_VERTICALLY_FLAG);
							bool flipped_diagonally = (global_tile_id & FLIPPED_DIAGONALLY_FLAG);

							if (flipped_horizontally && flipped_vertically && flipped_diagonally)
							{
								flip = SDL_FLIP_HORIZONTAL;
								angle = 90;
							}
							else if (flipped_horizontally && flipped_vertically)
								angle = 180;
							else if (flipped_horizontally && flipped_diagonally)
								angle = 90;
							else if (flipped_vertically && flipped_diagonally)
								angle = 270;
							else if (flipped_horizontally)
								flip = SDL_FLIP_HORIZONTAL;
							else if (flipped_vertically)
								flip = SDL_FLIP_VERTICAL;
							else if (flipped_diagonally)
							{
								angle = 90;
								flip = SDL_FLIP_VERTICAL;
							}

							global_tile_id &= ~(FLIPPED_HORIZONTALLY_FLAG |
								FLIPPED_VERTICALLY_FLAG |
								FLIPPED_DIAGONALLY_FLAG);

							int xx = 0;
							int yy = 0;
							xx = j % width;
							xx *= tileWidth * worldScale;
							yy += tileHeight * (j / width) * worldScale;
							Vector2Di finalTilePosition = Vector2Di(xx, yy);

							//Calculate the position of the tile in the tileset
							int tsxx = 0;
							int tsyy = 0;

							//border tile
							if ((global_tile_id) % (columns) == 0)
							{
								tsxx = ((((global_tile_id - spacing) % (columns)) + spacing) * (tileWidth + spacing)) - (tileWidth + spacing);
								tsyy = (global_tile_id / (columns)) * (tileHeight + spacing) - (tileWidth + spacing);
							}
							//every other tile
							else
							{
								tsxx = (((global_tile_id) % (columns))* (tileWidth + spacing)) - (tileWidth + spacing);
								tsyy = (global_tile_id / (columns)) * (tileHeight + spacing);
							}

							Vector2Di finalTileSetPosition = Vector2Di(tsxx, tsyy);

							if (!visible)
							{
								tls.tex = NULL;
							}

							//Build the actual tile and add it 

							//std::cout << finalTilePosition << " "<<finalTileSetPosition << " "<<tls.tilesetName << std::endl;

							TOB.push_back(Tile(finalTilePosition, Vector2Di(tileWidth, tileHeight),
								finalTileSetPosition, tls.tilesetName, flip, angle, visible));
						}
					}
					p_Data = p_Data->NextSiblingElement("data");
				}
			}
			p_Layer = p_Layer->NextSiblingElement("layer");
		}
	}

	return TOB;
}

