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
		GO_REDBAR,
		GO_TAMAGUCCI,
		GO_TAMDROP1,
		GO_TAMDROP2,
		GO_GREENBAR,
		GO_MOVE,
		GO_TOTAL,
	};

	GameObject(Vector3 scale = Vector3(1, 1, 1));
	~GameObject() {};
	virtual void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap){};
	bool CheckCollision(Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	bool CheckCollision(GameObject* go, CMap* m_cMap = NULL);
	void CollisionResponse();
	bool active;
	GAMEOBJECT_TYPE type;
	Vector3 position;
	Vector3 direction;
	Vector3 scale;
	int maxDia;
	int currDia;
	int dialogueNum = 0;
	// Collision Stuff
	Vector3 Min;
	Vector3 Max;
};

#endif // !GAMEOBJECT_H
