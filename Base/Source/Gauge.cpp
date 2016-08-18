#include "Gauge.h"

Gauge::Gauge(Vector3 scale) : GameObject(scale)
{
}
Gauge::~Gauge()
{
}
void Gauge::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	if (gauge == MOVE)
	{
		if (moveRight)
		{
			this->position.x += 100.f*dt;
		}
		if (this->position.x >= 650)
		{
			moveRight = false;
			moveLeft = true;
		}
		if (!moveRight)
		{
			if (moveLeft)
				this->position.x -= 100.f *dt;
			if (this->position.x <= 150)
			{
				moveRight = true;
				moveLeft = false;
			}
		}
	}
	if (this->active && CheckCollision(playerPos, mapOffset, m_cMap))
	{
		//addPotion();
		//active = false;
	}
}