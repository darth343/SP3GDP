#include "Enemy.h"

const float MOVEMENTSPEED = 10.f;

Enemy::Enemy(Vector3 scale, int ID)
: GameObject(scale)
, monsterID(ID)
{
	enemyStates = E_PATROL;
	health = 100.0f;
	damage = 5.0f;
	monsterID = 0;
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
void Enemy::SetDamage(float dmg)
{
	this->damage = dmg;
}
float Enemy::GetHealth()
{
	return health;
}
int Enemy::GetMonsterID()
{
	return monsterID;
}
void Enemy::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	if (CheckCollision(playerPos, mapOffset, m_cMap))
	{
		cout << "Collided with " << monsterID << endl;
	}
}