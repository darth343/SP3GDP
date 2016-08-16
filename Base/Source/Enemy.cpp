#include "Enemy.h"

Enemy::Enemy()
{
	enemyStates = E_NOSTATE;
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
void Enemy::Update(float dt)
{

}