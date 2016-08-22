#include "Items.h"

Items::Items(Vector3 scale) : GameObject(scale)
{
	itemType = POTION;
}

Items::~Items()
{
}

void Items::addPotion(int count)
{
	numPotions += count;
}

void Items::addTrap(int count)
{
	numTraps += count;
}

void Items::removePotion()
{
	numPotions -= 1;
}

void Items::removeTrap()
{
	numTraps -= 1;
}

int Items::getPotionCount()
{
	return numPotions;
}

int Items::getTrapCount()
{
	return numTraps;
}

void Items::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	if (this->active && CheckCollision(playerPos, mapOffset, m_cMap))
	{
		addPotion(1);
		active = false;
		std::cout << getPotionCount() << std::endl;
	}
}