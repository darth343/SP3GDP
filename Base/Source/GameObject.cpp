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

bool GameObject::CheckCollision(GameObject* go, CMap* m_cMap)
{
	return !(go->position.x > this->position.x + this->scale.x || go->position.x + go->scale.x < this->position.x || go->position.y + go->scale.y < this->position.y || go->position.y > this->position.y + this->scale.y);
}