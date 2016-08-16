#ifndef ITEM_H
#define ITEM_H
#include "GameObject.h"
#include <string>

using std::string;

enum ITEM_TYPE
{
	POTION,
	TRAP,

	ITEM_TOTAL,
};
inline
	std::ostream& operator<<(std::ostream& cout, ITEM_TYPE type)
{
		switch (type)
		{
		case POTION:
			cout << "POTION";
			return cout;
		case TRAP:
			cout << "TRAP";
			return cout;
		}
	}
class Items : public GameObject
{
public:


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

private:
	string itemName; // Name of items
	ITEM_TYPE itemType;
	int numPotions = 0; // Number of Potions player has
	int numTraps = 0; // Number of Traps player has

};

#endif