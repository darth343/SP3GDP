#include "Gauge.h"
const float GAUGE_SPEED = 200.f;


Gauge::Gauge(Vector3 scale) 
: GameObject(scale)
, moveRight(true)
{
}
Gauge::~Gauge()
{
}
void Gauge::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	if (moveRight)
	{
		this->position.x += GAUGE_SPEED * dt;
	}
	if (this->position.x >= 650)
	{
		this->position.x = 650;
		moveRight = false;
		moveLeft = true;
	}
	if (!moveRight)
	{
		if (moveLeft)
			this->position.x -= GAUGE_SPEED * dt;
		if (this->position.x <= 150)
		{
			this->position.x = 150;
			moveRight = true;
			moveLeft = false;
		}
	}
}