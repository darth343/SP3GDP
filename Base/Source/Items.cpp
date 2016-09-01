#include "Items.h"
#include "SharedData.h"
Items::Items(Vector3 scale) : GameObject(scale)
{
	itemType = POTION;
}

Items::~Items()
{
}

void Items::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	if (this->active && CheckCollision(playerPos, mapOffset, m_cMap))
	{
		SharedData::GetInstance()->soundManager.playSE("Sound//pickUp.mp3");

		SharedData::GetInstance()->inventory.addToInventory(this->itemType);
		SharedData::GetInstance()->inventory.addToInventory(ITEM_TYPE::TRAP);
		active = false;
	}
}
