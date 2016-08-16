#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h"
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
	E_ATTACK,
	E_DEAD,
	E_TOTAL,
};
	void TakeDamage(int);
	float GetHealth();
	float GetDamage();
	void SetDamage(float dmg);
	int GetMonsterID();
	void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	friend std::ostream& operator<<(std::ostream& cout, ENEMY_STATES state);

private:
	float health;
	float damage;
	int monsterID;
	ENEMY_STATES enemyStates;
};

#endif // !ENEMY_H
