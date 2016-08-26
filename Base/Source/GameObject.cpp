#include "GameObject.h"

GameObject::GameObject(Vector3 scale)
: scale(scale)
{
	Min.SetZero(); Max = scale;
	active = true;
}

bool GameObject::CheckCollision(Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	Vector3 GameObjectCoordOnScreen = this->position - mapOffset;
	if (
		(playerPos.x < GameObjectCoordOnScreen.x + this->scale.x && playerPos.x > GameObjectCoordOnScreen.x) // X AXIS COLLISION CHECK
	&& (playerPos.y < GameObjectCoordOnScreen.y + this->scale.y && playerPos.y > GameObjectCoordOnScreen.y) // Y AXIS COLLISION CHECK
		)
	{
		return true;
	}
	if (
		(playerPos.x < GameObjectCoordOnScreen.x + this->scale.x && playerPos.x > GameObjectCoordOnScreen.x) // X AXIS COLLISION CHECK
		&& playerPos.y == GameObjectCoordOnScreen.y // Y AXIS COLLISION CHECK
		)
	{
		return true;
	}

	if (
		playerPos.x == GameObjectCoordOnScreen.x // X AXIS COLLISION CHECK
		&& (playerPos.y < GameObjectCoordOnScreen.y + this->scale.y && playerPos.y > GameObjectCoordOnScreen.y) // Y AXIS COLLISION CHECK
		)
	{
		return true;
	}
	return false;
}

bool GameObject::CheckCollision(GameObject* go, CMap* m_cMap)
{
	return !(go->position.x > this->position.x + this->scale.x || go->position.x + go->scale.x < this->position.x || go->position.y + go->scale.y < this->position.y || go->position.y > this->position.y + this->scale.y);
}