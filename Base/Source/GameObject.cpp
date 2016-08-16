#include "GameObject.h"

GameObject::GameObject(Vector3 scale)
: scale(scale)
{
	Min.SetZero(); Max = scale;
}

bool GameObject::CheckCollision(CPlayerInfo* player, CMap* m_cMap)
{
	if (
		((player->GetPosition().x < this->position.x + this->Max.x - player->GetMapOffset().x && player->GetPosition().x > this->position.x + this->Min.x - player->GetMapOffset().x) || (player->GetPosition().x + m_cMap->GetTileSize() < this->position.x + this->Max.x - player->GetMapOffset().x && player->GetPosition().x + m_cMap->GetTileSize() > this->position.x + this->Min.x - player->GetMapOffset().x))
		&&
		((player->GetPosition().y < this->position.y + this->Max.y - player->GetMapOffset().y && player->GetPosition().y > this->position.y + this->Min.y - player->GetMapOffset().y) || (player->GetPosition().y + m_cMap->GetTileSize()< this->position.y + this->Max.y - player->GetMapOffset().y && player->GetPosition().y + m_cMap->GetTileSize()> this->position.y + this->Min.y - player->GetMapOffset().y))
		)
	{
		return true;
	}
	else
	{
		return false;
	}


}