#include "Inventory.h"

Inventory::Inventory(Vector3 scale) : GameObject(scale)
{
}

Inventory::~Inventory()
{
}

void Inventory::addToInventory(Items::ITEM_TYPE type)
{
	if (type == Items::POTION)
	{
		ItemInventory[Items::POTION]++;
	}
	if (type == Items::TRAP)
	{
		ItemInventory[Items::TRAP]++;
	}
}

void Inventory::removeFromInventory(Items::ITEM_TYPE type)
{
	if (type == Items::POTION && ItemInventory[Items::POTION] != 0)
	{
		ItemInventory[Items::POTION]--;
	}
	if (type == Items::TRAP && ItemInventory[Items::TRAP] != 0)
	{
		ItemInventory[Items::TRAP]--;
	}
}


void Inventory::addToInventory(Monster monster)
{
	Equipment temp;
	temp.setEQAtk(monster.getMonsterAtk());
	temp.setEQDef(monster.getMonsterDef());
	temp.setEQHP(monster.getMonsterHP());

	EQinventory.push_back(temp);
}


void Inventory::printInventory()
{
	for (int i = 0; i < Items::ITEM_TOTAL; i++)
	{
		cout << ItemInventory[i] << endl;
	}
	return;
}

int Inventory::getPotionInventory()
{	
	return ItemInventory[0];	
}

int Inventory::getTrapInventory()
{
	return ItemInventory[1];
}

