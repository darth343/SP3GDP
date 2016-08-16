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

		ITEM_TOTAL,
	};


	Items(Vector3 scale = Vector3(1, 1, 1));
	~Items();
	
	// Function to increase Item Count
	void addPotion();
	void addTrap();

	//Function to decrease Item Count
	void removePotion();
	void removeTrap();

	// Getters for Items
	int getPotionCount();
	int getTrapCount();
	void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	ITEM_TYPE itemType;
private:
	string itemName; // Name of items

	int numPotions = 0; // Number of Potions player has
	int numTraps = 0; // Number of Traps player has

};

#endif