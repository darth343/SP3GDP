#include "Enemy.h"

Enemy::Enemy()
{
	enemyStates = E_NOSTATE;
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
void Enemy::Update(float dt)
{

}