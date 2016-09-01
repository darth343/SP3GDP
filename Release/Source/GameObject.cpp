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
			//Check X axis
			(
				//Check if Player Minimum X is within the mesh
				(playerPos.x < GameObjectCoordOnScreen.x + this->scale.x && playerPos.x > GameObjectCoordOnScreen.x) ||
				//Check if Player Maximum X is within the mesh
				(playerPos.x + 32.f < GameObjectCoordOnScreen.x + this->scale.x && playerPos.x + 32.f  > GameObjectCoordOnScreen.x)
			) &&
			//Check Y axis
			(
				//Check if Player Minimum y is within the mesh
				(playerPos.y < GameObjectCoordOnScreen.y + this->scale.y && playerPos.y > GameObjectCoordOnScreen.y) ||
				//Check if Player Maximum y is within the mesh
				(playerPos.y + 32.f < GameObjectCoordOnScreen.y + this->scale.y && playerPos.y + 32.f  > GameObjectCoordOnScreen.y)
			)

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