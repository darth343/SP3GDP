#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h"
#include "Pathfinder.h"
#include "Tile.h"

class Enemy : public GameObject
{
public:
	Enemy(Vector3 scale = Vector3(1, 1, 1), int ID = 0);
	~Enemy();

enum ENEMY_STATES
{
	E_NOSTATE,
	E_IDLE,
	E_PATROL,
	E_CHASE,
	E_DEAD,
	E_TOTAL,
};
	void TakeDamage(int);
	float GetHealth();
	float GetDamage();
	void SetDamage(float dmg);
	int GetMonsterID();
	bool GetFlipStatus();
	void MoveTo(double dt, Tile nextTile, CMap* m_cMap);
	void MoveLeftRight(double dt, bool left);
	void MoveUpDown(double dt, bool up);
	void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	friend std::ostream& operator<<(std::ostream& cout, ENEMY_STATES state);
	Pathfinder thePath;
private:
	float health;
	float damage;
	int monsterID;
	Vector3 prevHeroPos;
	Vector3 patrolPos;
	//Pathfinder thePath;
	ENEMY_STATES enemyStates;
	//Rendering Variables
	bool flip;
};

#endif // !ENEMY_H
