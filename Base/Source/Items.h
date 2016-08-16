#pragma once
#include <string>

using std::string;

	enum ITEM_TYPE
	{
		POTION,
		TRAP,

		ITEM_TOTAL,
	};

class Items
{
public:


	Items();
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

private:
	string itemName; // Name of items
	int numPotions = 0; // Number of Potions player has
	int numTraps = 0; // Number of Traps player has

};

