#include "GameObject.h"

GameObject::GameObject(Vector3 scale)
: scale(scale)
{
	Min.SetZero(); Max = scale;
	active = true;
}

bool GameObject::CheckCollision(Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	if (
		((playerPos.x < this->position.x + this->Max.x - mapOffset.x && playerPos.x > this->position.x + this->Min.x - mapOffset.x) || (playerPos.x + m_cMap->GetTileSize() < this->position.x + this->Max.x - mapOffset.x && playerPos.x + m_cMap->GetTileSize() > this->position.x + this->Min.x - mapOffset.x))
		&&
		((playerPos.y < this->position.y + this->Max.y - mapOffset.y && playerPos.y > this->position.y + this->Min.y - mapOffset.y) || (playerPos.y + m_cMap->GetTileSize()< this->position.y + this->Max.y - mapOffset.y && playerPos.y + m_cMap->GetTileSize()> this->position.y + this->Min.y - mapOffset.y))
		)
	{
		return true;
	}
	else
	{
		return false;
	}


}