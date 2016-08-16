#include "Items.h"

Items::Items()
{
}

Items::~Items()
{
}

void Items::addPotion()
{
	numPotions += 1;
}

void Items::addTrap()
{
	numTraps += 1;
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



