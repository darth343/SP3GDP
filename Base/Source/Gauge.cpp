#include "Gauge.h"

Gauge::Gauge(Vector3 scale) 
: GameObject(scale)
, moveRight(true)
{
}
Gauge::~Gauge()
{
}
void Gauge::Update(double dt, float speed)
{
	if (moveRight)
	{
		this->position.x += speed * dt;
	}
	if (this->position.x + this->scale.x >= 650)
	{
		this->position.x = 650 - this->scale.x;
		moveRight = false;
		moveLeft = true;
	}
	if (!moveRight)
	{
		if (moveLeft)
			this->position.x -= speed * dt;
		if (this->position.x <= 150)
		{
			this->position.x = 150;
			moveRight = true;
			moveLeft = false;
		}
	}
}