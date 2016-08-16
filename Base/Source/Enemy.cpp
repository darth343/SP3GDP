#include "Enemy.h"

const float MOVEMENTSPEED = 10.f;

Enemy::Enemy(Vector3 scale, int ID)
: GameObject(scale)
, monsterID(ID)
{
	enemyStates = E_PATROL;
}

Enemy::~Enemy()
{
}

void Enemy::TakeDamage(int damage)
{
	health -= damage;
}
float Enemy::GetDamage()
{
	return damage;
}
float Enemy::GetHealth()
{
	return health;
}
int Enemy::GetMonsterID()
{
	return monsterID;
}
void Enemy::Update(double dt, CPlayerInfo* player, CMap* m_cMap)
{
	if (CheckCollision(player, m_cMap))
	{
		cout << "Collided with " << monsterID << endl;
	}
}