#include "Enemy.h"
#include "Application.h"

const float MOVEMENTSPEED = 0.f;
const float IDLE_TIMER = 1.0;
const int TILES_BEFORECHASING = 7;
const int TILES_BEFOREPATROLLING = 12;

Enemy::Enemy(Vector3 scale, int ID)
: GameObject(scale)
, monsterID(ID)
, flip(false)
{
	enemyStates = E_IDLE;
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

bool Enemy::GetFlipStatus()
{
	return flip;
}

void Enemy::MoveLeftRight(double dt, bool left)
{
	if (left)
	{
		position.x -= dt * MOVEMENTSPEED;
		flip = false;
	}
	else
	{
		position.x += dt * MOVEMENTSPEED;
		flip = true;
	}
}

void Enemy::MoveUpDown(double dt, bool up)
{
	if (up)
	{
		position.y += dt * MOVEMENTSPEED;
	}
	else
	{
		position.y -= dt * MOVEMENTSPEED;
	}
}

void Enemy::MoveTo(double dt, Tile nextTile, CMap* m_cMap)
{
	if (position.x > nextTile.Pos.x * m_cMap->GetTileSize())
	{
		MoveLeftRight(dt, true);
		if (position.x < nextTile.Pos.x * m_cMap->GetTileSize())
		{
			position.x = nextTile.Pos.x * m_cMap->GetTileSize();
		}
	}
	else if (position.x < nextTile.Pos.x * m_cMap->GetTileSize())
	{
		MoveLeftRight(dt, false);
		if (position.x > nextTile.Pos.x * m_cMap->GetTileSize())
		{
			position.x = nextTile.Pos.x * m_cMap->GetTileSize();
		}
	}
	if (position.y > nextTile.Pos.y * m_cMap->GetTileSize())
	{
		MoveUpDown(dt, false);
		if (position.y < nextTile.Pos.y * m_cMap->GetTileSize())
		{
			position.y = nextTile.Pos.y * m_cMap->GetTileSize();
		}
	}
	else if (position.y < nextTile.Pos.y * m_cMap->GetTileSize())
	{
		MoveUpDown(dt, true);
		if (position.y > nextTile.Pos.y * m_cMap->GetTileSize())
		{
			position.y = nextTile.Pos.y * m_cMap->GetTileSize();
		}
	}
}

void Enemy::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	if (CheckCollision(playerPos, mapOffset, m_cMap))
	{
		SceneText* theScene = (SceneText*)Application::GetInstance().GetScene();
		theScene->EnterBattleScene();
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
		if (IDLE_TIME > IDLE_TIMER)
		{
			if (patrolPos.IsZero())
			{
				Vector3 RandMapPos(Math::RandIntMinMax(0, m_cMap->GetNumOfTiles_Width()-1), Math::RandIntMinMax(0, m_cMap->GetNumOfTiles_Height()-1), 0);
				if (m_cMap->theMap[RandMapPos.y][RandMapPos.x].shouldCollide)
					return;
				patrolPos = RandMapPos;
			}
			else
			{
				Vector3 EnemyPos = (position)* (1.f / m_cMap->GetTileSize());
				thePath.FindPath(m_cMap->theMap[EnemyPos.y][EnemyPos.x], m_cMap->theMap[patrolPos.y][patrolPos.x], m_cMap);
				if (thePath.found)
				{
					patrolPos.SetZero();
					enemyStates = E_PATROL;
				}
			}
		}
		int TilesInBetween = ((playerPos + mapOffset) - position).Length() / m_cMap->GetTileSize();
		if (TilesInBetween <= TILES_BEFORECHASING)
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
		if (thePath.found)
		{
			Tile nextTile = thePath.pathToEnd.back();
			MoveTo(dt, nextTile, m_cMap);
			if ((nextTile.Pos * m_cMap->GetTileSize() - position).Length() < 5.f)
			{
				if (thePath.pathToEnd.size() > 1)
				{
					thePath.pathToEnd.pop_back();
				}
				else
				{
					thePath.initializedStartandEnd = false;
					thePath.found = false;
					enemyStates = E_IDLE;
				}
			}
		}

		int TilesInBetween = ((playerPos + mapOffset) - position).Length() / m_cMap->GetTileSize();
		if (TilesInBetween <= TILES_BEFORECHASING)
		{
			thePath.initializedStartandEnd = false;
			enemyStates = E_CHASE;
		}
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
		Vector3 EnemyPos = (position) * (1.f / m_cMap->GetTileSize());
		Vector3 HeroPos = (playerPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if ((EnemyPos - HeroPos).Length() > 1.f)
		thePath.FindPath(m_cMap->theMap[EnemyPos.y][EnemyPos.x], m_cMap->theMap[HeroPos.y][HeroPos.x], m_cMap);
		
		if (thePath.found)
		{
			Tile nextTile = thePath.pathToEnd.back();
			MoveTo(dt, nextTile, m_cMap);
			if ((nextTile.Pos * m_cMap->GetTileSize() - position).Length() < 5.f)
			{
				if (thePath.pathToEnd.size() > 1)
				{
					thePath.pathToEnd.pop_back();
					if (abs((prevHeroPos - HeroPos).Length()) > 1)
					{
						thePath.initializedStartandEnd = false;
					}
				}
				else
				{
					thePath.initializedStartandEnd = false;
					thePath.found = false;
				}
			}
		}
		prevHeroPos = HeroPos;
		if (TilesInBetween >= TILES_BEFOREPATROLLING)
			enemyStates = E_IDLE;
		break;
	}
	}
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
	}
}