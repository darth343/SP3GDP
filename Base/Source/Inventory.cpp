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
	if (type == Items::POTION)
	{
		ItemInventory[Items::POTION]--;
	}
	if (type == Items::TRAP)
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

void Inventory::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	//if (this->active && CheckCollision(playerPos, mapOffset, m_cMap))
	//{
	//	addToInventory(Items::POTION);
	//	printInventory();
	//	active = false;
	//}
}