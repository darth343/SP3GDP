#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Tile.h"
using namespace std;

class CMap
{
public:
	CMap(void);
	~CMap(void);

	void Init(const int theScreen_Height, const int theScreen_Width, int theTileSize = 25);

	bool LoadMap(const string mapName);
	int GetNumOfTiles_Height(void);		// Get the number of tiles for height of the screen
	int GetNumOfTiles_Width(void);		// Get the number of tiles for width of the screen
	int GetTileSize(void);

	int getNumOfTiles_MapHeight(void);		// Get the number of tiles for height of the map
	int getNumOfTiles_MapWidth(void);		// Get the number of tiles for width of the map

	int getScreenWidth(void);		// Get the number of tiles for height of the map
	int getScreenHeight(void);		// Get the number of tiles for width of the map
	
	vector<vector<Tile>> theMap;

	int theScreen_Height;
	int theScreen_Width;
	int theNumOfTiles_Height;		// Number of tiles in the screen's height
	int theNumOfTiles_Width;		// Number of tiles in the screen's width
	int theTileSize;

	int theMap_Height;				// The map's height
	int theMap_Width;				// The map's width
	int theNumOfTiles_MapHeight;	// Number of tiles in the map's height
	int theNumOfTiles_MapWidth;		// Number of tiles in the map's width

	bool LoadFile(const string mapName);
};
