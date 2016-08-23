#ifndef INVENTORY_H
#define INVENTORY_H
#include "Items.h"
#include "Equipment.h"
#include "Monster.h"
#include <Vector>

using std::vector;

class Inventory : public GameObject
{
public:
	Inventory(Vector3 scale = Vector3(1, 1, 1));
	~Inventory();

	void addToInventory(Items::ITEM_TYPE);
	void addToInventory(Monster monster);
	void removeFromInventory(Items::ITEM_TYPE);
	void printInventory();
	int getPotionInventory();
	int getTrapInventory();

private:
	vector<Equipment> EQinventory;
	int ItemInventory[Items::ITEM_TOTAL];
	int numPotions = 0; // Number of Potions player has
	int numTraps = 0; // Number of Traps player has
};




#endif