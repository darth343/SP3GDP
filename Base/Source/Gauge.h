#ifndef GAUGE_H
#define GAUGE_H
#include "GameObject.h"
#include <string>

using std::string;


class Gauge : public GameObject
{
public:
	enum GAUGE_TYPE
	{
		REDBAR,
		GREENBAR,
		MOVE,
	};

	Gauge(Vector3 scale = Vector3(1, 1, 1));
	~Gauge();
	GAUGE_TYPE gauge;

	void Update(double dt = 0, float speed = 200.f);
private:
	string itemName; // Name of items
	bool moveRight = true;
	bool moveLeft = false;
	Vector3 maxscale;

};

#endif