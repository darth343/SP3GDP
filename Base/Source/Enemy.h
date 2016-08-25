#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h"
#include "Pathfinder.h"
#include "Tile.h"
#include "Monster.h"

class Enemy : public GameObject
{
public:
	Enemy(Monster monster, Vector3 scale = Vector3(1, 1, 1));
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
	bool GetFlipStatus();
	void MoveTo(double dt, Tile nextTile, CMap* m_cMap);
	void MoveLeftRight(double dt, bool left);
	void MoveUpDown(double dt, bool up);
	void SetName(string name);
	string GetName();
	Monster GetMonster();
	void SetAttackDamage(int);
	void SetDef(int);
	int GetDef();
	int GetAttackDamage();
	float GetMaxHealth();
	void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	friend std::ostream& operator<<(std::ostream& cout, ENEMY_STATES state);

private:
	string enemyName;
	Vector3 prevHeroPos;
	Vector3 patrolPos;
	//Pathfinder thePath;
	Pathfinder thePath;
	ENEMY_STATES enemyStates;
	Monster monster;
	//Rendering Variables
	bool flip;
};

#endif // !ENEMY_H
