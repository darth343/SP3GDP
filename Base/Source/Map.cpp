#include "Map.h"
#include <iostream>
CMap::CMap(void)
: theScreen_Height(0)
, theScreen_Width(0)
, theNumOfTiles_Height(0)
, theNumOfTiles_Width(0)
, theMap_Height(0)
, theMap_Width(0)
, theNumOfTiles_MapHeight(0)
, theNumOfTiles_MapWidth(0)
, theTileSize(0)
{
	theMap.clear();
}

CMap::~CMap(void)
{
	theMap.clear();
}


void CMap::Init(const int theScreen_Height, const int theScreen_Width, int theTileSize)
{
	this->theScreen_Height = theScreen_Height;
	this->theScreen_Width = theScreen_Width;
	this->theTileSize = theTileSize;
}

bool CMap::LoadMap(const string mapName)
{
	if (LoadFile(mapName) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

bool CMap::LoadFile(const string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	ifstream file(mapName.c_str());
	if (file.is_open())
	{
		//Update Map Height && Map Width
		{
			string tempString;
			int NumberOfRows = 0;
			int NumberOfCols = 0;
			while (getline(file, tempString))
			{
				NumberOfRows++;
			}
			theNumOfTiles_Height = NumberOfRows;
			file.clear();
			file.seekg(0);
			getline(file, tempString);
			for (int i = 0; i < tempString.size(); ++i)
			{
				if (tempString[i] == ',')
					++NumberOfCols;
			}
			++NumberOfCols;
			theNumOfTiles_Width = NumberOfCols;
		}
		//Allocate Memory Dynamically For Map
		{
		theMap.resize(theNumOfTiles_Height);
		for (int i = 0; i < theNumOfTiles_Height; ++i)
			theMap[i].resize(theNumOfTiles_Width);
		}
		file.clear();
		file.seekg(0);

		//Populate Map
		{
			int x = 0, y = 0;
			for (y = theNumOfTiles_Height-1; y >= 0; --y)
			{
				string tempString;
				for (x = 0; x < theNumOfTiles_Width-1; ++x)
				{
					getline(file, tempString, ',');
					int ID = std::stoi(tempString);
					if (ID == 0 || ID == 2)
					{
						Tile tempTile(Vector3(x, y), ID, false);
						theMap[y][x] = tempTile;
					}
					else
					{
						Tile tempTile(Vector3(x, y), ID, true);
						theMap[y][x] = tempTile;
					}
				}
				getline(file, tempString, '\n');
				int ID = std::stoi(tempString);
				if (ID == 0 || ID == 2)
				{
					Tile tempTile(Vector3(theNumOfTiles_Width, y), ID, false);
					theMap[y][x] = tempTile;
				}
				else
				{
					Tile tempTile(Vector3(theNumOfTiles_Width, y), ID, true);
					theMap[y][x] = tempTile;
				}
			}
		}
	}
	return true;
}

int CMap::GetNumOfTiles_Height(void)
{
	return theNumOfTiles_Height;
}

int CMap::GetNumOfTiles_Width(void)
{
	return theNumOfTiles_Width;
}

int CMap::GetTileSize(void)
{
	return theTileSize;
}

// Get the number of tiles for height of the map
int CMap::getScreenHeight()
{
	return theScreen_Height;
}

// Get the number of tiles for height of the map
int CMap::getScreenWidth()
{
	return theScreen_Width;
}


// Get the number of tiles for width of the map
int CMap::getNumOfTiles_MapWidth(void)
{
	return theNumOfTiles_MapWidth;
}

// Get the number of tiles for width of the map
int CMap::getNumOfTiles_MapHeight(void)
{
	return theNumOfTiles_MapHeight;
}