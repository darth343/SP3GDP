#ifndef TAMAGUCCI_H
#define TAMAGUCCI_H
#include "Vector3.h"
#include <vector>
using std::vector;
class TAMAGUCCI
{
	enum TAMABUTTONS
	{
		T_NOTHING,
		T_FOOD,
		T_SLEEP,
		T_ENTERTAINMENT,
		T_CLEAN,
		T_STATS,
		T_BOND,
		TOTAL_TBUTTONS,
	};
public:
	TAMAGUCCI();
	~TAMAGUCCI();
	void UpdateTamagucci();
	void GetTamagucciInput();
	friend std::ostream& operator<<(std::ostream& cout, TAMABUTTONS buttons);
private:
	TAMABUTTONS button;
	vector<Vector3> pooPositions;
	int hungerLevel;
	int energyLevel;
	int happinessLevel;
};
#endif // !TAMAGUCCI_H
