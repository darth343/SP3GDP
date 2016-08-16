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
	static float IDLE_TIME = 0.f;
	static float PATROL_TIME = 0.f;
	switch (enemyStates)
	{
	case E_IDLE:
	{
		IDLE_TIME += dt;
		if (PATROL_TIME > 0.f)
		{
			PATROL_TIME = 0.f;
		}
		if (IDLE_TIME > 2.f)
		{
			enemyStates = E_PATROL;
		}
		int TilesInBetween = ((playerPos + mapOffset) - position).Length() / m_cMap->GetTileSize();
		if (TilesInBetween <= 2)
			enemyStates = E_CHASE;
		break;
	}
	case E_PATROL:
	{
		PATROL_TIME += dt;
		if (IDLE_TIME > 0.f)
		{
			IDLE_TIME = 0.f;
		}
		if (PATROL_TIME > 5.f)
		{
			enemyStates = E_IDLE;
		}
		int TilesInBetween = ((playerPos + mapOffset) - position).Length() / m_cMap->GetTileSize();
		if (TilesInBetween <= 2)
			enemyStates = E_CHASE;
		break;
	}
	case E_CHASE:
	{
		int TilesInBetween = ((playerPos + mapOffset) - position).Length() / m_cMap->GetTileSize();
		if (IDLE_TIME > 0.f)
		{
			IDLE_TIME = 0.f;
		}
		if (PATROL_TIME > 0.f)
		{
			PATROL_TIME = 0.f;
		}
		if (TilesInBetween >= 7)
			enemyStates = E_PATROL;
		break;
	}
	case E_ATTACK:
	{
		if (IDLE_TIME > 0.f)
		{
			IDLE_TIME = 0.f;
		}
		if (PATROL_TIME > 0.f)
		{
			PATROL_TIME = 0.f;
		}
		break;
	}
	}

	//cout << enemyStates << endl;
}

std::ostream& operator<<(std::ostream& cout, Enemy::ENEMY_STATES state)
{
	switch (state)
	{
	case Enemy::E_IDLE:
		{
			return cout << "IDLING";
		}
	case Enemy::E_PATROL:
		{
			return cout << "PATROLLING";
		}
	case Enemy::E_CHASE:
		{
			return cout << "CHASING";
		}
	case Enemy::E_ATTACK:
		{
			return cout << "ATTACKING";
		}
	}
}