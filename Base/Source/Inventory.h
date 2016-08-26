#ifndef INVENTORY_H
#define INVENTORY_H
#include "Items.h"
#include "Equipment.h"
#include "Vector3.h"
#include "Enemy.h"
#include <Vector>

using std::vector;

class Inventory
{
public:
	enum INVENTORY_STATE
	{
		TAB1,
		TAB2
	};

	enum INPUT_STATE
	{
		INVENTORY,
		EQUIP_OPTIONS
	};

	Inventory(Vector3 scale = Vector3(1, 1, 1));
	~Inventory();

	void addToInventory(Items::ITEM_TYPE);
	void addToInventory(Enemy* enemy);
	void removeFromInventory(Items::ITEM_TYPE);
	void removeFromInventory(Equipment* equipment);
	void printInventory();
	void UpdateSeeker();
	void UpdateInput();
	void Update(double dt);
	void ResetInventory();
	void SortInventory();
	void SortByType(vector<Equipment*>);
	void SortByStats(vector<Equipment*> list, int index);
	void EquipItem(string itemType);
	int GetPotionCount();
	int GetTotalATK();
	int GetTotalDEF();
	INVENTORY_STATE getState();
	INPUT_STATE getInputState();
	Vector3 getSeeker();
	int getSecondSeeker();
	Equipment* getEquipmentLookAt();
	vector<string> getOptions();
	void setEquipmentLookAt(Equipment* equipment);
	Equipment* getHead();
	Equipment* getLeftArm();
	Equipment* getRightArm();
	Equipment* getArmour();
	Equipment* getLeg();
	vector<Equipment*> EQinventory;
	vector<string> options;
private:
	Equipment* head;
	Equipment* leftArm;
	Equipment* rightArm;
	Equipment* armour;
	Equipment* leggings;
	Vector3 seeker;
	int secondSeeker;
	INVENTORY_STATE state;
	INPUT_STATE inputeState;
	int ItemInventory[Items::ITEM_TOTAL];
};




#endif