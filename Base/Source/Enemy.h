#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
	Enemy();
	~Enemy();

	enum ENEMY_STATES
	{
		E_NOSTATE,
		E_IDLE,
		E_CHASE,
		E_ATTACK,
		E_DEAD,
		E_TOTAL,
	};
	void TakeDamage(int);
	float GetHealth();
	float GetDamage();
	int GetMonsterID();
	void Update(float);

private:
	float health;
	float damage;
	int monsterID;
	ENEMY_STATES enemyStates;
};

#endif // !ENEMY_H
