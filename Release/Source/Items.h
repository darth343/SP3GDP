#ifndef ITEM_H
#define ITEM_H
#include "GameObject.h"
#include <string>

using std::string;


class Items : public GameObject
{
public:
	enum ITEM_TYPE
	{
		POTION,
		TRAP,
		ENCRYPTED_MEMORY,
		ITEM_TOTAL,
	};

	Items(Vector3 scale = Vector3(1, 1, 1));
	~Items();
	
	void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	ITEM_TYPE itemType;
	
private:
	string itemName; // Name of items
};

#endif