#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Vector3.h"
#include "Map.h"
#include <string>
using std::string;

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE,
		GO_ITEM,
		GO_ENEMY,
		GO_NPC,
		GO_TOTAL,
	};
	GameObject(Vector3 scale = Vector3(1, 1, 1));
	~GameObject() {};
	virtual void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap){};
	bool CheckCollision(Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	bool CheckCollision(GameObject* go, CMap* m_cMap);
	void CollisionResponse();
	bool active;
	GAMEOBJECT_TYPE type;
	Vector3 position;
	Vector3 direction;
	Vector3 scale;

	// Collision Stuff
	Vector3 Min;
	Vector3 Max;
};

#endif // !GAMEOBJECT_H
