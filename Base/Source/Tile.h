#ifndef TILE_H
#define TILE_H
#include "Vector3.h"

class Tile
{
public:
	Tile(Vector3 pos, int ID, bool collidable = true):
		Pos(pos),
		BlockID(ID),
		shouldCollide(collidable)
	{};
	Tile(){};
	~Tile(){};
	int BlockID;
	Vector3 Pos;
	bool shouldCollide;
	void SetPosition();
	void onTrigger();
};

#endif